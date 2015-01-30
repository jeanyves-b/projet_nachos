#lancer de l'étape 4

./../../build/nachos-final -f > /dev/null
echo -e "Appuyer sur [Enter] pour lancer \033[33;1mdeux processus en même temps\033[0m: "; read;
./../../build/nachos-final -cp ./../../build/user1 user1 > /dev/null
./../../build/nachos-final -cp ./../../build/user2 user2 > /dev/null
./../../build/nachos-final -cp ./../../build/etape4/makeprocess a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
./../../build/nachos-final -r user1 > /dev/null
./../../build/nachos-final -r user2 > /dev/null
echo -e "Appuyer sur [Enter] pour lancer \033[33;1mun programme de multiplication mathétique gourmand en ressources\033[0m: "; read;
./../../build/nachos-final -cp ./../../build/etape4/matmult a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
echo -e "Appuyer sur [Enter] pour lancer \033[33;1mun grand nombre de processus en même temps\033[0m: "; read;
./../../build/nachos-final -cp ./../../build/user0 user0 > /dev/null
./../../build/nachos-final -cp ./../../build/etape4/forkexec a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
./../../build/nachos-final -r user0 > /dev/null
echo -e "Appuyer sur [Enter] pour lancer un test sur \033[33;1mvaleurs d'erreur'\033[0m: "; read;
./../../build/nachos-final -cp ./../../build/etape4/forkerreur a > /dev/null
./../../build/nachos-final -cp ./../../build/etape4/matmult nothing > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
./../../build/nachos-final -r nothing > /dev/null
echo "fin du lancer de l'étape 4"
echo ""
