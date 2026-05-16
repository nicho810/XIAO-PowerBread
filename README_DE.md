# XIAO PowerBread - Eine Steckbrett-Stromversorgung mit Echtzeit-Überwachung

[English](README.md) | Deutsch | [Français](README_FR.md) | [日本語](README_JP.md)

**XIAO PowerBread** ist ein Open-Source-Hardware-Projekt, das entwickelt wurde, um eine zuverlässige und effiziente Stromversorgungslösung für Steckbrett-Prototyping zu bieten. Mit eingebauten Sensoren, Echtzeit-Überwachung und Kompatibilität mit [Seeed Studio XIAO Mikrocontrollern](https://www.seeedstudio.com/xiao-series-page) macht PowerBread die Stromversorgung und Entwicklung von Elektronikprojekten einfacher als je zuvor.

![XIAO PowerBread](Docs/Images/pic_overview.webp)
> Einfache Installation und kompaktes Design

![XIAO PowerBread](Docs/Images/pic_directPowerAndMonitoring.webp)
> Direkte Stromversorgung und Überwachung Ihrer Steckbrett-Projekte

## Was ist neu in v3.0

- **Web-Konsole** — Streamt Spannung, Strom, Leistung und Energie live über USB-Serial in den Browser (kein Treiber, keine Installation).
- **XPB Binärprotokoll v1.2** — Deterministisches 100-Hz-Streaming-Protokoll mit dreistufigem Handshake und automatischer Wiederverbindung.
- **FreeRTOS-Refactoring** — Sensorabtastung, UI-Rendering, Drehregler-Eingabe und serielles Streaming laufen als unabhängige Tasks mit expliziter Synchronisation.
- **LVGL 8.3 + LovyanGFX** — Flüssigeres Rendering auf dem integrierten ST7735-LCD.

> [!IMPORTANT]
> **Die v3.0-Firmware unterstützt XIAO ESP32-S3, ESP32-C3 und ESP32-C6.**
> Die Portierungen für RP2040 / RP2350 weisen noch einen FreeRTOS-Scheduling-Bug auf, dessen Ursache noch nicht gefunden wurde. Wenn Sie ein XIAO RP2040 oder RP2350 verwenden, bleiben Sie bitte bei der **v2.0.1**-Firmware, bis der v3.x-Port abgeschlossen ist. Siehe die Tabelle [Firmware-Versionen](#firmware-versionen) weiter unten.

## Hauptmerkmale

1. **Echtzeit-Überwachung**: Sehen Sie wichtige Strommetriken auf einen Blick – kein Multimeter erforderlich. Das eingebaute Display zeigt Echtzeit-Spannungs-, Strom- und Leistungsdaten an.
2. **Hohe Stromabgabe**: Liefert bis zu 1,5 A bei 3,3 V, ideal für die meisten steckbrettbasierten Elektronikprojekte.
3. **Eingebautes LCD-Display**: Bleiben Sie informiert mit Echtzeit-Feedback – kritische Stromdaten werden klar auf dem integrierten LCD angezeigt.
4. **Web-Konsole (v3.0)**: Streamt Sensordaten an ein browserbasiertes Dashboard über USB-Serial – keine zusätzliche Software erforderlich.
5. **Plug-and-Play-Design**: Kompatibel mit Standard-Steckbrettern, einfach einstecken, einschalten und ohne zusätzliche Einrichtung mit dem Prototyping beginnen.
6. **Open-Source und Seeed Studio XIAO-Kompatibilität**: Kompatibel mit fast allen XIAO-Entwicklungsboards. Dieses Open-Source-Design bietet Flexibilität für zukünftige Erweiterungen wie USB-Seriell-Kommunikation und PWM-Steuerung.
7. **Zweikanal-Spannungs- und Strommessung**: Ausgestattet mit dem INA3221-Sensor für Zweikanal-Spannungs- und Stromüberwachung – wissen Sie, was mit jedem Aspekt Ihrer Stromversorgung passiert.
8. **Kompaktes Kraftpaket**: Dieses kompakte Design liefert sowohl 3,3 V als auch 5 V Ausgänge und optimiert Ihren Steckbrettplatz ohne Kompromisse bei der Leistung.
9. **Mehrere UI-Modi**: Die Benutzeroberfläche kann zwischen Datenüberwachung, Liniendiagramm und Statistikmodus umgeschaltet werden.

![Funktion](Docs/Images/pic_functions.webp)
> Mehrere UI-Funktionen

![Kanal wechseln](Docs/Images/pic_switchCH.webp)
> Langes Drücken des Drehreglers zum Umschalten der Kanäle im Diagramm- und Statistikmodus

## Web-Konsole

Die **XPB Web-Konsole** ist eine begleitende Browser-App, die Ihr XIAO PowerBread in eine vollständige Strom-Analyse-Workstation verwandelt. Schließen Sie das Board an einen beliebigen Chromium-basierten Browser an, klicken Sie auf **Connect**, und die Firmware streamt live Messwerte über die Web-Serial-API – keine Treiber, keine Installationen, kein Backend.

![Web-Konsole](Docs/Images/web-console/web-console-cover.webp)

### Höhepunkte

- **Live-Diagramme** für Spannung, Strom und Leistung auf beiden Kanälen, abgetastet mit 100 Hz.
- **Energieintegration pro Kanal** (mAh / mWh) mit Reset auf Knopfdruck.
- **Dreistufiger Handshake**, der das Gerät automatisch erkennt, das Protokoll aushandelt und das Streaming startet.
- **Hell-/Dunkel-Theme** mit persistenten Einstellungen.
- **Debug-Konsole**, die die rohen Protokollrahmen zur Fehlersuche anzeigt.

### Screenshots

<table>
  <tr>
    <td align="center">
      <img src="Docs/Images/web-console/light-mode.webp" width="420"><br>
      <b>Heller Modus</b>
    </td>
    <td align="center">
      <img src="Docs/Images/web-console/dark-mode.webp" width="420"><br>
      <b>Dunkler Modus</b>
    </td>
  </tr>
</table>

### Browser-Anforderungen

Die Web-Konsole setzt auf die **Web-Serial-API**, die in Chromium-basierten Browsern unterstützt wird:

- Chrome 89+
- Edge 89+
- Opera 76+

Firefox und Safari unterstützen die Web-Serial-API derzeit nicht.

### Zugriff

Ein praktisches Begleit-Tool für Ihr PowerBread — direkt im Browser öffnen: **<https://xiao-power-console-web.ioatlas.com>**

## Hardware-Spezifikationen

- **Eingangsspannung**: Stromversorgung über USB-C durch das Seeed Studio XIAO.
- **Ausgangsspannungen**: Liefert 5 V und 3,3 V Ausgänge, mit einem maximalen Strom von 1,5 A für die 3,3-V-Schiene.
- **Strommessung**: Der eingebaute INA3221-Sensor misst Spannung, Strom und Leistung für die Echtzeit-Anzeige.
- **Mikrocontroller**: Eines der unterstützten Seeed Studio XIAO-Module – verarbeitet Sensordaten, steuert die Anzeige und das serielle Streaming.
- **Display**: 0,96" ST7735-LCD für Echtzeit-Stromüberwachung.
- **PCB-Abmessungen**: Entwickelt für nahtlose Integration mit Standard-Steckbrettern bei minimaler Platznutzung.

![Stromweg](Docs/Images/pic_outputSwitch.webp)

![XIAO PowerBread](Docs/Images/pic_hardwareSpec.webp)

## Erste Schritte

### Hardware

- **XIAO-Board** – wählen Sie eines der unten unterstützten Module.
- **XIAO PowerBread-Board** – Klonen oder remixen Sie das Design aus diesem Repository, oder kaufen Sie die fertig montierte Hardware bei [Seeed Studio](https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html) oder [Tindie](https://www.tindie.com/products/35842/).
- **Steckbrett** – passt auf jedes Standard-Steckbrett.
- **Stromversorgung** – jede beliebige USB-C-Stromquelle.

#### Unterstützte XIAO-Boards & Firmware

| XIAO-Board | v3.x | v2.0.1 | v1.1.4 | Empfohlene Firmware |
|---|---|---|---|---|
| [XIAO ESP32-S3](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html) | ✅ | ✅ | ✅ | **v3.0** |
| [XIAO ESP32-C3](https://www.seeedstudio.com/Seeed-XIAO-ESP32C3-p-5431.html) | ✅ | ✅ | ✅ | **v3.0** |
| [XIAO RP2040](https://www.seeedstudio.com/XIAO-RP2040-v1-0-p-5026.html) | ❌ (WIP) | ✅ | ✅ | **v2.0.1** |
| [XIAO RP2350](https://www.seeedstudio.com/Seeed-XIAO-RP2350-p-5944.html) | ❌ (WIP) | ✅ | ✅ | **v2.0.1** |
| [XIAO ESP32-C6](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C6-p-5884.html) | ✅ | — | ✅ | **v3.0** |

> Die v3.x-Unterstützung für RP2040 und RP2350 ist weiterhin in Arbeit. Die RP-Serie weist derzeit ein FreeRTOS-Scheduling-Problem auf, dessen Ursache noch nicht gefunden wurde; bleiben Sie bitte bei v2.0.1, bis der v3.x-Port abgeschlossen ist.

### Hardware erhalten

<div align="center">
  <table>
    <tr>
      <td align="center">
        <a href="https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html" target="_blank">
          <img src="Docs/Images/seeed-logo.webp" width="200"><br>
          <b>Seeed Studio</b><br>
          Kit und XIAO-Board erhalten
        </a>
      </td>
      <td align="center">
        <a href="https://www.tindie.com/products/35842/" target="_blank">
          <img src="Docs/Images/tindie-logo.webp" width="200"><br>
          <b>Tindie</b><br>
          Vollständig montierte Hardware erhalten
        </a>
      </td>
    </tr>
  </table>
</div>

**Hinweis**: Beim Kauf bei Seeed Studio denken Sie daran, auch ein kompatibles XIAO-Board zu bestellen, da es erforderlich, aber separat erhältlich ist.

### Software

- **Aus dem Quellcode bauen** – öffnen Sie `Firmware/XPB-Firmware-PlatformIO/` in PlatformIO und wählen Sie die zu Ihrem Board passende Umgebung aus (`seeed_xiao_esp32s3`, `seeed_xiao_esp32c3`, `seeed_xiao_esp32c6` usw.).
- **Vorgefertigte Firmware flashen**:
  - **XIAO ESP32-S3 / ESP32-C3 / ESP32-C6 (v3.x)**
    - Web-Flash-Tool: <https://powerbread-flasher.ioatlas.com>
    - Anleitung: [Firmware-Flash für die XIAO ESP32-Serie](Docs/flash-firmware-for-esp32-series.md)
  - **XIAO RP2040 / RP2350 (bis auf Weiteres v2.0.1)**
    1. Laden Sie die `.uf2`-Firmware von der [Release-Seite](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.1) herunter.
    2. Verbinden Sie das XIAO über ein 4-poliges Daten-USB-Kabel mit Ihrem Computer.
    3. Aktivieren Sie den Bootloader-Modus: Halten Sie **BOOT (B)**, drücken Sie kurz **RESET (R)**, dann lassen Sie **BOOT** los.
    4. Es erscheint ein USB-Laufwerk namens `RPI-RP2`.
    5. Ziehen Sie die `.uf2` auf das Laufwerk – das Board startet mit der neuen Firmware neu.

### Web-Konsole verwenden

1. Stellen Sie sicher, dass auf Ihrem Board die v3.x-Firmware läuft (XIAO ESP32-S3, ESP32-C3 oder ESP32-C6).
2. Öffnen Sie die Web-Konsole unter <https://xiao-power-console-web.ioatlas.com> in Chrome / Edge / Opera.
3. Klicken Sie auf **Connect**, wählen Sie den seriellen Port Ihres XIAO und gewähren Sie die Berechtigung.
4. Die Firmware wechselt vom Text-Debug-Modus in den binären Streaming-Modus, sobald der Handshake abgeschlossen ist – die Diagramme werden sofort gefüllt.

### Geplante Software-Funktionen
- [x] Echtzeit-Stromsensordaten auf dem LCD angezeigt.
- [x] Drehregler zur Anpassung der Benutzeroberfläche für verschiedene Betrachtungswinkel.
- [x] Liniendiagramm-Modus zur Visualisierung des Stromverbrauchs für jeden Kanal.
- [x] Statistik-Modus für Durchschnitt (Sekunde, Minute, gesamt) und Spitzenverbrauch pro Kanal.
- [x] Einstellungen im EEPROM speichern.
- [x] Webbasierte UI für die Datenüberwachung (v3.0).
- [x] USB-Serial-Streaming-Protokoll mit Handshake und Wiederverbindung (v3.0).
- [ ] v3.x-Port für XIAO RP2040 / RP2350.
- [ ] PWM-Ausgang auf IO0 und IO1.
- [ ] ADC-Eingang von IO0 und IO1.

### Firmware-Versionen

| Version | Stabil | Funktionen | Link |
|---|---|---|---|
| 0.9.0 | Ja | Echtzeit-Stromsensordaten auf dem LCD. | - |
| 1.0.0 | Ja | Drehregler zur Anpassung der Benutzeroberfläche. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.0.0) |
| 1.1.0 | Nein | Liniendiagramm-Modus für Stromverbrauch. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.0) |
| 1.1.1 | Ja | Daten-Dashboard, Liniendiagramm, Strom-Statistik, Einstellungen. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.1) |
| 1.1.2 | Ja | LED-Blink für Debugging. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.2) |
| 1.1.3 | Ja | Feste und automatische Skala für Liniendiagramme. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.3) |
| 1.1.4 | Ja | Unterstützung für RP2350, ESP32-C3, ESP32-S3, ESP32-C6 hinzugefügt. | [RP2040, RP2350, ESP32C3, ESP32S3, ESP32C6](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.4) |
| 2.0.0 | Ja | Refactoring + Wechsel zu LVGL / LovyanGFX. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.0) |
| 2.0.1 | Ja 👍 (RP2040 / RP2350) | Unterstützung für RP2350, ESP32-C3, ESP32-S3 hinzugefügt. | [RP2040, RP2350, ESP32C3, ESP32S3](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.1) |
| **3.0.0** | **Ja 👍 (ESP32-S3 / C3 / C6)** | **Web-Konsole, XPB Binärprotokoll v1.2, FreeRTOS-Refactoring. ESP32-S3 / C3 / C6.** | [ESP32S3, ESP32C3, ESP32C6](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v3.0.0) |

