# ce shell permet de lancer les testeur de chaque étape en une seule fois.

echo -e "\033[31;1m## Étape 2 ##\033[0m"
echo "Fichier in0, teste générique et chaine très longue, une grande chaine avec des espaces, ^D, ÿ"
echo "Fichier in1, teste une série de petite chaine"
echo "Fichier in2, teste des saut de ligne successif"
echo "Fichier in3, fichier vide"
echo "Fichier getchar, getint, getstring, putchar, putint, putstring testent les fonction du même nom"
echo
echo -e "Appuyer sur [Enter] pour lancer les tests de l'\033[31;1métape 2\033[0m.."; read;
cd etape2
./lancer.sh
cd ..
echo
echo -e "Appuyer sur [Enter] pour passer à l'\033[31;1métape 3\033[0m.."; read;

echo -e "\033[31;1m## Étape 3 ##\033[0m"
echo "userpage0 : fait des userthreadcreate et des userthreadjoins"
echo "makethread : crée des threads et fait des join dessus. fait également des put char et putstring entrelacé"
echo "threadautoexit : teste la terminaison d'un thread sans appeler userthreadexit"
echo "threadnojoin : même teste que makethread sans les joins"
echo "threaderror : teste les erreurs de userthreadjoin et userthreadcreate et fini en essayant de créer 1000 thread pour surcharger le système"
echo
echo -e "Appuyer sur [Enter] pour lancer les tests de l'\033[31;1métape 3\033[0m.."; read;
cd etape3
./lancer.sh
cd ..
echo
echo -e "Appuyer sur [Enter] pour passer à l'\033[31;1métape 4\033[0m.."; read;

echo -e "\033[31;1m## Étape 4 ##\033[0m"
echo "makeprocess : test de base, crée un thread et un process"
echo "shell : lance un shell qui va demander de rentrer des caractères et va essayer de lancer les programmes correspondant"
echo "matmult et sort : ces deux programmes font des opération mathématiques pour surcharger le système de mémoir virtuelle"
echo "forkexec : une fork bombe limité en nombre d'itération"
echo "forkerror : test les possibilités d'erreur du fork"
echo
echo -e "Appuyer sur [Enter] pour lancer les tests de l'\033[31;1métape 4\033[0m.."; read;
cd etape4
./lancer.sh
cd ..
echo
echo -e "Appuyer sur [Enter] pour passer à l'\033[31;1métape 6\033[0m.."; read;
clear
read
read

#echo -e "\033[31;1m## Étape 5 ##\033[0m"
#echo "mkdir : crée un dossier"
#echo "rmdir : supprime un dossier"
#echo "mkfile : crée un fichier"
#echo "rmfile : supprime un fichier"
#echo "cd : crée des dossier et se déplace dedans"
#echo
#echo -e "Appuyer sur [Enter] pour lancer les tests de l'\033[31;1métape 5\033[0m.."; read;
#cd etape5
#./lancer.sh
#cd ..
#echo
#echo -e "Appuyer sur [Enter] pour passer à l'\033[31;1métape 6\033[0m.."; read;

echo -e "\033[31;1m## Étape 6 ##\033[0m"
cd etape6
./lancer.sh
cd ..
