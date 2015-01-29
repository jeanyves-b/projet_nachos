#copie les fichiers dans le systeme de fchier nachos
#executes les programmes
#remove les fichiers

echo "lancement des programme de test de l'étape 2"
echo "lancement de getchar"
./../../build/nachos-step6 -cp ./../../build/etape2/getchar  a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de getint"
./../../build/nachos-step6 -cp ./../../build/etape2/getint  a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
ceho "lancement de getstring"
./../../build/nachos-step6 -cp ./../../build/etape2/getstring  a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de putchar"
./../../build/nachos-step6 -cp ./../../build/etape2/putchar  a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de putint"
./../../build/nachos-step6 -cp ./../../build/etape2/putint  a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de putstring"
./../../build/nachos-step6 -cp ./../../build/etape2/putstring  a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de la console synchrone avec les fichiers in0, in1, in2, in3"
./../../build/nachos-step6 -cp in0 in0
./../../build/nachos-step6 -sc in0 out0
./../../build/nachos-step6 -r in0
./../../build/nachos-step6 -cp in1 in1
./../../build/nachos-step6 -sc in1 out1
./../../build/nachos-step6 -r in1
./../../build/nachos-step6 -cp in2 in2
./../../build/nachos-step6 -sc in2 out2
./../../build/nachos-step6 -r in2
./../../build/nachos-step6 -cp in3 in3
./../../build/nachos-step6 -sc in3 out3
./../../build/nachos-step6 -r in3
echo "fin du lancer de l'étape 2"
echo ""