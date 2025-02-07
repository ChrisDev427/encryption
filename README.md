EncryptFile - Chiffrement et Déchiffrement de fichiers

📌 EncryptFile est un programme en C++ permettant de chiffrer et déchiffrer des fichiers textes grâce à une table de correspondance (mapping).
Il fonctionne en deux passes pour renforcer la sécurité et garantir une obfuscation efficace des données.

L'algorithme repose sur une transformation des caractères via une table de substitution stockée dans mapping.txt,
associant chaque caractère à 2000 valeurs chiffrées possibles, sélectionnées de manière aléatoire.

⚙️ Fonctionnalités
✅ Chiffrement en deux passes avec substitution aléatoire (2000 combinaisons possibles par caractère)
✅ Déchiffrement optimisé avec une table inversée pour plus de rapidité
✅ Affichage d'une barre de progression pour suivre le traitement des fichiers volumineux
✅ Vérification et chargement du mapping pour éviter les doublons
✅ Gestion automatique des fichiers d’entrée et sortie
✅ Fichier Makefile pour une compilation simplifiée

🚀 Fonctionnement

1️⃣ Chiffrement (encrypt)
Lecture du fichier texte source
Première passe : chaque caractère est remplacé par une valeur aléatoire parmi 2000 disponibles
Seconde passe : transformation supplémentaire des données déjà chiffrées
Sauvegarde dans un fichier .encrypted

2️⃣ Déchiffrement (decrypt)
Lecture du fichier .encrypted
Optimisation : chargement de la table de correspondance inversée
Première passe : remplacement des séquences chiffrées par leurs caractères d'origine
Seconde passe : décodage final
Sauvegarde dans un fichier .decrypted

🏎️ Performance et Sécurité

2000 combinaisons possibles par caractère, rendant le texte extrêmement difficile à analyser
Optimisation de la recherche avec une table inversée pour le déchiffrement, accélérant considérablement le processus
Chiffrement pseudo-aléatoire, rendant chaque fichier encrypté unique même avec le même texte source
Séparation en deux passes, augmentant la complexité d’une attaque basée sur l’analyse du texte chiffré.

📂 Utilisation

# Compilation avec Makefile
make

# Chiffrement d’un fichier
./encryptFile monTexte.txt encrypt

# Déchiffrement d’un fichier
./encryptFile monTexte.encrypted decrypt

# Nettoyage des fichiers compilés
make clean

🔧 Fichiers du projet
EncryptFile.hpp / EncryptFile.cpp → Code principal du chiffrement et déchiffrement
utils.hpp / utils.cpp → Fonctions utilitaires (gestion de fichiers, erreurs...)
mapping.txt → Table de substitution utilisée pour la transformation des caractères
