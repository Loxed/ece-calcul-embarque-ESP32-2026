## Structure

```
tp2/
├── kernels.py        # définition des kernels de convolution
├── filters.py         # logique de traitement (kernels + shifts + pipeline)
├── display.py          # affichage matplotlib
├── main.py             # point d'entrée
└── requirements.txt
```

## Prérequis

- Python 3.9 ou plus récent
- `python3 --version` (Linux/Mac/WSL) ou `python --version` (Windows) pour vérifier

---

## Installation

### Windows — Invite de commandes (cmd.exe)

```bat
cd chemin\vers\tp2

:: Créer l'environnement virtuel
python -m venv venv

:: Activer l'environnement
venv\Scripts\activate.bat

:: Installer les dépendances
pip install -r requirements.txt
```

### Windows — PowerShell

```powershell
cd chemin\vers\tp2

# Créer l'environnement virtuel
python -m venv venv

# Activer l'environnement
venv\Scripts\Activate.ps1

# Installer les dépendances
pip install -r requirements.txt

#  Si PowerShell bloque l'activation (erreur de politique d'exécution), lancer une fois :
Set-ExecutionPolicy -Scope CurrentUser RemoteSigned
```


### WSL / Linux / macOS (bash ou zsh)

```bash
cd chemin/vers/tp2

# Créer l'environnement virtuel
python3 -m venv venv

# Activer l'environnement
source venv/bin/activate

# Installer les dépendances
pip install -r requirements.txt
```
---

## Utilisation

Une fois l'environnement activé et les dépendances installées, placer une image
(ex: `kirby.png`) dans le dossier `tp2/`, puis lancer :

```bash
python main.py        
```

Le script exécute deux démonstrations :
1. Une seule opération (`flou`) affichée en avant/après.
2. Un pipeline de plusieurs opérations enchaînées (`flou` → `netteté_forte` →
   `red_shift` → `edges`), avec toutes les étapes intermédiaires affichées côte à côte.

Pour personnaliser, éditer `main.py` ou importer les fonctions ailleurs :

```python
from main import run

# une opération
run("mon_image.png", ["edges"])

# pipeline enchaîné
run("mon_image.png", ["flou_fort", "sobel_horizontal"], show_all_steps=True)

# avec paramètres sur une opération
run("mon_image.png", [("red_shift", {"amount": 80}), "netteté"])
```

Kernels et opérations disponibles :

```python
from filters import OPERATIONS
print(sorted(OPERATIONS))
```

---

## Désactiver l'environnement virtuel

```bash
deactivate
```

(fonctionne pareil sur Windows, WSL, Linux et Mac)