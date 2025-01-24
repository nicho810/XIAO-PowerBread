# XIAO PowerBread - リアルタイムモニタリング機能付きブレッドボード電源

[English](README.md) | [Deutsch](README_DE.md) | [Français](README_FR.md) | 日本語

**XIAO PowerBread**は、ブレッドボードプロトタイピング向けの信頼性の高い効率的な電源ソリューションを提供するオープンソースハードウェアプロジェクトです。内蔵センサー、リアルタイムモニタリング、[Seeed Studio XIAOマイクロコントローラー](https://www.seeedstudio.com/xiao-series-page)との互換性を備え、PowerBreadは電子プロジェクトの電源供給と開発をこれまで以上に容易にします。

![XIAO PowerBread](Docs/Images/pic_overview.png)
> 簡単な設置とコンパクトな設計

![XIAO PowerBread](Docs/Images/pic_directPowerAndMonitoring.png)
> ブレッドボードプロジェクトへの直接電源供給とモニタリング

## 主な特徴

1. **リアルタイムモニタリング**: マルチメーター不要で重要な電源メトリクスを一目で確認。内蔵ディスプレイにリアルタイムの電圧、電流、電力データを表示。
2. **大電流出力**: 最大1.5Aの3.3V電力を供給し、ほとんどのブレッドボードベースの電子プロジェクトに最適。
3. **内蔵LCDディスプレイ**: 統合されたLCDに重要な電源データが明確に表示され、リアルタイムのフィードバックを確認可能。
4. **プラグアンドプレイ設計**: 標準的なブレッドボードと互換性があり、追加設定なしで接続して電源を入れるだけですぐに使用可能。
5. **オープンソースとSeeed Studio XIAO互換性**: ほぼすべてのXIAO開発ボードと互換性があり（XIAO SAMD21とXIAO RA4M1のサポートは開発中）、USB-シリアル通信やPWM制御などの将来の拡張に対応したオープンソース設計。
6. **デュアルチャンネル電圧・電流センシング**: INA3221センサーを搭載し、デュアルチャンネルの電圧と電流モニタリングが可能。
7. **コンパクトパワーハウス**: このコンパクトな設計で3.3Vと5V出力を提供し、電力を犠牲にすることなくブレッドボードスペースを最適化。
8. **複数のUI機能**: データモニタリング、ラインチャート、統計モードの切り替えが可能。

![function](Docs/Images/pic_functions.png)
> 複数のUI機能

![switchCH](Docs/Images/pic_switchCH.png)
> チャートモードと統計モードでチャンネルを切り替えるにはダイヤルを長押し

## ハードウェア仕様

- **入力電圧**: Seeed Studio XIAOを介したUSB-C電源供給
- **出力電圧**: 5Vと3.3V出力を提供、3.3Vレールで最大1.5Aの電流
- **電流センシング**: 内蔵INA3221センサーで電圧、電流、電力をリアルタイム表示
- **マイクロコントローラー**: RP2040搭載で、センサーデータ、ディスプレイ制御を処理し、USB-シリアル通信やPWM生成などの将来の機能に対応
- **ディスプレイ**: リアルタイム電源モニタリング用のLCD画面
- **基板寸法**: 標準的なブレッドボードとシームレスに統合できるよう設計され、スペース使用を最小限に抑制

![power path](Docs/Images/pic_outputSwitch.png)

![XIAO PowerBread](Docs/Images/pic_hardwareSpec.png)

## はじめ方

### ハードウェア

- **XIAOボード**: [XIAO RP2040](https://www.seeedstudio.com/XIAO-RP2040-v1-0-p-5026.html)を強く推奨します。その他に使用可能なボード: [XIAO RP2350](https://www.seeedstudio.com/Seeed-XIAO-RP2350-p-5944.html)、[XIAO ESP32-C3](https://www.seeedstudio.com/Seeed-XIAO-ESP32C3-p-5431.html)、[XIAO ESP32-S3](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html)、[XIAO ESP32-C6](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C6-p-5884.html)。（お使いのボードタイプに対応したファームウェアを書き込んでください）
- **XIAO PowerBreadボード**: このリポジトリからデザインをクローンまたはリミックスしてください。また、[Seeed Studio](https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html)や[Tindie](https://www.tindie.com/products/35842/)からハードウェアを入手することもできます。
- **ブレッドボード**: 標準的なブレッドボードに対応し、簡単なプロトタイピングが可能です。
- **電源**: 標準的なUSB電源を使用します。

### ハードウェアの入手

<div align="center">
  <table>
    <tr>
      <td align="center">
        <a href="https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html" target="_blank">
          <img src="Docs/Images/seeed-logo.png" width="200"><br>
          <b>Seeed Studio</b><br>
          キットとXIAOボードを入手
        </a>
      </td>
      <td align="center">
        <a href="https://www.tindie.com/products/35842/" target="_blank">
          <img src="Docs/Images/tindie-logo.png" width="200"><br>
          <b>Tindie</b><br>
          完全組み立て済みハードウェアを入手
        </a>
      </td>
    </tr>
  </table>
</div>

**注意**: Seeed Studioで購入する場合、互換性のあるXIAOボードは別売りですので、必ず一緒に注文してください。

### ソフトウェア

- **プロジェクトのリミックス**: 提供されているArduinoソースコードを使用して、プロジェクトを変更またはリミックスできます。
- **コンパイル済みファームウェアを直接使用**: 
  - XIAO ESP32シリーズの場合
    - Webベースの書き込みツール: https://powerbread-flasher.ioatlas.com
    - チュートリアル: [XIAO - ESP32シリーズ用ファームウェアの書き込み](Docs/flash-firmware-for-esp32-series.md)
  - XIAO RP2040/RP2350の場合
    - 書き込みツール: UF2方式でファームウェアをアップロード:
      1. [リリースページ](https://github.com/nicho810/XIAO-PowerBread/releases)からファームウェアファイル(*.uf2)をダウンロード
      2. XIAOをUSBケーブル（4ピンのデータケーブルが必要）でコンピュータに接続
      3. ブートローダーモードに入る:
         - BOOTボタン(B)を押したまま
         - BOOTを押しながら、RESETボタン(R)を押して離す
         - BOOTボタンを離す
      4. コンピュータに"RPI-RP2"という名前の新しいUSBドライブが表示される
      5. .uf2ファームウェアファイルをRPI-RP2ドライブにドラッグ＆ドロップ
      6. XIAOは自動的にリセットされ、新しいファームウェアで起動します

### 計画中のソフトウェア機能
- [x] LCDにリアルタイムの電流センサーデータを表示
- [x] UIの視野角を調整するためのダイヤルホイール
- [x] 各チャンネルの電力使用を視覚化するラインチャートモード
- [x] 各チャンネルの平均（秒、分、全時間）とピーク電流消費をカウントするモード
- [x] 設定データをEEPROMに保存可能
- [ ] デバッグツールとしてのUSBシリアルモード
- [ ] IO0とIO1でのPWM出力
- [ ] IO0とIO1からのADC読み取り
- [ ] データモニタリング用WebベースUI

### ファームウェアリリース

| バージョン | 安定性 | タグ | 追加された機能 | リンク |
|---------|---------|---------|---------|---------|
| 0.9.0     | はい | - | LCDにリアルタイムの電流センサーデータを表示 | - |
| 1.0.0     | はい | - | UIの視野角を調整するためのダイヤルホイール | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.0.0) |
| 1.1.0     | いいえ | - | 各チャンネルの電力使用を視覚化するラインチャートモード | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.0) |
| 1.1.1     | はい | - | データダッシュボード、ラインチャート、電流統計、設定をサポート | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.1) |
| 1.1.2     | はい | - | デバッグ用のLED点滅機能を導入 | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.2) |
| 1.1.3     | はい | - | ラインチャートの固定スケールと自動スケールのサポートを追加 | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.3) |
| 1.1.4     | はい👍 | - | XIAO RP2350、XIAO ESP32-C3、XIAO ESP32-S3、XIAO ESP32-C6のサポートを追加 | [RP2040, RP2350, ESP32C3, ESP32S3, ESP32C6](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.4) |
| 2.0.0     | はい | - | コードをリファクタリングし、パフォーマンス向上のためUIレンダリングをLVGL(LovyanGFX)に変更 | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.0) |
| 2.0.1     | はい👍 | - | XIAO RP2350、XIAO ESP32-C3、XIAO ESP32-S3のサポートを追加 | [RP2040, RP2350, ESP32C3, ESP32S3](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.1) |

