#copie les fichiers dans le systeme de fchier nachos
#executes les programmes
#remove les fichiers

./../../build/nachos-final -f > /dev/null
echo -e "Lancement de \033[33;1mgetchar\033[0m, entrez un caractère"
./../../build/nachos-final -cp ./../../build/etape2/getchar a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
echo -e "Lancement de \033[33;1mgetint\033[0m, entrez un entier"
./../../build/nachos-final -cp ./../../build/etape2/getint a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
echo -e "Lancement de \033[33;1mgetstring\033[0m, entrez un caractère"
./../../build/nachos-final -cp ./../../build/etape2/getstring a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
echo -e "Appuyer sur [Enter] pour lancer \033[33;1mputchar\033[0m:"; read;
./../../build/nachos-final -cp ./../../build/etape2/putchar a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
echo -e "Appuyer sur [Enter] pour lancer \033[33;1mputint\033[0m:"; read;
./../../build/nachos-final -cp ./../../build/etape2/putint a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
echo -e "Appuyer sur [Enter] pour lancer \033[33;1mputstring\033[0m:"; read;
./../../build/nachos-final -cp ./../../build/etape2/putstring a > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null

echo "lancement de la console synchrone avec les fichiers in0, in1, in2, in3...";
./../../build/nachos-final -sc < in0.txt > out0.txt 
./../../build/nachos-final -sc < in1.txt > out1.txt
./../../build/nachos-final -sc < in2.txt > out2.txt 
./../../build/nachos-final -sc < in3.txt > out3.txt
echo
