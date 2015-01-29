#copie les fichiers dans le systeme de fchier nachos
#executes les programmes
#remove les fichiers

echo "lancement des programme de test de l'étape 2"
echo "lancement de getchar"
./../../build/nachos-final -cp ./../../build/etape2/getchar  a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de getint"
./../../build/nachos-final -cp ./../../build/etape2/getint  a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
ceho "lancement de getstring"
./../../build/nachos-final -cp ./../../build/etape2/getstring  a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de putchar"
./../../build/nachos-final -cp ./../../build/etape2/putchar  a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de putint"
./../../build/nachos-final -cp ./../../build/etape2/putint  a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de putstring"
./../../build/nachos-final -cp ./../../build/etape2/putstring  a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de la console synchrone avec les fichiers in0, in1, in2, in3"

#./../../build/nachos-final -cp in0 in0
./../../build/nachos-final -sc < in0.txt > out0.txt
#./../../build/nachos-final -r in0
./../../build/nachos-final -cp in1 in1
./../../build/nachos-final -sc in1 out1
./../../build/nachos-final -r in1
./../../build/nachos-final -cp in2 in2
./../../build/nachos-final -sc in2 out2
./../../build/nachos-final -r in2
./../../build/nachos-final -cp in3 in3
./../../build/nachos-final -sc in3 out3
./../../build/nachos-final -r in3
echo "fin du lancer de l'étape 2"
echo ""
