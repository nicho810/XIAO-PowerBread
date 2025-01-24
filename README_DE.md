# XIAO PowerBread - Eine Steckbrett-Stromversorgung mit Echtzeit-Überwachung

[English](README.md) | Deutsch | [Français](README_FR.md) | [日本語](README_JP.md)

**XIAO PowerBread** ist ein Open-Source-Hardware-Projekt, das entwickelt wurde, um eine zuverlässige und effiziente Stromversorgungslösung für Steckbrett-Prototyping zu bieten. Mit eingebauten Sensoren, Echtzeit-Überwachung und Kompatibilität mit [Seeed Studio XIAO Mikrocontrollern](https://www.seeedstudio.com/xiao-series-page) macht PowerBread die Stromversorgung und Entwicklung von Elektronikprojekten einfacher als je zuvor.

![XIAO PowerBread](Docs/Images/pic_overview.png)
> Einfache Installation und kompaktes Design

![XIAO PowerBread](Docs/Images/pic_directPowerAndMonitoring.png)
> Direkte Stromversorgung und Überwachung Ihrer Steckbrett-Projekte

## Hauptmerkmale

1. **Echtzeit-Überwachung**: Sehen Sie wichtige Strommetriken auf einen Blick - kein Multimeter erforderlich. Das eingebaute Display zeigt Echtzeit-Spannungs-, Strom- und Leistungsdaten an.
2. **Hohe Stromabgabe**: Liefert bis zu 1,5A bei 3,3V, ideal für die meisten steckbrettbasierten Elektronikprojekte.
3. **Eingebautes LCD-Display**: Bleiben Sie informiert mit Echtzeit-Feedback - kritische Stromdaten werden klar auf dem integrierten LCD angezeigt.
4. **Plug-and-Play-Design**: Kompatibel mit Standard-Steckbrettern, einfach einstecken, einschalten und ohne zusätzliche Einrichtung mit dem Prototyping beginnen.
5. **Open-Source und Seeed Studio XIAO-Kompatibilität**: Kompatibel mit fast allen XIAO-Entwicklungsboards (die Unterstützung für XIAO SAMD21 und XIAO RA4M1 ist noch in Arbeit), bietet dieses Open-Source-Design Flexibilität für zukünftige Erweiterungen wie USB-Seriell-Kommunikation und PWM-Steuerung.
6. **Zweikanal-Spannungs- und Strommessung**: Ausgestattet mit dem INA3221-Sensor für Zweikanal-Spannungs- und Stromüberwachung - wissen Sie, was mit jedem Aspekt Ihrer Stromversorgung passiert.
7. **Kompaktes Kraftpaket**: Dieses kompakte Design liefert sowohl 3,3V als auch 5V Ausgänge und optimiert Ihren Steckbrettplatz ohne Kompromisse bei der Leistung.
8. **Mehrere UI-Funktionen**: Die Benutzeroberfläche kann zwischen Datenüberwachung, Liniendiagramm und Statistikmodus umgeschaltet werden.

![Funktion](Docs/Images/pic_functions.png)
> Mehrere UI-Funktionen

![Kanal wechseln](Docs/Images/pic_switchCH.png)
> Langes Drücken des Drehreglers zum Umschalten der Kanäle im Diagramm- und Statistikmodus

## Hardware-Spezifikationen

- **Eingangsspannung**: Stromversorgung über USB-C durch das Seeed Studio XIAO.
- **Ausgangsspannungen**: Liefert 5V und 3,3V Ausgänge, mit einem maximalen Strom von 1,5A für die 3,3V-Schiene.
- **Strommessung**: Der eingebaute INA3221-Sensor misst Spannung, Strom und Leistung für die Echtzeit-Anzeige.
- **Mikrocontroller**: Angetrieben vom RP2040, der Sensordaten verarbeitet, die Anzeige steuert und zukünftige Funktionen wie USB-Seriell-Kommunikation und PWM-Generierung ermöglicht.
- **Display**: LCD-Bildschirm für Echtzeit-Stromüberwachung.
- **PCB-Abmessungen**: Entwickelt für nahtlose Integration mit Standard-Steckbrettern bei minimaler Platznutzung.

![Stromweg](Docs/Images/pic_outputSwitch.png)

![XIAO PowerBread](Docs/Images/pic_hardwareSpec.png)

## Erste Schritte

### Hardware

- **XIAO-Board**: Das [XIAO RP2040](https://www.seeedstudio.com/XIAO-RP2040-v1-0-p-5026.html) wird empfohlen, Sie können auch verwenden: [XIAO RP2350](https://www.seeedstudio.com/Seeed-XIAO-RP2350-p-5944.html), [XIAO ESP32-C3](https://www.seeedstudio.com/Seeed-XIAO-ESP32C3-p-5431.html), [XIAO ESP32-S3](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html), [XIAO ESP32-C6](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C6-p-5884.html). (Stellen Sie sicher, dass Sie die mit Ihrem Board-Typ kompatible Firmware flashen)
- **XIAO PowerBread-Board**: Klonen oder remixen Sie das Design aus diesem Repository. Sie können die Hardware auch von [Seeed Studio](https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html) oder [Tindie](https://www.tindie.com/products/35842/) erhalten.
- **Steckbrett**: Passt auf Standard-Steckbretter für einfaches Prototyping.
- **Stromversorgung**: Verwenden Sie eine Standard-USB-Stromquelle.

### Hardware erhalten

<div align="center">
  <table>
    <tr>
      <td align="center">
        <a href="https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html" target="_blank">
          <img src="Docs/Images/seeed-logo.png" width="200"><br>
          <b>Seeed Studio</b><br>
          Kit und XIAO-Board erhalten
        </a>
      </td>
      <td align="center">
        <a href="https://www.tindie.com/products/35842/" target="_blank">
          <img src="Docs/Images/tindie-logo.png" width="200"><br>
          <b>Tindie</b><br>
          Vollständig montierte Hardware erhalten
        </a>
      </td>
    </tr>
  </table>
</div>

**Hinweis**: Beim Kauf bei Seeed Studio denken Sie daran, auch ein kompatibles XIAO-Board zu bestellen, da es erforderlich ist, aber separat verkauft wird.

### Software

- **Projekt remixen**: Modifizieren oder remixen Sie das Projekt mit dem bereitgestellten Arduino-Quellcode.
- **Direkte Verwendung der kompilierten Firmware**: 
  - Für XIAO ESP32-Serien
    - Webbasiertes Flash-Tool: https://powerbread-flasher.ioatlas.com
    - Anleitung: [Firmware-Flash für XIAO - ESP32-Serien](Docs/flash-firmware-for-esp32-series.md)
  - Für XIAO RP2040/RP2350
    - Flash-Tool: Verwenden Sie die UF2-Methode zum Hochladen der Firmware:
      1. Laden Sie die Firmware-Datei (*.uf2) von der [Release-Seite](https://github.com/nicho810/XIAO-PowerBread/releases) herunter
      2. Verbinden Sie XIAO über USB-Kabel mit Ihrem Computer (muss ein 4-poliges Datenkabel sein)
      3. Bootloader-Modus aktivieren:
         - Drücken und halten Sie die BOOT-Taste (B)
         - Während Sie BOOT halten, drücken und lassen Sie die RESET-Taste (R) los
         - Lassen Sie die BOOT-Taste los
      4. Ein neues USB-Laufwerk namens "RPI-RP2" erscheint auf Ihrem Computer
      5. Ziehen Sie die .uf2-Firmware-Datei auf das RPI-RP2-Laufwerk
      6. Das XIAO wird automatisch neu gestartet und die neue Firmware ausführen

### Geplante Software-Funktionen
[Rest des Inhalts bleibt gleich...]

### Firmware-Versionen

| Version | Stabil | Tags | Hinzugefügte Funktionen | Link |
|---------|---------|---------|---------|---------|
| 0.9.0   | Ja | - | Echtzeit-Stromsensordaten auf dem LCD angezeigt. | - |
| 1.0.0   | Ja | - | Drehregler zur Anpassung der Benutzeroberfläche für verschiedene Betrachtungswinkel. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.0.0) |
| 1.1.0   | Nein | - | Liniendiagramm-Modus zur Visualisierung des Stromverbrauchs für jeden Kanal. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.0) |
| 1.1.1   | Ja | - | Unterstützung für Daten-Dashboard, Liniendiagramm, Strom-Statistik, Einstellungen | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.1) |
| 1.1.2   | Ja | - | Führt LED-Blink-Funktionalität für Debugging-Zwecke ein | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.2) |
| 1.1.3   | Ja | - | Fügt Unterstützung für feste Skala und Auto-Skala für Liniendiagramme hinzu | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.3) |
| 1.1.4   | Ja👍 | - | Unterstützung für XIAO RP2350, XIAO ESP32-C3, XIAO ESP32-S3, XIAO ESP32-C6 hinzugefügt | [RP2040, RP2350, ESP32C3, ESP32S3, ESP32C6](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.4) |
| 2.0.0   | Ja | - | Code-Refaktorierung und Wechsel zu LVGL(LovyanGFX) für UI-Rendering zur Verbesserung der Leistung | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.0) |
| 2.0.1   | Ja👍 | - | Unterstützung für XIAO RP2350, XIAO ESP32-C3, XIAO ESP32-S3 hinzugefügt | [RP2040, RP2350, ESP32C3, ESP32S3](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.1) |

