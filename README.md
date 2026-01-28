# 📚 DMPS - Data & Media Processing Studio

**Version :** 1.0  
**Date :** Janvier 2026  
**Auteurs :** KLILA Mohamed et CHAGHAL Mohamed Amine

---

## 🎯 Vue d'ensemble

DMPS (Data & Media Processing Studio) est une application desktop modulaire développée en C++ avec Qt, permettant de charger, traiter et sauvegarder différents types de données (texte, images, tableaux) à travers une architecture de modules interchangeables.

**Technologies :** C++17, Qt 6, CMake, STB

---

## ✨ Fonctionnalités principales

### Implémentées ✅

- **Chargement multi-formats** : Différents fichiers textes, images et deux formats en mode Tabular: `.csv`, `.xlsx`
- **Système de modules** : 35+ modules (texte, image, tableaux) avec activation/désactivation dynamique
- **Configuration des paramètres** : Interface graphique pour configurer chaque module
- **Exécution en pipeline** : Application séquentielle avec gestion d'erreurs
- **Affichage des résultats** : Arborescence des résultats + logs chronologiques
- **Sauvegarde multi-formats** : Export dans différents formats
- **Interface professionnelle** : Docks redimensionnables, raccourcis clavier, thèmes clair/sombre


## 🏗️ Architecture du projet

### Pattern architectural : MVC modifié

```
┌──────────────┐
│      UI      │  MainWindow, Dialogs, Widgets
│  (Qt/QML)    │
└──────┬───────┘
       │
┌──────▼───────┐
│  Controller  │  Orchestration UI ↔ Engine
└──────┬───────┘
       │
┌──────▼───────┐
│    Engine    │  Logique métier, pipeline
└──────┬───────┘
       │
┌──────┴───────┬──────────┬──────────┐
│     Data     │  Modules │ Managers │
└──────────────┴──────────┴──────────┘
```

**Patterns utilisés :** Factory, Strategy, Registry, Adapter, Observer

---

## 📁 Structure des dossiers

```
DMPS/
├── CMakeLists.txt              # Configuration build
│
├── src/
│   ├── core/                   # Types de base (DataType, ModuleResult, etc.)
│   ├── data/                   # Modèles de données (IData, TextData, ImageData, TabularData)
│   ├── module/                 # Modules de traitement (~35 modules)
│   │   ├── IModule.hpp
│   │   ├── text/
│   │   ├── image/
│   │   └── tabular/
│   ├── moduleManager/          # Gestion modules (Registry, Manager)
│   ├── engine/                 # Moteur principal (Engine)
│   ├── dataLoader/             # Chargement fichiers (Loaders + Manager)
│   ├── dataSaver/              # Sauvegarde fichiers (Savers + Manager)
│   ├── controller/             # Contrôleur MVC
│   └── ui/                     # Interface utilisateur
│       ├── MainWindow.ui/hpp/cpp
│       ├── display/            # Adaptateurs affichage (Text, Image, Tabular)
│       ├── dialogs/            # Config modules, paramètres
│       └── widgets/            # Results Explorer, Messages Log, Renderers
│
├── include/                    # Headers externes (STB)
└── build/                      # Dossier de build
```

### Composants principaux

- **Data Layer** : Représentation polymorphe des données (TextData, ImageData, TabularData)
- **Module Layer** : Modules de traitement implémentant `IModule::apply()`
- **Engine** : Orchestrateur central (chargement, exécution pipeline, sauvegarde)
- **Module Management** : Registry (métadonnées + factories) + Manager (cycle de vie)
- **Controller** : Pont UI ↔ Engine
- **UI Layer** : MainWindow + Dialogs + DisplayAdapters + Result Widgets

---

## 🔨 Compilation

### Prérequis

- Qt 6 (ou Qt 5.15+)
- CMake 3.16+
- Compilateur C++17

### Avec Qt Creator (RECOMMANDÉ)

1. **Ouvrir le projet**
   - Lancer Qt Creator
   - File → Open File or Project
   - Sélectionner `CMakeLists.txt`
   - Configure Project

2. **Compiler**
   - Build → Build Project (`Ctrl+B`)

3. **Exécuter**
   - Build → Run (`Ctrl+R`)

### En ligne de commande

```bash
mkdir build && cd build
cmake ..
make -j4
./DMPS
```

---

## 🚀 Exécution et utilisation

### Test rapide

#### **1. Ouvrir un fichier** (`Ctrl+O`)
```
File → Open

Sélectionner un fichier texte, une image ou un fichier CSV/XLSX  
(⚠️ éviter les chemins contenant des caractères accentués ou non ASCII)

→ Le fichier s'affiche dans la fenêtre centrale

Ctrl + molette de la souris : zoom avant / arrière sur la donnée affichée
```

#### **2. Activer des modules** (`Ctrl+M`)
```
Modules → Enable/Disable
→ Cocher les modules désirés (ex: "To Upper", "Grayscale")
→ Sélectionner un module coché avec paramètres
→ Cliquer "Configure" pour ajuster les paramètres
→ OK pour valider
```

#### **3. Appliquer les modules** (`F5`)
```
Modules → Apply Modules
→ Pipeline s'exécute
→ Results Explorer (gauche) : résultats par module
→ Messages Log (bas) : logs d'exécution
→ Fenêtre centrale : donnée modifiée
```

#### **4. Sauvegarder** (`Ctrl+S`)
```
File → Save As
→ Choisir format et emplacement
→ Save
```

#### **5. Quitter** (`Ctrl+Q`)
```
File → Exit
```


### Thèmes

**Settings → Theme → Light / Dark** : Basculer entre thème clair et sombre (sauvegardé automatiquement)