### Dokumentation
- [Erklärung der Systemkonfiguration](Docs/sysConfig.md)
  - Standardmoduseinstellung
  - Shunt-Widerstands-Einstellung (20 mΩ, 50 mΩ)
  - Einstellungen der seriellen Ausgabe (Mensch-lesbar, Arduino-Plotter)
  - Liniendiagramm-Einstellungen (Aktualisierungsrate)
- [XPB Binärprotokoll v1.2](Firmware/XPB-Firmware-PlatformIO/docs/) – Handshake, Frame-Layout, STREAMING-Wiederverbindungssemantik.
- [Firmware-Flash für die ESP32-Serie](Docs/flash-firmware-for-esp32-series.md)

### Fehlersuche

1. **Das Display reagiert nicht** – stellen Sie sicher, dass Sie die neueste Firmware verwenden; die neuesten Releases enthalten Stabilitätskorrekturen. Versuchen Sie, das XIAO über den Reset-Knopf neu zu starten.
2. **Das Display funktioniert nicht** – prüfen Sie die I2C- und SPI-Pinverbindungen und vergewissern Sie sich, dass das XIAO sauber verlötet ist.
3. **Der Stromwert stimmt nicht** – prüfen Sie den Shunt-Widerstandswert und setzen Sie ihn korrekt in `sysConfig` (z. B. `20` für 20 mΩ, `50` für 50 mΩ). Siehe [Systemkonfiguration](Docs/sysConfig.md).
4. **Beide Spannungsanzeigen zeigen 0 V** – prüfen Sie den Power-Schalter; er muss in der **UNTEREN** Position stehen (OBEN = AUS, UNTEN = EIN).
5. **Web-Konsole meldet „Web Serial API not supported"** – verwenden Sie Chrome, Edge oder Opera. Firefox und Safari implementieren Web Serial nicht.
6. **Web-Konsole verbindet sich, aber es kommen keine Daten an** – stellen Sie sicher, dass die Firmware v3.x ist und kein anderes Programm (Arduino Serial Monitor, PlatformIO Monitor) den seriellen Port belegt.

