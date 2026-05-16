# XIAO PowerBread - リアルタイムモニタリング機能付きブレッドボード電源

[English](README.md) | [Deutsch](README_DE.md) | [Français](README_FR.md) | 日本語

**XIAO PowerBread**は、ブレッドボードプロトタイピング向けの信頼性の高い効率的な電源ソリューションを提供するオープンソースハードウェアプロジェクトです。内蔵センサー、リアルタイムモニタリング、[Seeed Studio XIAOマイクロコントローラー](https://www.seeedstudio.com/xiao-series-page)との互換性を備え、PowerBreadは電子プロジェクトの電源供給と開発をこれまで以上に容易にします。

![XIAO PowerBread](Docs/Images/pic_overview.webp)
> 簡単な設置とコンパクトな設計

![XIAO PowerBread](Docs/Images/pic_directPowerAndMonitoring.webp)
> ブレッドボードプロジェクトへの直接電源供給とモニタリング

## v3.0 の新機能

- **Web コンソール** — USB-Serial 経由で電圧・電流・電力・エネルギーをブラウザにライブ配信（ドライバ不要、インストール不要）。
- **XPB バイナリプロトコル v1.2** — 3 段階ハンドシェイクと自動再接続を備えた決定論的 100 Hz ストリーミングプロトコル。
- **FreeRTOS リファクタリング** — センサーサンプリング、UI レンダリング、ダイヤル入力、シリアルストリーミングを独立タスクに分離し、同期を明示化。
- **LVGL 8.3 + LovyanGFX** — オンボード ST7735 LCD のレンダリングをさらに滑らかに。

> [!IMPORTANT]
> **v3.0 ファームウェアは現時点で XIAO ESP32-S3 と XIAO ESP32-C3 のみをサポートします。**
> RP2040 / RP2350 への移植には、原因未特定の FreeRTOS スケジューリングバグが残っています。XIAO RP2040 または RP2350 をお使いの場合は、v3.x の移植が完了するまで **v2.0.1** ファームウェアをご利用ください。下の[ファームウェアリリース](#ファームウェアリリース)表を参照してください。

## 主な特徴

1. **リアルタイムモニタリング**: マルチメーター不要で重要な電源メトリクスを一目で確認。内蔵ディスプレイにリアルタイムの電圧、電流、電力データを表示。
2. **大電流出力**: 最大 1.5A の 3.3V 電力を供給し、ほとんどのブレッドボードプロジェクトに最適。
3. **内蔵 LCD ディスプレイ**: 統合された LCD に重要な電源データが明確に表示され、リアルタイムのフィードバックを確認可能。
4. **Web コンソール (v3.0)**: USB-Serial 経由でセンサーデータをブラウザベースのダッシュボードにストリーミング。追加のソフトウェアインストール不要。
5. **プラグアンドプレイ設計**: 標準的なブレッドボードと互換性があり、追加設定なしで接続して電源を入れるだけですぐに使用可能。
6. **オープンソースと Seeed Studio XIAO 互換性**: 大半の XIAO 開発ボードと互換性があり、USB-シリアル通信や PWM 制御などの将来の拡張に対応したオープンソース設計。
7. **デュアルチャンネル電圧・電流センシング**: INA3221 センサーを搭載し、デュアルチャンネルの電圧と電流モニタリングが可能。
8. **コンパクトパワーハウス**: コンパクトな設計で 3.3V と 5V 出力を提供し、電力を犠牲にすることなくブレッドボードスペースを最適化。
9. **複数の UI モード**: データモニタリング、ラインチャート、統計モードの切り替えが可能。

![function](Docs/Images/pic_functions.webp)
> 複数の UI 機能

![switchCH](Docs/Images/pic_switchCH.webp)
> チャートモードと統計モードでチャンネルを切り替えるにはダイヤルを長押し

## Web コンソール

**XPB Web コンソール**は、XIAO PowerBread を本格的な電力解析ワークステーションに変える、コンパニオンブラウザアプリです。Chromium 系ブラウザにボードをつなぎ、**Connect** をクリックすれば、ファームウェアが Web Serial API 経由でリアルタイム計測値の配信を開始します — ドライバ不要、インストール不要、バックエンド不要。

![Web コンソール](Docs/Images/web-console/web-console-cover.webp)

### ハイライト

- 両チャンネルの**電圧・電流・電力ライブチャート**（サンプリング 100 Hz）。
- ワンクリックリセット付きの**チャンネル別エネルギー積算**（mAh / mWh）。
- デバイスを自動検出し、プロトコルをネゴシエートして配信を開始する**3 段階ハンドシェイク**。
- 設定が永続化される**ダーク / ライトテーマ**。
- 生のプロトコルフレームを表示できる**デバッグコンソール**。

### スクリーンショット

<table>
  <tr>
    <td align="center">
      <img src="Docs/Images/web-console/light-mode.webp" width="420"><br>
      <b>ライトモード</b>
    </td>
    <td align="center">
      <img src="Docs/Images/web-console/dark-mode.webp" width="420"><br>
      <b>ダークモード</b>
    </td>
  </tr>
</table>

### ブラウザ要件

Web コンソールは **Web Serial API** に依存しており、Chromium 系ブラウザでサポートされます:

- Chrome 89+
- Edge 89+
- Opera 76+

Firefox と Safari は現時点では Web Serial API に対応していません。

### ソースコード

Web コンソールは別リポジトリで開発されています — ソース、ビルド手順、ホスト URL は [XIAO-PowerConsole-Web](https://github.com/nicho810/XIAO-PowerConsole-Web) を参照してください。

## ハードウェア仕様

- **入力電圧**: Seeed Studio XIAO を介した USB-C 電源供給
- **出力電圧**: 5V と 3.3V 出力を提供、3.3V レールで最大 1.5A の電流
- **電流センシング**: 内蔵 INA3221 センサーで電圧、電流、電力をリアルタイム表示
- **マイクロコントローラー**: サポートされている Seeed Studio XIAO モジュールのいずれか — センサーデータ取得、ディスプレイ表示、シリアル配信を処理
- **ディスプレイ**: 0.96 インチ ST7735 LCD によるリアルタイム電源モニタリング
- **基板寸法**: 標準的なブレッドボードとシームレスに統合できる設計で、スペース使用を最小限に抑制

![power path](Docs/Images/pic_outputSwitch.webp)

![XIAO PowerBread](Docs/Images/pic_hardwareSpec.webp)

## はじめ方

### ハードウェア

- **XIAO ボード** — 下のサポートされているモジュールから 1 つを選択。
- **XIAO PowerBread ボード** — このリポジトリからクローン/リミックスするか、[Seeed Studio](https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html) や [Tindie](https://www.tindie.com/products/35842/) で組立済みハードウェアを購入。
- **ブレッドボード** — 標準的なブレッドボードに対応。
- **電源** — 標準的な USB-C 電源を使用。

#### サポートする XIAO ボード & ファームウェア

| XIAO ボード | v3.x | v2.0.1 | v1.1.4 | 推奨ファームウェア |
|---|---|---|---|---|
| [XIAO ESP32-S3](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html) | ✅ | ✅ | ✅ | **v3.0** |
| [XIAO ESP32-C3](https://www.seeedstudio.com/Seeed-XIAO-ESP32C3-p-5431.html) | ✅ | ✅ | ✅ | **v3.0** |
| [XIAO RP2040](https://www.seeedstudio.com/XIAO-RP2040-v1-0-p-5026.html) | ❌ (WIP) | ✅ | ✅ | **v2.0.1** |
| [XIAO RP2350](https://www.seeedstudio.com/Seeed-XIAO-RP2350-p-5944.html) | ❌ (WIP) | ✅ | ✅ | **v2.0.1** |
| [XIAO ESP32-C6](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C6-p-5884.html) | ❌ (WIP) | — | ✅ | **v1.1.4** |

> RP2040、RP2350、ESP32-C6 への v3.x 対応は作業中です。RP 系ボードには現在、原因未特定の FreeRTOS スケジューリング問題が残っているため、v3.x の移植完了までは v2.0.1 をお使いください。

### ハードウェアの入手

<div align="center">
  <table>
    <tr>
      <td align="center">
        <a href="https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html" target="_blank">
          <img src="Docs/Images/seeed-logo.webp" width="200"><br>
          <b>Seeed Studio</b><br>
          キットと XIAO ボードを入手
        </a>
      </td>
      <td align="center">
        <a href="https://www.tindie.com/products/35842/" target="_blank">
          <img src="Docs/Images/tindie-logo.webp" width="200"><br>
          <b>Tindie</b><br>
          完全組み立て済みハードウェアを入手
        </a>
      </td>
    </tr>
  </table>
</div>

**注意**: Seeed Studio で購入する場合、互換性のある XIAO ボードは別売りですので、必ず一緒に注文してください。

### ソフトウェア

- **ソースからビルド** — `Firmware/XPB-Firmware-PlatformIO/` を PlatformIO で開き、ボードに対応する環境（`seeed_xiao_esp32s3`、`seeed_xiao_esp32c3` など）を選択。
- **ビルド済みファームウェアを書き込む**:
  - **XIAO ESP32-S3 / ESP32-C3 (v3.x)**
    - Web 書き込みツール: <https://powerbread-flasher.ioatlas.com>
    - チュートリアル: [XIAO ESP32 シリーズのファームウェア書き込み](Docs/flash-firmware-for-esp32-series.md)
  - **XIAO RP2040 / RP2350（当面 v2.0.1 を使用）**
    1. [リリースページ](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.1) から `.uf2` ファームウェアをダウンロード。
    2. データ通信対応の 4 ピン USB ケーブルで XIAO を PC に接続。
    3. ブートローダーモードへ: **BOOT (B)** を押したまま **RESET (R)** をタップし、**BOOT** を離します。
    4. `RPI-RP2` という名前の USB ドライブが表示されます。
    5. `.uf2` をドライブにドラッグ — ボードが新ファームウェアで自動的に再起動します。

### Web コンソールの使い方

1. ボードが v3.x ファームウェア（XIAO ESP32-S3 または ESP32-C3）で動作していることを確認。
2. Chrome / Edge / Opera で Web コンソールを開く。
3. **Connect** をクリックし、XIAO に対応するシリアルポートを選択して許可を付与。
4. ハンドシェイク完了と同時に、ファームウェアがテキストデバッグモードからバイナリストリーミングに切り替わり、チャートがすぐに埋まり始めます。

### 計画中のソフトウェア機能
- [x] LCD にリアルタイムの電流センサーデータを表示。
- [x] UI の視野角を調整するためのダイヤルホイール。
- [x] 各チャンネルの電力使用を視覚化するラインチャートモード。
- [x] 各チャンネルの平均（秒、分、全時間）とピーク電流をカウントするモード。
- [x] 設定データを EEPROM に保存。
- [x] データモニタリング用 Web ベース UI（v3.0）。
- [x] ハンドシェイクと再接続を伴う USB-Serial ストリーミングプロトコル（v3.0）。
- [ ] XIAO RP2040 / RP2350 / ESP32-C6 への v3.x 移植。
- [ ] IO0 と IO1 での PWM 出力。
- [ ] IO0 と IO1 からの ADC 読み取り。

### ファームウェアリリース

| バージョン | 安定性 | 機能 | リンク |
|---|---|---|---|
| 0.9.0 | はい | LCD にリアルタイムの電流センサーデータを表示。 | - |
| 1.0.0 | はい | UI の視野角を調整するためのダイヤルホイール。 | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.0.0) |
| 1.1.0 | いいえ | 各チャンネルの電力使用ラインチャート。 | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.0) |
| 1.1.1 | はい | データダッシュボード、ラインチャート、電流統計、設定。 | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.1) |
| 1.1.2 | はい | デバッグ用 LED 点滅機能。 | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.2) |
| 1.1.3 | はい | ラインチャートに固定スケールと自動スケールを追加。 | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.3) |
| 1.1.4 | はい | RP2350、ESP32-C3、ESP32-S3、ESP32-C6 のサポート追加。 | [RP2040, RP2350, ESP32C3, ESP32S3, ESP32C6](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.4) |
| 2.0.0 | はい | コードのリファクタリング + LVGL / LovyanGFX への移行。 | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.0) |
| 2.0.1 | はい 👍 (RP2040 / RP2350) | RP2350、ESP32-C3、ESP32-S3 のサポート追加。 | [RP2040, RP2350, ESP32C3, ESP32S3](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.1) |
| **3.0.0** | **はい 👍 (ESP32-S3 / C3)** | **Web コンソール、XPB バイナリプロトコル v1.2、FreeRTOS リファクタリング。ESP32-S3 / C3 のみ。** | [ESP32S3, ESP32C3](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v3.0.0) |

### ドキュメント
- [システム設定の説明](Docs/sysConfig.md)
  - デフォルトモード設定
  - シャント抵抗設定（20 mΩ、50 mΩ）
  - シリアル出力設定（人間可読モード、Arduino プロッターモード）
  - ラインチャート設定（更新レート）
- [XPB バイナリプロトコル v1.2](Firmware/XPB-Firmware-PlatformIO/docs/) — ハンドシェイク、フレームレイアウト、STREAMING 再接続セマンティクス。
- [ESP32 シリーズのファームウェア書き込み](Docs/flash-firmware-for-esp32-series.md)

### トラブルシューティング

1. **ディスプレイが応答しない** — 最新のファームウェアを使用しているか確認してください。最新リリースには安定性に関する修正が含まれています。XIAO のリセットボタンを押してリセットを試してください。
2. **ディスプレイが動作しない** — I2C と SPI のピン接続を確認し、XIAO が正しくはんだ付けされていることを確認してください。
3. **電流値が正しくない** — シャント抵抗値を確認し、`sysConfig` で正しく設定してください（例：20 mΩ は `20`、50 mΩ は `50`）。[システム設定](Docs/sysConfig.md) を参照。
4. **両方の電圧表示が 0V** — 電源スイッチを確認してください。**DOWN** 位置である必要があります（UP = OFF、DOWN = ON）。
5. **Web コンソールが「Web Serial API not supported」と表示する** — Chrome、Edge、Opera を使用してください。Firefox と Safari は Web Serial を実装していません。
6. **Web コンソールは接続できるがデータが届かない** — ファームウェアが v3.x であること、および他のアプリケーション（Arduino Serial Monitor、PlatformIO Monitor）がシリアルポートを占有していないことを確認してください。

### クレジット

| ライブラリ | v1.x | v2.x | v3.x | リンク |
|---|---|---|---|---|
| Arduino-Pico Core | ✅ | ✅ | ✅ | [arduino-pico](https://github.com/earlephilhower/arduino-pico) |
| Arduino-ESP32 Core | ✅ | ✅ | ✅ | [arduino-esp32](https://github.com/espressif/arduino-esp32) |
| INA3221_RT | ✅ | ✅ | ✅ | [INA3221_RT](https://github.com/RobTillaart/INA3221_RT/tree/master) |
| Adafruit GFX | ✅ |  |  | [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library) |
| Adafruit ST7735 | ✅ |  |  | [Adafruit-ST7735](https://github.com/adafruit/Adafruit-ST7735-Library) |
| Adafruit SleepyDog | ✅ |  |  | [Adafruit_SleepyDog](https://github.com/adafruit/Adafruit_SleepyDog) |
| LovyanGFX |  | ✅ | ✅ | [LovyanGFX](https://github.com/lovyan03/LovyanGFX) |
| LVGL |  | ✅ | ✅ | [LVGL](https://github.com/lvgl/lvgl) |
| FreeRTOS |  |  | ✅ | Arduino-ESP32 / Arduino-Pico に同梱 |
| Chart.js (Web コンソール) |  |  | ✅ | [Chart.js](https://www.chartjs.org/) |

| ツール | リンク |
|---|---|
| ESP32 シリーズ用 MergeBin ツール | [DavidSchinazi、DunkelRatte に感謝](https://github.com/platformio/platform-espressif32/issues/1078#issuecomment-2219671743) |
| Web コンソール | [XIAO-PowerConsole-Web](https://github.com/nicho810/XIAO-PowerConsole-Web) |

## 貢献

XIAO PowerBread プロジェクトの改善へのご協力をお待ちしています！プルリクエストの提出、新機能の提案、バグ報告など、イシュートラッカーをご利用ください。**v3.x の RP2040 / RP2350（FreeRTOS）移植への協力は特に歓迎します** — `rp-rtos` タグの付いた未解決の Issue を参照してください。

## ライセンス

このプロジェクトは MIT ライセンスの下で公開されています。詳細は [LICENSE](./LICENSE) ファイルをご覧ください。