### Credits

| Bibliothek | v1.x | v2.x | Link |
|---------|------|------|------|
| Arduino-Pico Core | ✅ | ✅ | [Arduino-Pico Core (4.x.x)](https://github.com/earlephilhower/arduino-pico) |
| Arduino-ESP32 Core | ✅ | ✅ | [Arduino-ESP32 Core (3.x.x)](https://github.com/espressif/arduino-esp32) |
| INA3221_RT Library | ✅ | ✅ | [INA3221_RT Library](https://github.com/RobTillaart/INA3221_RT/tree/master) |
| Adafruit GFX Library | ✅ |  | [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) |
| Adafruit ST7735 Library | ✅ |  | [Adafruit ST7735 Library](https://github.com/adafruit/Adafruit-ST7735-Library) |
| Adafruit SleepyDog | ✅ |  | [Adafruit SleepyDog](https://github.com/adafruit/Adafruit_SleepyDog) |
| LovyanGFX |  | ✅ | [LovyanGFX](https://github.com/lovyan03/LovyanGFX) |
| LVGL |  | ✅ | [LVGL](https://github.com/lvgl/lvgl) |

| Tools | Link |
|---------|------|
| MergeBin tool for esp32 series | [Thanks to DavidSchinazi, DunkelRatte](https://github.com/platformio/platform-espressif32/issues/1078#issuecomment-2219671743) |

## Mitwirken

Beiträge zur Verbesserung des XIAO PowerBread-Projekts sind willkommen! Ob Sie einen Pull-Request einreichen, neue Funktionen vorschlagen oder einen Fehler melden möchten, nutzen Sie gerne den Issue-Tracker.

## Lizenz

Dieses Projekt ist unter der MIT-Lizenz lizenziert. Siehe die [LICENSE](./LICENSE)-Datei für Details.