### Credits

| Bibliothek | v1.x | v2.x | v3.x | Link |
|---|---|---|---|---|
| Arduino-Pico Core | ✅ | ✅ | ✅ | [arduino-pico](https://github.com/earlephilhower/arduino-pico) |
| Arduino-ESP32 Core | ✅ | ✅ | ✅ | [arduino-esp32](https://github.com/espressif/arduino-esp32) |
| INA3221_RT | ✅ | ✅ | ✅ | [INA3221_RT](https://github.com/RobTillaart/INA3221_RT/tree/master) |
| Adafruit GFX | ✅ |  |  | [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library) |
| Adafruit ST7735 | ✅ |  |  | [Adafruit-ST7735](https://github.com/adafruit/Adafruit-ST7735-Library) |
| Adafruit SleepyDog | ✅ |  |  | [Adafruit_SleepyDog](https://github.com/adafruit/Adafruit_SleepyDog) |
| LovyanGFX |  | ✅ | ✅ | [LovyanGFX](https://github.com/lovyan03/LovyanGFX) |
| LVGL |  | ✅ | ✅ | [LVGL](https://github.com/lvgl/lvgl) |
| FreeRTOS |  |  | ✅ | mitgeliefert mit Arduino-ESP32 / Arduino-Pico |
| Chart.js (Web-Konsole) |  |  | ✅ | [Chart.js](https://www.chartjs.org/) |

| Werkzeug | Link |
|---|---|
| MergeBin-Tool für die ESP32-Serie | [Dank an DavidSchinazi, DunkelRatte](https://github.com/platformio/platform-espressif32/issues/1078#issuecomment-2219671743) |
| XPB Web-Konsole | <https://xiao-power-console-web.ioatlas.com> |

## Mitwirken

Beiträge zur Verbesserung des XIAO PowerBread-Projekts sind willkommen! Ob Pull-Request, neuer Funktionsvorschlag oder Bug-Report – nutzen Sie gerne den Issue-Tracker. **Hilfe beim v3.x-Port auf RP2040 / RP2350 (FreeRTOS) ist besonders willkommen** – siehe die offenen Issues mit dem Tag `rp-rtos`.

## Lizenz

Dieses Projekt ist unter der MIT-Lizenz lizenziert. Siehe die [LICENSE](./LICENSE)-Datei für Details.
