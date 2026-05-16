# XPB Binary Serial Protocol v1

> Language-agnostic specification for PC-side developers.
> All values are **little-endian**. All integers are unsigned unless noted.

---

## 1. Frame Structure

Every frame follows a fixed envelope:

```
Offset  Size  Field       Description
─────────────────────────────────────────────
0       1     SYNC_0      0xAA
1       1     SYNC_1      0x55
2       1     TYPE        Frame type identifier
3       1     LEN         Payload length in bytes (0..255)
4       LEN   PAYLOAD     Type-specific payload
4+LEN   1     CRC8        CRC-8/MAXIM over TYPE + LEN + PAYLOAD
```

**Total frame size** = 5 + LEN bytes.

### Sync Word

The two-byte sync word `0xAA 0x55` marks the start of every frame.
A parser should scan for this pair byte-by-byte and only attempt
to decode `TYPE` once the pair is found.

---

## 2. Frame Types

| TYPE | Name           | LEN | Direction     | Frequency          |
|------|----------------|-----|---------------|--------------------|
| 0x01 | Realtime Data  | 20  | Device → PC   | Periodic (default 100 Hz) |
| 0xC0 | Start          | 0   | PC → Device   | Once per session   |
| 0xC1 | Config ACK     | 0   | PC → Device   | Once per session   |
| 0xC2 | Stop           | 0   | PC → Device   | Once per session   |
| 0xFE | Device Config  | 9   | Device → PC   | Once per session   |

### 2.1 Type 0x01 -- Realtime Data Frame

**Total size**: 25 bytes (5 header/crc + 20 payload)

```
Offset  Size  Type   Field        Unit    Description
─────────────────────────────────────────────────────────
0       4     f32    busV_a       V       Channel A bus voltage
4       4     f32    shuntV_a     V       Channel A shunt voltage
8       4     f32    busV_b       V       Channel B bus voltage
12      4     f32    shuntV_b     V       Channel B shunt voltage
16      4     u32    timestamp    ms      Monotonic timestamp since boot
```

**Derived values** (computed by PC side):

```
current_x  = shuntV_x / shuntR_x      [A]
power_x    = busV_x * current_x        [W]
```

`shuntR_x` comes from the Type 0xFE config frame.

### 2.2 Type 0xC0 -- Start Frame (PC → Device)

**Total size**: 5 bytes (5 header/crc + 0 payload)

PC requests the device to begin a data streaming session.
Empty payload.

```
[0xAA][0x55][0xC0][0x00][CRC8]
```

### 2.3 Type 0xC1 -- Config ACK Frame (PC → Device)

**Total size**: 5 bytes (5 header/crc + 0 payload)

PC confirms successful receipt of the Device Config frame.
The device begins streaming data frames only after receiving this ACK.

```
[0xAA][0x55][0xC1][0x00][CRC8]
```

### 2.4 Type 0xC2 -- Stop Frame (PC → Device)

**Total size**: 5 bytes (5 header/crc + 0 payload)

PC requests the device to stop data streaming and return to idle.
A new session can be started by sending another Start frame.

```
[0xAA][0x55][0xC2][0x00][CRC8]
```

### 2.5 Type 0xFE -- Device Config Frame

**Total size**: 14 bytes (5 header/crc + 9 payload)

Sent once in response to a Start frame, before any data frames.

```
Offset  Size  Type   Field        Unit    Description
─────────────────────────────────────────────────────────
0       4     f32    shuntR_a     ohm     Channel A shunt resistor value
4       4     f32    shuntR_b     ohm     Channel B shunt resistor value
8       1     u8     version      --      Protocol version (0x01 for v1)
```

---

## 3. Session Lifecycle

The protocol uses a three-phase handshake: **request → confirm → stream**.

```
PC                              Device
│                                │  (power-on / idle, no output)
│── START (0xC0) ──────────────>│  PC requests data stream
│                                │
│<────── CONFIG (0xFE) ─────────│  Device responds with configuration
│                                │
│── CONFIG_ACK (0xC1) ─────────>│  PC confirms config received
│                                │  Device begins streaming
│<────── DATA (0x01) ───────────│
│<────── DATA (0x01) ───────────│
│           ...                  │
│── STOP (0xC2) ───────────────>│  PC requests stop (optional)
│                                │  Device stops, returns to idle
```

### 3.1 Device State Machine

```
                    ┌──────────┐
         ┌─────────│   IDLE   │<──────────────┐
         │         └──────────┘               │
         │ recv START                    timeout/STOP
         v                                    │
    ┌────────────┐                   ┌────────────────┐
    │  WAIT_ACK  │──── timeout ────>│  retry ≤ 3     │
    │            │<────────────────  │  then → IDLE   │
    └────────────┘                   └────────────────┘
         │  ^
         │  │ recv START (re-handshake)
         │  │
         │ recv CONFIG_ACK
         v  │
    ┌────────────┐
    │ STREAMING  │──── recv STOP ────> IDLE
    └────────────┘
```

