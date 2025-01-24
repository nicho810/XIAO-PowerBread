# XIAO PowerBread - Une alimentation pour plaque d'essai avec surveillance en temps réel

[English](README.md) | [Deutsch](README_DE.md) | Français | [日本語](README_JP.md)

**XIAO PowerBread** est un projet de matériel open-source conçu pour offrir une solution d'alimentation fiable et efficace pour le prototypage sur plaque d'essai. Doté de capteurs intégrés, d'une surveillance en temps réel et de la compatibilité avec les microcontrôleurs [Seeed Studio XIAO](https://www.seeedstudio.com/xiao-series-page), PowerBread facilite plus que jamais l'alimentation et le développement de projets électroniques.

![XIAO PowerBread](Docs/Images/pic_overview.png)
> Installation facile et conception compacte

![XIAO PowerBread](Docs/Images/pic_directPowerAndMonitoring.png)
> Alimentation directe et surveillance de vos projets sur plaque d'essai

## Caractéristiques principales

1. **Surveillance en temps réel** : Visualisez les métriques d'alimentation essentielles d'un coup d'œil, sans multimètre. L'écran intégré affiche en temps réel les données de tension, de courant et de puissance.
2. **Sortie à courant élevé** : Fournit jusqu'à 1,5A de puissance en 3,3V, idéal pour la plupart des projets électroniques sur plaque d'essai.
3. **Écran LCD intégré** : Restez informé avec un retour en temps réel - les données d'alimentation critiques sont clairement affichées sur l'écran LCD intégré.
4. **Conception plug-and-play** : Compatible avec les plaques d'essai standard, il suffit de brancher, d'alimenter et de commencer à prototyper sans configuration supplémentaire.
5. **Open-source et compatibilité Seeed Studio XIAO** : Compatible avec presque toutes les cartes de développement XIAO (le support pour XIAO SAMD21 et XIAO RA4M1 est en cours), cette conception open-source offre une flexibilité pour les extensions futures comme la communication USB-série et le contrôle PWM.
6. **Détection de tension et de courant à double canal** : Équipé du capteur INA3221 pour la surveillance de la tension et du courant à double canal - connaissez tous les aspects de votre alimentation.
7. **Centrale électrique compacte** : Cette conception compacte fournit des sorties 3,3V et 5V, optimisant l'espace de votre plaque d'essai sans compromettre la puissance.
8. **Fonctions d'interface utilisateur multiples** : L'interface peut basculer entre la surveillance des données, le graphique linéaire et le mode statistique.

![function](Docs/Images/pic_functions.png)
> Fonctions d'interface utilisateur multiples

![switchCH](Docs/Images/pic_switchCH.png)
> Appuyez longuement sur la molette pour changer de canal en mode graphique et statistique

## Spécifications matérielles

- **Tension d'entrée** : Alimenté via USB-C à travers le Seeed Studio XIAO.
- **Tensions de sortie** : Fournit des sorties 5V et 3,3V, avec un courant maximum de 1,5A pour le rail 3,3V.
- **Détection de courant** : Le capteur INA3221 intégré mesure la tension, le courant et la puissance pour un affichage en temps réel.
- **Microcontrôleur** : Piloté par le RP2040, gérant les données des capteurs, le contrôle de l'affichage et permettant des fonctionnalités futures comme la communication USB-série et la génération PWM.
- **Affichage** : Écran LCD pour la surveillance de l'alimentation en temps réel.
- **Dimensions du PCB** : Conçu pour une intégration parfaite avec les plaques d'essai standard tout en minimisant l'utilisation de l'espace.

![power path](Docs/Images/pic_outputSwitch.png)

![XIAO PowerBread](Docs/Images/pic_hardwareSpec.png)

## Pour commencer

### Matériel

