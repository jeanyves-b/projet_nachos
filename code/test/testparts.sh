# ce shell permet de lancer les testeur de chaque étape en une seule fois.

echo "## Étape 2 ##"
echo "Fichier in0, teste générique et chaine très longue, une grande chaine avec des espaces, ^D, ÿ"
echo "Fichier in1, teste une série de petite chaine"
echo "Fichier in2, teste des saut de ligne successif"
echo "Fichier in3, fichier vide"
echo "Fichier getchar, getint, getstring, putchar, putint, putstring testent les fonction du même nom"
echo
read -p "Appuyer sur [Enter] pour commencer les tests.."
cd etape2
./lancer.sh
cd ..
echo
read -p "Appuyer sur [Enter] pour passer à l'étape suivate.."

echo "## Étape 3 ##"
echo "userpage0 : fait des userthreadcreate et des userthreadjoins"
echo "makethread : crée des threads et fait des join dessus. fait également des put char et putstring entrelacé"
echo "threadautoexit : teste la terminaison d'un thread sans appeler userthreadexit"
echo "threadnojoin : même teste que makethread sans les joins"
echo "threaderror : teste les erreurs de userthreadjoin et userthreadcreate et fini en essayant de créer 1000 thread pour surcharger le système"
echo
read -p "Appuyer sur [Enter] pour commencer les tests.."
cd etape3
./lancer.sh
cd ..
echo
read -p "Appuyer sur [Enter] pour passer à l'étape suivate.."

echo "## Étape 4 ##"
echo "makeprocess : test de base, crée un thread et un process"
echo "shell : lance un shell qui va demander de rentrer des caractères et va essayer de lancer les programmes correspondant"
echo "matmult et sort : ces deux programmes font des opération mathématiques pour surcharger le système de mémoir virtuelle"
echo "forkexec : une fork bombe limité en nombre d'itération"
echo "forkerror : test les possibilités d'erreur du fork"
echo
read -p "Appuyer sur [Enter] pour commencer les tests.."
cd etape4
./lancer.sh
cd ..
echo
read -p "Appuyer sur [Enter] pour passer à l'étape suivate.."

echo "## Étape 5 ##"
echo "mkdir : crée un dossier"
echo "rmdir : supprime un dossier"
echo "mkfile : crée un fichier"
echo "rmfile : supprime un fichier"
echo "cd : crée des dossier et se déplace dedans"
echo
read -p "Appuyer sur [Enter] pour commencer les tests.."
cd etape5
./lancer.sh
cd ..
echo
read -p "Appuyer sur [Enter] pour passer à l'étape suivate.."

echo "## Étape 6 ##"
cd etape6
./lancer.sh
cd ..
