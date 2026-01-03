# 🛡️ CodeFighter - Dossier de Réalisation

Ce projet est un jeu de combat au tour par tour développé en **C++** avec la bibliothèque **SFML 2.5.1**.

---

## 🏗️ Méthodologie de Travail (Workflow)

Bien que la consigne impose un rendu sous **Code::Blocks**, notre équipe a mis en place un workflow moderne pour garantir la qualité du code et la fluidité de la collaboration :

* **Développement sous VS Code** : utilisé comme éditeur principal pour sa gestion avancée de Git et sa souplesse.
* **Architecture modulaire** : le projet est divisé en sous-dossiers (`src/models`, `src/views`, `src/states`, etc.). Cela évite les fichiers "four-tout" et facilite le travail en parallèle.
* **Automatisation Python (`tools/`)** : Code::Blocks gérant difficilement l'ajout massif de nouveaux fichiers, nous avons créé le script `update_cbp.py` qui scanne les dossiers et met à jour automatiquement le fichier `.cbp`.
* **Configuration VS Code (`.vscode/`)** : pour une compilation instantanée, nous avons configuré :
    * `tasks.json` : automatise la compilation avec `g++` en incluant tous les sous-dossiers.
    * `c_cpp_properties.json` : configure l'IntelliSense pour la reconnaissance de SFML.

---

## 🚀 Guide d'installation (Nouvelle Machine)

Ce guide permet de lancer le projet sur n'importe quel ordinateur Windows équipé de SFML 2.5.1.

### 1. Ouverture du projet
- Ouvrez **Code::Blocks**.
- Allez dans `File > Open...` et sélectionnez le fichier **`CodeFighters.cbp`** à la racine du projet.

### 2. Configuration des chemins (Linking)
Si SFML n'est pas trouvé par l'IDE :
1. Faites un clic droit sur le projet (explorateur à gauche) > **Build options...**
2. Sélectionnez le **nom du projet** (tout en haut de la liste à gauche).
3. Onglet **Search directories** :
    * Sous-onglet **Compiler** : Ajoutez le chemin vers votre dossier `SFML/include`.
    * Sous-onglet **Linker** : Ajoutez le chemin vers votre dossier `SFML/lib`.
4. Cliquez sur **OK**.

### 3. Compilation et Lancement
- Appuyez sur **F9** (Build and Run).
- **Note** : les fichiers `.dll` nécessaires sont déjà présents dans `bin/Debug`. En cas de changement de configuration, assurez-vous que les DLL de SFML sont bien à côté de l'exécutable généré.

---

## 📂 Organisation des fichiers

* **`src/` & `include/`** : Code source et en-têtes, organisés par responsabilités (MVC).
* **`bin/Debug/`** : Contient l'exécutable Windows et les DLL nécessaires.
* **`assets/`** : Ressources graphiques et sonores.
* **`tools/`** : Scripts Python pour la maintenance du projet Code::Blocks.
* **`.vscode/`** : Configuration pour le développement sous Visual Studio Code.

---

## 🧩 Design Patterns utilisés

* **Pattern Commande** : Encapsule les attaques (`PythonStrike`, `JavaException`) pour les rendre interchangeables.
* **Pattern State** : Gère la logique des différents écrans (Menu -> Sélection -> Combat).
* **Flyweight (ResourceManager)** : Optimise l'utilisation de la mémoire vive en ne chargeant les textures et polices qu'une seule fois.

---

## ❓ Pourquoi ce système ?

Cette méthode nous a permis de ne jamais être bloqués par l'IDE. Si Code::Blocks est instable sur une machine, nous pouvons compiler en une seconde via **VS Code**. Pour le rendu final, le fichier `.cbp` est généré proprement par notre script Python. C'est une approche **"Agile"** appliquée à un projet académique.