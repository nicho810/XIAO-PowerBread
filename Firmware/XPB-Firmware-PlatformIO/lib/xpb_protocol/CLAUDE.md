# xpb_protocol/
> L2 | 父级: lib/CLAUDE.md

XPB Binary Protocol v1.2 引擎，平台无关，零 I/O 依赖。

## 成员清单

xpb_protocol.h: 协议公共接口 — 常量 (XPB_SYNC/TYPE/VER)、XpbParsedFrame/XpbSessionAction 结构体、xpb_crc8_maxim/xpb_build_* 帧构建函数、XpbFrameParser 逐字节解析器、XpbSession 三态会话状态机
xpb_protocol.cpp: 协议实现 — CRC-8/MAXIM 算法、帧序列化 (CONFIG 9B / REALTIME 20B payload)、逐字节状态机解析、会话状态机 (IDLE→WAIT_ACK→STREAMING)

## 设计原则

零 I/O 依赖: 所有函数接受/返回字节缓冲区，调用者负责 Serial.write/read
XpbSessionAction 返回值模式: on_frame()/on_tick() 返回动作结构体 (tx_data + send_data + reset_parser)，调用者执行

## 公共接口

```cpp
// CRC + 帧构建
xpb_crc8_maxim(data, len) → uint8_t
xpb_build_frame(buf, type, payload, len) → size_t
xpb_build_config_frame(buf, shuntR_a, shuntR_b) → size_t   // 14 bytes
xpb_build_realtime_frame(buf, busV_a, shuntV_a, busV_b, shuntV_b, ts) → size_t  // 25 bytes

// 解析
XpbFrameParser::feed(byte, out) → bool   // 一帧完整且 CRC 通过
XpbFrameParser::reset() → void

// 会话
XpbSession::set_shunt_config(shuntR_a, shuntR_b)
XpbSession::on_frame(frame, now_ms) → XpbSessionAction
XpbSession::on_tick(now_ms) → XpbSessionAction
XpbSession::state() → State {IDLE, WAIT_ACK, STREAMING}
```

[PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
