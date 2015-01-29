# ce shell permet de lancer les testeur de chaque étape en une seule fois.

./../build/nachos_step6 -f

echo "## Étape 2 ##"
echo "fichier in0, teste générique et chaine très longue, une grande chaine avec des espaces, ^D, ÿ"
echo "fichier in1, teste une série de petite chaine"
echo "fichier in2, teste des saut de ligne successif"
echo "fichier in3, fichier vide"
echo "fichier getchar, getint, getstring, putchar, putint, putstring testent les fonction du même nom"
./etape2/lancer.sh
read -p "Press [Enter] key to start backup..."

echo "## Étape 3 ##"
echo "userpage0 : fait des userthreadcreate et des userthreadjoins"
echo "makethread : crée des threads et fait des join dessus. fait également des put char et putstring entrelacé"
echo "threadautoexit : teste la terminaison d'un thread sans appeler userthreadexit"
echo "threadnojoin : même teste que makethread sans les joins"
echo "threaderror : teste les erreurs de userthreadjoin et userthreadcreate et fini en essayant de créer 1000 thread pour surcharger le système"
./etape3/lancer.sh
read -p "Press [Enter] key to start backup..."

echo "## Étape 4 ##"
echo "makeprocess : test de base, crée un thread et un process"
echo "shell : lance un shell qui va demander de rentrer des caractères et va essayer de lancer les programmes correspondant"
echo "matmult et sort : ces deux programmes font des opération mathématiques pour surcharger le système de mémoir virtuelle"
echo "forkexec : une fork bombe limité en nombre d'itération"
echo "forkerror : test les possibilités d'erreur du fork"
./etape4/lancer.sh
read -p "Press [Enter] key to start backup..."

echo "## Étape 5 ##"
echo "mkdir : crée un dossier"
echo "rmdir : supprime un dossier"
echo "mkfile : crée un fichier"
echo "rmfile : supprime un fichier"
echo "cd : crée des dossier et se déplace dedans"
./etape5/lancer.sh
read -p "Press [Enter] key to start backup..."

echo "## Étape 6 ##"
./etape6/lancer.sh
