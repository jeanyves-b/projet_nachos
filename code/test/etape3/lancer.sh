#lancer de l'étape 3

./../../build/nachos-final -f > /dev/null
echo -e "Appuyer sur [Enter] pour lancer un \033[33;1maffichage entrelacé\033[0m de caractères \033[33;1mpar deux threads\033[0m (un thread affiche des lettres et l'autre des chiffres): "; read;
./../../build/nachos-final -cp ./../../build/user0 a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
echo -e "Appuyer sur [Enter] pour lancer une \033[33;1mun thread sans le terminer explicitement\033[0m: "; read;
./../../build/nachos-final -cp ./../../build/etape3/threadsautoexit a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
echo -e "Appuyer sur [Enter] pour lancer plusieurs threads \033[33;1msans join entre eux\033[0m (les threads lancés se terminent même si le main est déjà fini): "; read;
./../../build/nachos-final -cp ./../../build/etape3/threadsnojoin a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
echo -e "Appuyer sur [Enter] pour lancer un test des \033[33;1mcodes d'erreur'\033[0m: "; read;
./../../build/nachos-final -cp ./../../build/etape3/threaderror a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
echo "fin du lancer de l'étape 3"
echo ""