Three states: `IDLE → WAIT_ACK → STREAMING`.

### 3.2 Timeout & Retry

| Scenario | Timeout | Behavior |
|----------|---------|----------|
| Device sends CONFIG, waits for CONFIG_ACK | 500 ms | Resend CONFIG, up to 3 attempts |
| 3 CONFIG retries exhausted | — | Return to IDLE, await next START |
| Device in IDLE receives non-START frame | — | Silently ignore |
| Device in STREAMING receives START | — | Re-handshake: send CONFIG, transition to WAIT_ACK |

### 3.3 Reconnection

The PC may send a START frame at any time without sending STOP first.
Device behavior depends on current state:

- **IDLE**: normal handshake begins.
- **STREAMING**: device aborts the current session, sends a new CONFIG
  frame, and transitions to WAIT_ACK (re-handshake).
- **WAIT_ACK**: START is ignored. The device will return to IDLE after
  retry timeout (~1.5 s), then accept the next START.

No cooldown is required between sessions.

---

## 4. CRC-8/MAXIM

| Parameter  | Value |
|------------|-------|
| Polynomial | 0x31 (x^8 + x^5 + x^4 + 1) |
| Init       | 0x00  |
| RefIn      | false |
| RefOut     | false |
| XorOut     | 0x00  |

**Scope**: CRC is computed over the byte sequence `[TYPE, LEN, PAYLOAD...]`,
i.e. everything between the sync word and the CRC byte itself.

### Reference Implementation (C)

```c
uint8_t crc8_maxim(const uint8_t *data, size_t len) {
    uint8_t crc = 0x00;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int b = 0; b < 8; b++) {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = crc << 1;
        }
    }
    return crc;
}
```

---

## 5. Byte Order

All multi-byte values use **little-endian** byte order.

Example: `float busV_a = 5.0f` is stored as `00 00 A0 40` (IEEE 754 LE).

---

## 6. Synchronization Strategy

### Initial Sync

1. Read bytes one at a time until `0xAA` is found.
2. Read next byte. If `0x55`, sync is acquired. Otherwise, go to step 1.
3. Read `TYPE` (1 byte) and `LEN` (1 byte).
4. Read `LEN` payload bytes + 1 CRC byte.
5. Compute CRC over `[TYPE, LEN, PAYLOAD...]`. If mismatch, discard and go to step 1.
6. Frame is valid. Dispatch by `TYPE`.

### Re-sync on Error

If CRC fails or `LEN` exceeds expected bounds (e.g. > 32 for known types),
discard the current frame candidate and resume scanning from step 1.

### Recommended Buffer Size

A ring buffer of 64 bytes is sufficient for all v1 frame types.

---

## 7. Timing

| Parameter            | Default | Notes                          |
|----------------------|---------|--------------------------------|
| Data frame interval  | 10 ms   | 100 Hz output rate             |
| Config frame         | once    | Sent after START received      |
| Config ACK timeout   | 500 ms  | Per retry, 3 retries max       |
| Baud rate            | 115200  | 8N1, no flow control           |

At 115200 baud (11520 bytes/sec), a 25-byte data frame takes ~2.2 ms,
leaving ~7.8 ms headroom per 10 ms cycle. Max sustainable rate ~460 Hz.

---

## 8. Parser Pseudocode

Both PC and device use the same frame parser. Direction is determined
by `TYPE` value: `0x01`/`0xFE` are device-originated, `0xC0`/`0xC1`/`0xC2`
are PC-originated.

```
state = WAIT_SYNC_0
buffer = []

on_byte_received(byte):
    switch state:
        WAIT_SYNC_0:
            if byte == 0xAA: state = WAIT_SYNC_1
        WAIT_SYNC_1:
            if byte == 0x55: state = READ_TYPE
            elif byte == 0xAA: stay  // consecutive 0xAA
            else: state = WAIT_SYNC_0
        READ_TYPE:
            frame_type = byte
            buffer = [byte]
            state = READ_LEN
        READ_LEN:
            frame_len = byte
            buffer.append(byte)
            remaining = frame_len + 1  // payload + crc
            state = READ_BODY
        READ_BODY:
            buffer.append(byte)
            remaining -= 1
            if remaining == 0:
                crc_received = buffer[-1]
                crc_computed = crc8_maxim(buffer[:-1])
                if crc_received == crc_computed:
                    dispatch(frame_type, buffer[2:-1])  // payload only
                state = WAIT_SYNC_0
```

---

## 9. Revision History

| Version | Date       | Changes          |
|---------|------------|------------------|
| 1.0     | 2026-03-13 | Initial release  |
| 1.1     | 2026-03-13 | Add three-phase handshake (START/CONFIG_ACK/STOP), session lifecycle, device state machine |
| 1.2     | 2026-03-15 | Document STREAMING re-handshake: START accepted in any state for reconnection |
