#copie les fichiers dans le systeme de fchier nachos
#executes les programmes
#remove les fichiers

./../../build/nachos-final -f > /dev/null
./../../build/nachos-final -init > /dev/null
echo "lancement des programme de test de l'étape 2"
echo "lancement de getchar"
./../../build/nachos-final -cp ./../../build/etape2/getchar a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de getint"
./../../build/nachos-final -cp ./../../build/etape2/getint a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de getstring"
./../../build/nachos-final -cp ./../../build/etape2/getstring a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de putchar"
./../../build/nachos-final -cp ./../../build/etape2/putchar a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de putint"
./../../build/nachos-final -cp ./../../build/etape2/putint a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de putstring"
./../../build/nachos-final -cp ./../../build/etape2/putstring a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de la console synchrone avec les fichiers in0, in1, in2, in3"
./../../build/nachos-final -sc < in0.txt > out0.txt
./../../build/nachos-final -sc < in1.txt > out1.txt
./../../build/nachos-final -sc < in2.txt > out2.txt
./../../build/nachos-final -sc < in3.txt > out3.txt
echo "fin du lancer de l'étape 2"
echo ""