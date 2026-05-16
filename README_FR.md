# XIAO PowerBread - Une alimentation pour plaque d'essai avec surveillance en temps réel

[English](README.md) | [Deutsch](README_DE.md) | Français | [日本語](README_JP.md)

**XIAO PowerBread** est un projet de matériel open-source conçu pour offrir une solution d'alimentation fiable et efficace pour le prototypage sur plaque d'essai. Doté de capteurs intégrés, d'une surveillance en temps réel et de la compatibilité avec les microcontrôleurs [Seeed Studio XIAO](https://www.seeedstudio.com/xiao-series-page), PowerBread facilite plus que jamais l'alimentation et le développement de projets électroniques.

![XIAO PowerBread](Docs/Images/pic_overview.webp)
> Installation facile et conception compacte

![XIAO PowerBread](Docs/Images/pic_directPowerAndMonitoring.webp)
> Alimentation directe et surveillance de vos projets sur plaque d'essai

## Nouveautés de la v3.0

- **Console Web** — Diffuse en direct la tension, le courant, la puissance et l'énergie vers le navigateur via USB-Série (sans pilote, sans installation).
- **Protocole binaire XPB v1.2** — Protocole de streaming déterministe à 100 Hz avec un handshake en trois étapes et une reconnexion automatique.
- **Refactorisation FreeRTOS** — L'échantillonnage des capteurs, le rendu de l'interface, l'entrée de la molette et le streaming série s'exécutent comme des tâches indépendantes avec une synchronisation explicite.
- **LVGL 8.3 + LovyanGFX** — Rendu plus fluide sur l'écran LCD ST7735 embarqué.

> [!IMPORTANT]
> **Le firmware v3.0 prend en charge XIAO ESP32-S3, ESP32-C3 et ESP32-C6.**
> Les portages RP2040 / RP2350 exposent encore un bug d'ordonnancement FreeRTOS dont la cause n'a pas encore été identifiée. Si vous utilisez un XIAO RP2040 ou RP2350, veuillez rester sur le firmware **v2.0.1** en attendant l'arrivée du portage v3.x. Voir le tableau [Versions du firmware](#versions-du-firmware) ci-dessous.

## Caractéristiques principales

1. **Surveillance en temps réel** : Visualisez les métriques d'alimentation essentielles d'un coup d'œil, sans multimètre. L'écran intégré affiche en temps réel la tension, le courant et la puissance.
2. **Sortie à courant élevé** : Fournit jusqu'à 1,5 A en 3,3 V, idéal pour la plupart des projets sur plaque d'essai.
3. **Écran LCD intégré** : Restez informé avec un retour en temps réel — les données critiques sont clairement affichées sur l'écran LCD intégré.
4. **Console Web (v3.0)** : Diffuse les données des capteurs vers un tableau de bord web via USB-Série, sans logiciel supplémentaire à installer.
5. **Conception plug-and-play** : Compatible avec les plaques d'essai standard, il suffit de brancher, d'alimenter et de commencer à prototyper.
6. **Open-source et compatibilité Seeed Studio XIAO** : Compatible avec la plupart des cartes XIAO. Cette conception open-source offre une flexibilité pour les extensions futures comme la communication USB-série et le contrôle PWM.
7. **Détection de tension et de courant à double canal** : Équipé du capteur INA3221 pour la surveillance de la tension et du courant à double canal — connaissez tous les aspects de votre alimentation.
8. **Centrale électrique compacte** : Cette conception compacte fournit des sorties 3,3 V et 5 V, optimisant l'espace sur la plaque d'essai sans compromettre la puissance.
9. **Plusieurs modes d'interface** : L'interface intégrée peut basculer entre la surveillance des données, le graphique linéaire et le mode statistique.

![function](Docs/Images/pic_functions.webp)
> Plusieurs fonctions d'interface utilisateur

![switchCH](Docs/Images/pic_switchCH.webp)
> Appuyez longuement sur la molette pour changer de canal en mode graphique et statistique

## Console Web

La **Console Web XPB** est une application navigateur compagnon qui transforme votre XIAO PowerBread en station complète d'analyse d'alimentation. Branchez la carte dans un navigateur basé sur Chromium, cliquez sur **Connect**, et le firmware diffuse en direct les mesures via l'API Web Serial — sans pilote, sans installation, sans backend.

