# firmware : sketchs des TP sur NeuroBoard-S3

Projet PlatformIO, un environnement par sketch (`src/<nom>/`). Configuration de la carte reprise de la doc
NeuroBoard : pioarduino (Arduino-ESP32 3.x), flash 16 Mo, PSRAM octale, `Serial` sur l'USB natif.

Les sketchs `tp1_*` sont des squelettes : ils compilent et affichent leurs mesures, mais les lignes marquées
`TODO` sont à écrire pendant le TP 1. `check_board`, `cam_capture` et `tools/serial_tool.py` sont des outils
complets.

## Installation

pioarduino 55.03 demande PlatformIO Core 6.2 ou plus récent. Un venv dédié évite les conflits avec une
installation PlatformIO plus ancienne :

```bash
uv venv --python 3.13 .piovenv
uv pip install --python .piovenv/Scripts/python.exe "platformio==6.2.0"
.piovenv/Scripts/pio run -e check_board
```

Premier build : environ 6 minutes (téléchargement de la toolchain). Les suivants : environ 2 minutes.

## Sketchs

| Environnement | Rôle | Branchements |
| --- | --- | --- |
| `check_board` | flash, PSRAM, version du core ; LED RGB qui change de couleur | aucun |
| `tp1_timer_arduino` | alarme timer à 16 kHz (API Arduino), compte les alarmes par seconde | oscilloscope sur GPIO21 |
| `tp1_gptimer` | GPTimer à 8, 16, 32, 44.1, 48 kHz, 5 s chacune : visée, calculée, mesurée | oscilloscope sur GPIO21 |
| `tp1_register_read` | compteur du timer lu par registre (`TIMERG0`) comparé à `gptimer_get_raw_count()` | aucun |
| `tp1_paced_adc` | durée de `analogRead()`, puis gigue et alarmes perdues à 8, 16, 48 kHz | potentiomètre ou GBF sur GPIO1 |
| `tp1_plotter` | buffers de 1 024 échantillons à 8 kHz vers le traceur série | GBF sur GPIO1 (0 à 3.3 V) |
| `cam_capture` | images de la caméra vers le PC (`tools/serial_tool.py capture`) | caméra |

Broches (voir `include/neuroboard.h`) : caméra branchée, seules GPIO1 et GPIO2 restent des entrées ADC1 libres ;
la LED RGB est sur GPIO38 (pas GPIO48 comme sur la DevKitC-1).

## Outil série (PC)

```bash
python tools/serial_tool.py ports
python tools/serial_tool.py log -p COMx -t 20 -o mesures.txt
python tools/serial_tool.py capture -p COMx -n 10 -o captures --format g
```

Dépendances : `pyserial`, `numpy`, `pillow` (présentes dans le venv de `edge-ai-toolchain`).

## Checklist du premier essai sur carte

1. `pio run -e check_board -t upload`, puis `log` : attendu 16 Mo de flash, 8 Mo de PSRAM, LED qui tourne.
   Si la carte n'est pas détectée : maintenir BOOT, appuyer sur RST, relâcher BOOT.
2. `tp1_gptimer` : la fréquence mesurée doit suivre la fréquence calculée (43 478 Hz pour 44.1 kHz).
3. `tp1_register_read` : les deux colonnes doivent croître ensemble. Sinon, le driver a pris un autre timer.
4. `tp1_paced_adc` : relever la durée de `analogRead()` et la fréquence à partir de laquelle des alarmes sont
   perdues ; ces valeurs fixent les questions E9 à E11 du TP 1.
5. `cam_capture` : `capture --format j` puis `--format g`. Image noire ou brouillée : vérifier le brochage ;
   couleurs fausses en RGB565 : l'ordre des octets dans `serial_tool.py` est à inverser.
6. Le message `tool-esptoolpy does not appear to be a Python project` au build est sans conséquence :
   `esptool` est installé dans l'environnement Python de PlatformIO. À vérifier au premier upload.