### ドキュメント
- [システム設定の説明](Docs/sysConfig.md)
  - デフォルトモード設定
  - シャント抵抗設定（20mΩ、50mΩ）
  - シリアル出力設定（人間が読める形式、Arduinoプロッター形式）
  - ラインチャート設定（チャート更新レート）

### トラブルシューティング
1. ディスプレイが応答しない
   - 最新のファームウェアを使用していることを確認してください。最新のファームウェアには安定性に関するバグ修正が含まれています。
   - XIAOボードのリセットボタンを押してリセットを試してください。
2. ディスプレイが動作しない
   - I2CとSPIピンの接続を確認し、XIAOのピンが正しくはんだ付けされていることを確認してください。
3. 電流値が正しくない
   - シャント抵抗値を確認し、sysConfigで正しく設定してください。（例：20mΩの場合は20、50mΩの場合は50）→ [システム設定の説明](Docs/sysConfig.md)
4. 両方の電圧表示が0V
   - 電源スイッチを確認してください。下側がONになっているはずです。（上側がOFF、下側がON）

### クレジット

| ライブラリ | v1.x | v2.x | リンク |
|---------|------|------|------|
| Arduino-Pico Core | ✅ | ✅ | [Arduino-Pico Core (4.x.x)](https://github.com/earlephilhower/arduino-pico) |
| Arduino-ESP32 Core | ✅ | ✅ | [Arduino-ESP32 Core (3.x.x)](https://github.com/espressif/arduino-esp32) |
| INA3221_RT Library | ✅ | ✅ | [INA3221_RT Library](https://github.com/RobTillaart/INA3221_RT/tree/master) |
| Adafruit GFX Library | ✅ |  | [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) |
| Adafruit ST7735 Library | ✅ |  | [Adafruit ST7735 Library](https://github.com/adafruit/Adafruit-ST7735-Library) |
| Adafruit SleepyDog | ✅ |  | [Adafruit SleepyDog](https://github.com/adafruit/Adafruit_SleepyDog) |
| LovyanGFX |  | ✅ | [LovyanGFX](https://github.com/lovyan03/LovyanGFX) |
| LVGL |  | ✅ | [LVGL](https://github.com/lvgl/lvgl) |

| ツール | リンク |
|---------|------|
| ESP32シリーズ用MergeBinツール | [DavidSchinazi、DunkelRatteに感謝](https://github.com/platformio/platform-espressif32/issues/1078#issuecomment-2219671743) |

## 貢献

XIAO PowerBreadプロジェクトの改善へのご協力をお待ちしています！プルリクエストの提出、新機能の提案、バグ報告など、イシュートラッカーをご利用ください。

## ライセンス

このプロジェクトはMITライセンスの下で公開されています。詳細は[LICENSE](./LICENSE)ファイルをご覧ください。
