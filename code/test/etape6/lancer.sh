#!/bin/bash
# init

./../../build/nachos-final -f > /dev/null
echo -e "	Lancement de test en \033[33;1manneau de taille 8\033[0m"
./../../build/nachos-final -cp ./../../build/etape6/ring_0 r0 > /dev/null
./../../build/nachos-final -cp ./../../build/etape6/ring_i ri > /dev/null
./../../build/nachos-final -cp ./../../build/etape6/ring_last rn > /dev/null
gnome-terminal --title="Membre 7 de l'anneau (dernier)" -e "./../../build/nachos-final -m 7 -rs 7 -x rn -d z"
gnome-terminal --title="Membre 6 de l'anneau" -e "./../../build/nachos-final -m 6 -rs 7 -x ri -d z"
gnome-terminal --title="Membre 5 de l'anneau" -e "./../../build/nachos-final -m 5 -rs 7 -x ri -d z"
gnome-terminal --title="Membre 4 de l'anneau" -e "./../../build/nachos-final -m 4 -rs 7 -x ri -d z"
gnome-terminal --title="Membre 3 de l'anneau" -e "./../../build/nachos-final -m 3 -rs 7 -x ri -d z"
gnome-terminal --title="Membre 2 de l'anneau" -e "./../../build/nachos-final -m 2 -rs 7 -x ri -d z"
gnome-terminal --title="Membre 1 de l'anneau" -e "./../../build/nachos-final -m 1 -rs 7 -x ri -d z"
./../../build/nachos-final -m 0 -rs 7 -x r0  -d z
./../../build/nachos-final -r r0 > /dev/null
./../../build/nachos-final -r ri > /dev/null
./../../build/nachos-final -r rn > /dev/null
echo -e "Appuyer sur [Entrée] pour passer au test \033[33;1md'envoi de fichier de 3750 octets\033[0m"; read;
./../../build/nachos-final -cp ./../../build/etape6/filesend fileS > /dev/null
./../../build/nachos-final -cp ./../../build/etape6/filereceive fileR > /dev/null
./../../build/nachos-final -cp ./../../build/etape6/initnetwork initN > /dev/null
./../../build/nachos-final -x initN > /dev/null
./../../build/nachos-final -cp ./../../build/00_tiny network/outbox/00_tiny > /dev/null
gnome-terminal -e "./../../build/nachos-final -m 0 -x fileS -rs 7"
./../../build/nachos-final -m 1 -x fileR -rs 7  -d z;
./../../build/nachos-final -r fileR > /dev/null
./../../build/nachos-final -r fileS > /dev/null
./../../build/nachos-final -r initN > /dev/null
echo "		Le fichier est bien présent dans le bon repértoire"
./../../build/nachos-final -cd "network/inbox"
read -p "	Appuyer pour passer afficher le contenu du fichier reçu..."
./../../build/nachos-final -p "network/inbox/01_R"
echo "__ Fin des tests de l'étape 6 __"
echo