- **Carte XIAO** : Le [XIAO RP2040](https://www.seeedstudio.com/XIAO-RP2040-v1-0-p-5026.html) est hautement recommandé, vous pouvez également utiliser : [XIAO RP2350](https://www.seeedstudio.com/Seeed-XIAO-RP2350-p-5944.html), [XIAO ESP32-C3](https://www.seeedstudio.com/Seeed-XIAO-ESP32C3-p-5431.html), [XIAO ESP32-S3](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html), [XIAO ESP32-C6](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C6-p-5884.html). (Assurez-vous de flasher le firmware compatible avec votre type de carte)
- **Carte XIAO PowerBread** : Clonez ou remixez la conception à partir de ce dépôt. Vous pouvez également obtenir le matériel sur [Seeed Studio](https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html) ou [Tindie](https://www.tindie.com/products/35842/).
- **Plaque d'essai** : S'adapte aux plaques d'essai standard pour un prototypage facile.
- **Alimentation** : Utilisez une source d'alimentation USB standard.

### Obtenez le matériel

<div align="center">
  <table>
    <tr>
      <td align="center">
        <a href="https://www.seeedstudio.com/XIAO-PowerBread-p-6318.html" target="_blank">
          <img src="Docs/Images/seeed-logo.png" width="200"><br>
          <b>Seeed Studio</b><br>
          Obtenez le kit et la carte XIAO
        </a>
      </td>
      <td align="center">
        <a href="https://www.tindie.com/products/35842/" target="_blank">
          <img src="Docs/Images/tindie-logo.png" width="200"><br>
          <b>Tindie</b><br>
          Obtenez le matériel entièrement assemblé
        </a>
      </td>
    </tr>
  </table>
</div>

**Note** : Lors de l'achat chez Seeed Studio, n'oubliez pas de commander également une carte XIAO compatible, car elle est requise mais vendue séparément.

### Logiciel

- **Remixer le projet** : Modifiez ou remixez le projet en utilisant le code source Arduino fourni.
- **Utiliser directement le firmware compilé** :
  - Pour les séries XIAO ESP32
    - Outil de flash web : https://powerbread-flasher.ioatlas.com
    - Tutoriel : [Flash du firmware pour les séries XIAO - ESP32](Docs/flash-firmware-for-esp32-series.md)
  - Pour XIAO RP2040/RP2350
    - Outil de flash : Utilisez la méthode UF2 pour télécharger le firmware :
      1. Téléchargez le fichier firmware (*.uf2) depuis la [page des versions](https://github.com/nicho810/XIAO-PowerBread/releases)
      2. Connectez XIAO à votre ordinateur via un câble USB (doit être un câble de données à 4 broches)
      3. Entrez en mode bootloader :
         - Appuyez et maintenez le bouton BOOT (B)
         - Tout en maintenant BOOT, appuyez et relâchez le bouton RESET (R)
         - Relâchez le bouton BOOT
      4. Un nouveau lecteur USB nommé "RPI-RP2" apparaîtra sur votre ordinateur
      5. Faites glisser et déposez le fichier firmware .uf2 sur le lecteur RPI-RP2
      6. Le XIAO redémarrera automatiquement et exécutera le nouveau firmware

### Fonctionnalités logicielles prévues
- [x] Données du capteur de courant en temps réel affichées sur l'écran LCD.
- [x] Molette pour ajuster l'interface utilisateur selon différents angles de vue.
- [x] Mode graphique linéaire pour visualiser l'utilisation de l'énergie pour chaque canal.
- [x] Mode de comptage pour compter la consommation moyenne (seconde, minute, tout le temps) et de pointe pour chaque canal.
- [x] Permet de sauvegarder les données de paramétrage dans l'EEPROM.
- [ ] Mode USB-Série comme outil de débogage.
- [ ] Sortie PWM sur IO0 et IO1.
- [ ] Lecture ADC depuis IO0 et IO1.
- [ ] Interface utilisateur web pour la surveillance des données.

### Versions du firmware

| Version | Stable | Tags | Fonctionnalités ajoutées | Lien |
|---------|---------|---------|---------|---------|
| 0.9.0   | Oui | - | Données du capteur de courant en temps réel affichées sur l'écran LCD. | - |
| 1.0.0   | Oui | - | Molette pour ajuster l'interface utilisateur selon différents angles de vue. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.0.0) |
| 1.1.0   | Non | - | Mode graphique linéaire pour visualiser l'utilisation de l'énergie pour chaque canal. | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.0) |
| 1.1.1   | Oui | - | Support du tableau de bord de données, du graphique linéaire, des statistiques de courant, des paramètres | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.1) |
| 1.1.2   | Oui | - | Introduit la fonctionnalité de clignotement LED à des fins de débogage | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.2) |
| 1.1.3   | Oui | - | Ajout du support d'échelle fixe et d'échelle automatique pour le graphique linéaire | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.3) |
| 1.1.4   | Oui👍 | - | Ajout du support pour XIAO RP2350, XIAO ESP32-C3, XIAO ESP32-S3, XIAO ESP32-C6 | [RP2040, RP2350, ESP32C3, ESP32S3, ESP32C6](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v1.1.4) |
| 2.0.0   | Oui | - | Refactorisation du code et passage à LVGL(LovyanGFX) pour le rendu de l'interface utilisateur pour améliorer les performances | [RP2040](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.0) |
| 2.0.1   | Oui👍 | - | Ajout du support pour XIAO RP2350, XIAO ESP32-C3, XIAO ESP32-S3 | [RP2040, RP2350, ESP32C3, ESP32S3](https://github.com/nicho810/XIAO-PowerBread/releases/tag/v2.0.1) |

### Documentation
- [Explication de la configuration du système](Docs/sysConfig.md)
  - Paramétrage du mode par défaut
  - Paramétrage de la résistance shunt (20mOhm, 50mOhm)
  - Paramétrage de la sortie série (mode lisible par l'homme, mode traceur Arduino)
  - Paramétrage du graphique linéaire (taux de rafraîchissement du graphique)

### Dépannage
1. L'écran ne répond pas
   - Assurez-vous d'utiliser le dernier firmware, le dernier firmware contient des corrections de bugs pour la stabilité.
   - Essayez de réinitialiser le XIAO en appuyant sur le bouton de réinitialisation de la carte XIAO.
2. L'écran ne fonctionne pas
   - Veuillez vérifier les connexions des broches I2C et SPI, et assurez-vous que le XIAO est correctement soudé aux broches.
3. La valeur du courant n'est pas correcte
   - Vérifiez la valeur de la résistance shunt et définissez-la correctement dans la configuration du système. (par exemple, 20 pour 20mOhm, 50 pour 50mOhm) -> [Explication de la configuration du système](Docs/sysConfig.md)
4. Les deux tensions affichées sur l'écran sont de 0V
   - Vérifiez l'interrupteur d'alimentation, il doit être en position basse. (Position HAUTE est OFF, position BASSE est ON)

### Crédits

| Bibliothèque | v1.x | v2.x | Lien |
|---------|------|------|------|
| Arduino-Pico Core | ✅ | ✅ | [Arduino-Pico Core (4.x.x)](https://github.com/earlephilhower/arduino-pico) |
| Arduino-ESP32 Core | ✅ | ✅ | [Arduino-ESP32 Core (3.x.x)](https://github.com/espressif/arduino-esp32) |
| INA3221_RT Library | ✅ | ✅ | [INA3221_RT Library](https://github.com/RobTillaart/INA3221_RT/tree/master) |
| Adafruit GFX Library | ✅ |  | [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) |
| Adafruit ST7735 Library | ✅ |  | [Adafruit ST7735 Library](https://github.com/adafruit/Adafruit-ST7735-Library) |
| Adafruit SleepyDog | ✅ |  | [Adafruit SleepyDog](https://github.com/adafruit/Adafruit_SleepyDog) |
| LovyanGFX |  | ✅ | [LovyanGFX](https://github.com/lovyan03/LovyanGFX) |
| LVGL |  | ✅ | [LVGL](https://github.com/lvgl/lvgl) |

| Outils | Lien |
|---------|------|
| Outil MergeBin pour les séries esp32 | [Merci à DavidSchinazi, DunkelRatte](https://github.com/platformio/platform-espressif32/issues/1078#issuecomment-2219671743) |

## Contribuer

Les contributions sont les bienvenues pour améliorer le projet XIAO PowerBread ! Que vous souhaitiez soumettre une pull request, proposer de nouvelles fonctionnalités ou signaler un bug, n'hésitez pas à utiliser le tracker d'issues.

## Licence

Ce projet est sous licence MIT. Voir le fichier [LICENSE](./LICENSE) pour plus de détails.