![Console Web](Docs/Images/web-console/web-console-cover.webp)

### Points forts

- **Graphiques en direct** pour la tension, le courant et la puissance sur les deux canaux, échantillonnés à 100 Hz.
- **Intégration d'énergie par canal** (mAh / mWh) avec réinitialisation en un clic.
- **Handshake en trois étapes** qui détecte automatiquement le dispositif, négocie le protocole et démarre le streaming.
- **Thème clair / sombre** avec réglages persistants.
- **Console de débogage** qui affiche les trames brutes du protocole pour le dépannage.

### Captures d'écran

<table>
  <tr>
    <td align="center">
      <img src="Docs/Images/web-console/light-mode.webp" width="420"><br>
      <b>Mode clair</b>
    </td>
    <td align="center">
      <img src="Docs/Images/web-console/dark-mode.webp" width="420"><br>
      <b>Mode sombre</b>
    </td>
  </tr>
</table>

### Prérequis navigateur

La Console Web repose sur l'**API Web Serial**, prise en charge par les navigateurs basés sur Chromium :

- Chrome 89+
- Edge 89+
- Opera 76+

Firefox et Safari n'exposent pas actuellement l'API Web Serial.

### Accès

Un outil compagnon pratique pour votre PowerBread — ouvrez-le dans votre navigateur : **<https://xiao-power-console-web.ioatlas.com>**

## Spécifications matérielles

- **Tension d'entrée** : Alimenté en USB-C via le Seeed Studio XIAO.
- **Tensions de sortie** : Fournit 5 V et 3,3 V, avec un courant maximum de 1,5 A pour le rail 3,3 V.
- **Mesure du courant** : Le capteur INA3221 intégré mesure la tension, le courant et la puissance pour un affichage en temps réel.
- **Microcontrôleur** : L'un des modules Seeed Studio XIAO pris en charge — gère l'acquisition des capteurs, l'affichage et le streaming série.
- **Affichage** : Écran LCD ST7735 0,96" pour la surveillance en temps réel.
- **Dimensions du PCB** : Conçu pour une intégration parfaite avec les plaques d'essai standard tout en minimisant l'utilisation de l'espace.

![power path](Docs/Images/pic_outputSwitch.webp)

![XIAO PowerBread](Docs/Images/pic_hardwareSpec.webp)

## Pour commencer

### Matériel

