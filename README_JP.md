# XIAO PowerBread - リアルタイムモニタリング機能付きブレッドボード電源供給装置

[English](README.md) | [Deutsch](README_DE.md) | [Français](README_FR.md) | 日本語

**XIAO PowerBread**は、ブレッドボードプロトタイピング用の信頼性の高い効率的な電源ソリューションを提供するオープンソースハードウェアプロジェクトです。内蔵センサー、リアルタイムモニタリング、[Seeed Studio XIAOマイクロコントローラー](https://www.seeedstudio.com/xiao-series-page)との互換性を備え、PowerBreadは電子プロジェクトの電源供給と開発をこれまで以上に容易にします。

![XIAO PowerBread](Docs/Images/pic_overview.png)
> 簡単な設置とコンパクトな設計

![XIAO PowerBread](Docs/Images/pic_directPowerAndMonitoring.png)
> ブレッドボードプロジェクトへの直接電源供給とモニタリング

## 主な特徴

1. **リアルタイムモニタリング**: マルチメーター不要で重要な電源メトリクスを一目で確認。内蔵ディスプレイにリアルタイムの電圧、電流、電力データを表示。
2. **高電流出力**: 最大1.5Aの3.3V電源を供給。ほとんどのブレッドボードベースの電子プロジェクトに最適。
3. **内蔵LCDディスプレイ**: リアルタイムフィードバックで常に情報を把握。重要な電源データが内蔵LCDに明確に表示されます。
4. **プラグアンドプレイ設計**: 標準的なブレッドボードと互換性があり、追加設定なしで簡単に接続、電源投入、プロトタイピングが可能。
5. **オープンソースとSeeed Studio XIAO互換**: ほぼすべてのXIAO開発ボードと互換性があります（XIAO SAMD21とXIAO RA4M1のサポートは開発中）。このオープンソース設計により、USB-シリアル通信やPWM制御などの将来の拡張に柔軟性を提供。
6. **デュアルチャンネル電圧・電流センシング**: INA3221センサーを搭載し、デュアルチャンネルの電圧・電流モニタリングが可能。電源供給のあらゆる側面を把握できます。
7. **コンパクトなパワーハウス**: このコンパクトな設計で3.3Vと5Vの出力を提供し、電力を犠牲にすることなくブレッドボードのスペースを最適化。
8. **複数のUI機能**: UIはデータモニタリング、ラインチャート、統計モードの間で切り替え可能。

![function](Docs/Images/pic_functions.png)
> 複数のUI機能

![switchCH](Docs/Images/pic_switchCH.png)
> ダイヤルを長押しして、チャートモードと統計モードでチャネルを切り替え

![uiRotate](Docs/Images/pic_uiRotation.png)
> ダイヤルを回転させて、異なる視角に合わせてUIを調整

## ハードウェア仕様

- **入力電圧**: XIAOを介してUSB-Cから電源供給。
- **出力電圧**: 5Vと3.3Vの出力を提供。3.3Vレールの最大電流は1.5A。
- **電流センシング**: 内蔵INA3221センサーが電圧、電流、電力を測定し、リアルタイム表示。
- **マイクロコントローラー**: RP2040搭載。センサーデータの処理、ディスプレイ制御を行い、将来的にUSB-シリアル通信やPWM生成などの機能を可能に。
- **ディスプレイ**: リアルタイム電源モニタリング用のLCD画面。
- **PCBサイズ**: 標準的なブレッドボードとシームレスに統合できるよう設計され、スペース使用を最小限に抑えています。

![XIAO PowerBread](Docs/Images/pic_hardwareSpec.png)

## はじめに

### ハードウェア

- **XIAOボード**: [XIAO RP2040](https://www.seeedstudio.com/XIAO-RP2040-v1-0-p-5026.html)を強く推奨します。また以下のボードも使用可能です：[XIAO RP2350](https://www.seeedstudio.com/Seeed-XIAO-RP2350-p-5944.html)、[XIAO ESP32-C3](https://www.seeedstudio.com/Seeed-XIAO-ESP32C3-p-5431.html)、[XIAO ESP32-S3](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html)、[XIAO ESP32-C6](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C6-p-5884.html)。（お使いのボードタイプに対応したファームウェアを必ずフラッシュしてください）
- **XIAO PowerBreadボード**: このリポジトリからデザインをクローンまたはリミックスできます。また、[Seeed Studio](https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html)や[Tindie](https://www.tindie.com/products/35842/)からハードウェアを入手することもできます。
- **ブレッドボード**: 標準的なブレッドボードに対応し、簡単なプロトタイピングが可能。
- **電源**: 標準的なUSB電源を使用。

### ハードウェアの入手

<div align="center">
  <table>
    <tr>
      <td align="center">
        <a href="https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html" target="_blank">
          <img src="Docs/Images/seeed-logo.png" width="200"><br>
          <b>Seeed Studio</b><br>
          キットとXIAOボードの入手
        </a>
      </td>
      <td align="center">
        <a href="https://www.tindie.com/products/35842/" target="_blank">
          <img src="Docs/Images/tindie-logo.png" width="200"><br>
          <b>Tindie</b><br>
          完全組み立て済みハードウェアの入手
        </a>
      </td>
    </tr>
  </table>
</div>

### ソフトウェア

- **プロジェクトのリミックス**: 提供されているArduinoソースコードを使用してプロジェクトを変更またはリミックス。
- **コンパイル済みファームウェアを直接使用**: 
  - XIAO ESP32シリーズの場合
    - ウェブベースのフラッシュツール: https://powerbread-flasher.ioatlas.com
    - チュートリアル: [XIAO - ESP32シリーズ向けファームウェアのフラッシュ](Docs/flash-firmware-for-esp32-series.md)
  - XIAO RP2040/RP2350の場合
    - フラッシュツール: UF2方式でファームウェアをアップロード:
      1. [リリースページ](https://github.com/nicho810/XIAO-PowerBread/releases)からファームウェアファイル(*.uf2)をダウンロード
      2. XIAOをUSB経由でPCに接続（4ピンデータケーブルを使用）
      3. ブートローダーモードに入る:
         - BOOTボタン(B)を押し続ける
         - BOOTを押したまま、RESETボタン(R)を押して離す
         - BOOTボタンを離す
      4. "RPI-RP2"という名前の新しいUSBドライブがPCに表示される
      5. .uf2ファームウェアファイルをRPI-RP2ドライブにドラッグ＆ドロップ
      6. XIAOは自動的にリセットされ、新しいファームウェアで起動する

![power path](Docs/Images/pic_outputSwitch.png)

### 計画中のソフトウェア機能
- [x] LCDにリアルタイムの電流センサーデータを表示
- [x] 異なる視角に合わせてUIを調整するダイヤルホイール
- [x] 各チャンネルの電力使用を視覚化するラインチャートモード
- [x] 各チャンネルの平均（秒、分、全時間）とピーク電流消費をカウントするカウントモード
- [x] 設定データをEEPROMに保存可能
- [ ] デバッグツールとしてのUSB-シリアルモード
- [ ] IO0とIO1でのPWM出力
- [ ] IO0とIO1からのADC読み取り
- [ ] データモニタリング用のウェブベースUI

### ファームウェアリリース

| バージョン | 安定版 | 開発版 | 追加された機能 | リンク |
|---------|---------|---------|---------|---------|
| 0.9.0     | はい | はい | LCDにリアルタイムの電流センサーデータを表示 | - |
| 1.0.0     | はい | はい | 異なる視角に合わせてUIを調整するダイヤルホイール | [リリースページ](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.0.0) |
| 1.1.0     | いいえ | はい | 各チャンネルの電力使用を視覚化するラインチャートモード | [リリースページ](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.0) |
| 1.1.1     | はい | はい | データダッシュボード、ラインチャート、電流統計、設定をサポート | [リリースページ](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.1) |
| 1.1.2     | はい | はい | デバッグ目的のLED点滅機能を導入 | [リリースページ](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.2) |
| 1.1.3     | はい👍 | はい | ラインチャートの固定スケールと自動スケールのサポートを追加 | [リリースページ](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.3) |
| 1.1.4     | はい👍 | はい | XIAO RP2350、XIAO ESP32-C3、XIAO ESP32-S3、XIAO ESP32-C6のサポートを追加 | [リリースページ](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.4) |

### トラブルシューティング
1. ディスプレイが反応しない
   - 最新のファームウェアを使用していることを確認してください。最新のファームウェアには安定性のためのバグ修正が含まれています。
   - XIAOボードのリセットボタンを押してリセットしてみてください。
2. ディスプレイが動作しない
   - XIAOの赤いLEDが点滅している場合、INA3221が正しく接続されていないことを意味します。I2Cピンの接続を確認してください。
   - XIAOの赤いLEDが点滅していない場合、ディスプレイが正しく駆動されていないことを意味します。SPIピンの接続を確認してください。
3. 電流値が正しくない
   - シャント抵抗の値を確認し、sysConfigで正しく設定してください。（例：20mΩの場合は20、50mΩの場合は50） -> [システム設定の説明](Docs/sysConfig.md)
4. 両方の電圧がディスプレイで0Vと表示される
   - 電源スイッチを確認してください。下側にあるべきです。（上側がOFF、下側がON）

### ドキュメンテーション
- [システム設定の説明](Docs/sysConfig.md)
  - デフォルトモード設定
  - シャント抵抗設定（20mΩ、50mΩ）
  - シリアル出力設定（人間可読モード、Arduinoプロッターモード）
  - ラインチャート設定（チャート更新レート）

### 使用ライブラリ
- [INA3221_RT ライブラリ](https://github.com/RobTillaart/INA3221_RT/tree/master)
- [Adafruit GFX ライブラリ](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit ST7735 ライブラリ](https://github.com/adafruit/Adafruit-ST7735-Library)
- [Arduino-Pico Core (4.0.x)](https://github.com/earlephilhower/arduino-pico)
- [Arduino-ESP32 Core (3.x.x)](https://github.com/espressif/arduino-esp32)
- [adafruit sleepydog](https://github.com/adafruit/Adafruit_SleepyDog)

> v1.1.2以降、このプロジェクトで使用されているLCDモジュールに適合するように修正されたAdafruit_ST7735ライブラリのバージョンが含まれています。

## 貢献

XIAO PowerBreadプロジェクトの改善に貢献を歓迎します！プルリクエストの提出、新機能の提案、バグの報告など、イシュートラッカーを自由にご利用ください。

## ライセンス

このプロジェクトはMITライセンスの下でライセンスされています。詳細は[LICENSE](./LICENSE)ファイルをご覧ください。