- **Carte XIAO** — choisissez l'un des modules pris en charge ci-dessous.
- **Carte XIAO PowerBread** — clonez ou remixez depuis ce dépôt, ou achetez la version assemblée sur [Seeed Studio](https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html) ou [Tindie](https://www.tindie.com/products/35842/).
- **Plaque d'essai** — s'adapte à toute plaque d'essai standard.
- **Alimentation** — toute source USB-C standard.

#### Cartes XIAO prises en charge & firmware

| Carte XIAO | v3.x | v2.0.1 | v1.1.4 | Firmware recommandé |
|---|---|---|---|---|
| [XIAO ESP32-S3](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html) | ✅ | ✅ | ✅ | **v3.0** |
| [XIAO ESP32-C3](https://www.seeedstudio.com/Seeed-XIAO-ESP32C3-p-5431.html) | ✅ | ✅ | ✅ | **v3.0** |
| [XIAO RP2040](https://www.seeedstudio.com/XIAO-RP2040-v1-0-p-5026.html) | ❌ (WIP) | ✅ | ✅ | **v2.0.1** |
| [XIAO RP2350](https://www.seeedstudio.com/Seeed-XIAO-RP2350-p-5944.html) | ❌ (WIP) | ✅ | ✅ | **v2.0.1** |
| [XIAO ESP32-C6](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C6-p-5884.html) | ✅ | — | ✅ | **v3.0** |

> Le support v3.x pour RP2040 et RP2350 est toujours en cours. Les cartes RP exposent actuellement un problème d'ordonnancement FreeRTOS dont la cause n'a pas été identifiée ; veuillez rester sur v2.0.1 en attendant l'arrivée du portage v3.x.

### Obtenez le matériel

<div align="center">
  <table>
    <tr>
      <td align="center">
        <a href="https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html" target="_blank">
          <img src="Docs/Images/seeed-logo.webp" width="200"><br>
          <b>Seeed Studio</b><br>
          Obtenez le kit et la carte XIAO
        </a>
      </td>
      <td align="center">
        <a href="https://www.tindie.com/products/35842/" target="_blank">
          <img src="Docs/Images/tindie-logo.webp" width="200"><br>
          <b>Tindie</b><br>
          Obtenez le matériel entièrement assemblé
        </a>
      </td>
    </tr>
  </table>
</div>

**Note** : Lors de l'achat chez Seeed Studio, n'oubliez pas de commander également une carte XIAO compatible — elle est requise mais vendue séparément.

### Logiciel

- **Construire depuis les sources** — ouvrez `Firmware/XPB-Firmware-PlatformIO/` dans PlatformIO et sélectionnez l'environnement correspondant à votre carte (`seeed_xiao_esp32s3`, `seeed_xiao_esp32c3`, `seeed_xiao_esp32c6`, etc.).
- **Flasher un firmware précompilé** :
  - **XIAO ESP32-S3 / ESP32-C3 / ESP32-C6 (v3.x)**
    - Outil de flash web : <https://powerbread-flasher.ioatlas.com>
    - Tutoriel : [Flasher le firmware pour la série XIAO ESP32](Docs/flash-firmware-for-esp32-series.md)
  - **XIAO RP2040 / RP2350 (rester sur v2.0.1 pour l'instant)**
    1. Téléchargez le firmware `.uf2` depuis la [page des versions](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.1).
    2. Connectez le XIAO à votre ordinateur avec un câble USB 4 broches compatible données.
    3. Entrez en mode bootloader : maintenez **BOOT (B)**, tapez sur **RESET (R)**, puis relâchez **BOOT**.
    4. Un lecteur USB nommé `RPI-RP2` apparaît.
    5. Faites glisser le `.uf2` sur le lecteur — la carte redémarre avec le nouveau firmware.

### Utiliser la Console Web

1. Assurez-vous que votre carte fonctionne avec le firmware v3.x (XIAO ESP32-S3, ESP32-C3 ou ESP32-C6).
2. Ouvrez la Console Web à l'adresse <https://xiao-power-console-web.ioatlas.com> dans Chrome / Edge / Opera.
3. Cliquez sur **Connect**, sélectionnez le port série correspondant à votre XIAO et accordez l'autorisation.
4. Le firmware passe du mode texte au streaming binaire dès la fin du handshake — les graphiques se remplissent immédiatement.

### Fonctionnalités logicielles prévues
- [x] Données du capteur de courant en temps réel sur l'écran LCD.
- [x] Molette pour ajuster l'interface selon l'angle de vue.
- [x] Mode graphique linéaire pour visualiser la consommation par canal.
- [x] Mode statistique pour moyenne (seconde, minute, totale) et pic par canal.
- [x] Sauvegarde des réglages dans l'EEPROM.
- [x] Interface utilisateur web pour la surveillance des données (v3.0).
- [x] Protocole de streaming USB-Série avec handshake et reconnexion (v3.0).
- [ ] Portage v3.x pour XIAO RP2040 / RP2350.
- [ ] Sortie PWM sur IO0 et IO1.
- [ ] Lecture ADC depuis IO0 et IO1.

### Versions du firmware

| Version | Stable | Fonctionnalités | Lien |
|---|---|---|---|
| 0.9.0 | Oui | Données du capteur de courant en temps réel sur l'écran LCD. | - |
| 1.0.0 | Oui | Molette pour ajuster l'interface. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.0.0) |
| 1.1.0 | Non | Mode graphique linéaire pour la consommation. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.0) |
| 1.1.1 | Oui | Dashboard, graphique linéaire, statistiques, réglages. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.1) |
| 1.1.2 | Oui | Clignotement LED pour débogage. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.2) |
| 1.1.3 | Oui | Échelle fixe et automatique pour le graphique linéaire. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.3) |
| 1.1.4 | Oui | Ajout du support RP2350, ESP32-C3, ESP32-S3, ESP32-C6. | [RP2040, RP2350, ESP32C3, ESP32S3, ESP32C6](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.4) |
| 2.0.0 | Oui | Refactorisation + passage à LVGL / LovyanGFX. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.0) |
| 2.0.1 | Oui 👍 (RP2040 / RP2350) | Ajout du support RP2350, ESP32-C3, ESP32-S3. | [RP2040, RP2350, ESP32C3, ESP32S3](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.1) |
| **3.0.0** | **Oui 👍 (ESP32-S3 / C3 / C6)** | **Console Web, Protocole binaire XPB v1.2, refactorisation FreeRTOS. ESP32-S3 / C3 / C6.** | [ESP32S3, ESP32C3, ESP32C6](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v3.0.0) |

### Documentation
- [Explication de la configuration du système](Docs/sysConfig.md)
  - Réglage du mode par défaut
  - Réglage de la résistance shunt (20 mΩ, 50 mΩ)
  - Réglage de la sortie série (mode lisible, mode traceur Arduino)
  - Réglage du graphique linéaire (taux de rafraîchissement)
- [Protocole binaire XPB v1.2](Firmware/XPB-Firmware-PlatformIO/docs/) — handshake, structure de trame, sémantique de reconnexion STREAMING.
- [Flasher le firmware pour la série ESP32](Docs/flash-firmware-for-esp32-series.md)

### Dépannage

1. **L'écran ne répond pas** — assurez-vous d'utiliser le dernier firmware ; les dernières versions contiennent des corrections de stabilité. Essayez de presser le bouton de reset du XIAO.
2. **L'écran ne fonctionne pas** — vérifiez les connexions des broches I2C et SPI, et assurez-vous que le XIAO est correctement soudé.
3. **La valeur du courant est incorrecte** — vérifiez la valeur de la résistance shunt et définissez-la correctement dans `sysConfig` (par exemple `20` pour 20 mΩ, `50` pour 50 mΩ). Voir [Configuration système](Docs/sysConfig.md).
4. **Les deux tensions affichées sont à 0 V** — vérifiez l'interrupteur d'alimentation ; il doit être en position **BAS** (HAUT = OFF, BAS = ON).
5. **La Console Web affiche « Web Serial API not supported »** — utilisez Chrome, Edge ou Opera. Firefox et Safari n'implémentent pas Web Serial.
6. **La Console Web se connecte mais aucune donnée n'arrive** — assurez-vous que le firmware est en v3.x et qu'aucune autre application (Arduino Serial Monitor, PlatformIO Monitor) n'occupe le port série.

### Crédits

| Bibliothèque | v1.x | v2.x | v3.x | Lien |
|---|---|---|---|---|
| Arduino-Pico Core | ✅ | ✅ | ✅ | [arduino-pico](https://github.com/earlephilhower/arduino-pico) |
| Arduino-ESP32 Core | ✅ | ✅ | ✅ | [arduino-esp32](https://github.com/espressif/arduino-esp32) |
| INA3221_RT | ✅ | ✅ | ✅ | [INA3221_RT](https://github.com/RobTillaart/INA3221_RT/tree/master) |
| Adafruit GFX | ✅ |  |  | [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library) |
| Adafruit ST7735 | ✅ |  |  | [Adafruit-ST7735](https://github.com/adafruit/Adafruit-ST7735-Library) |
| Adafruit SleepyDog | ✅ |  |  | [Adafruit_SleepyDog](https://github.com/adafruit/Adafruit_SleepyDog) |
| LovyanGFX |  | ✅ | ✅ | [LovyanGFX](https://github.com/lovyan03/LovyanGFX) |
| LVGL |  | ✅ | ✅ | [LVGL](https://github.com/lvgl/lvgl) |
| FreeRTOS |  |  | ✅ | fourni avec Arduino-ESP32 / Arduino-Pico |
| Chart.js (Console Web) |  |  | ✅ | [Chart.js](https://www.chartjs.org/) |

| Outil | Lien |
|---|---|
| Outil MergeBin pour la série ESP32 | [Merci à DavidSchinazi, DunkelRatte](https://github.com/platformio/platform-espressif32/issues/1078#issuecomment-2219671743) |
| Console Web XPB | <https://xiao-power-console-web.ioatlas.com> |

## Contribuer

Les contributions sont les bienvenues ! Pull request, propositions de nouvelles fonctionnalités, rapports de bugs — n'hésitez pas à utiliser le tracker d'issues. **L'aide pour le portage v3.x RP2040 / RP2350 (FreeRTOS) est particulièrement appréciée** — voir les issues ouvertes avec le tag `rp-rtos`.

## Licence

Ce projet est sous licence MIT. Voir le fichier [LICENSE](./LICENSE) pour plus de détails.
