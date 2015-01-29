#copie les fichiers dans le systeme de fchier nachos
#executes les programmes
#remove les fichiers

echo "lancement des programme de test de l'étape 2"
echo "lancement de getchar"
./../../build/nachos-step6 -x ./../../build/getchar < a
echo "lancement de getint"
./../../build/nachos-step6 -x ./../../build/getint < 22473
ceho "lancement de getstring"
./../../build/nachos-step6 -x ./../../build/getstring < papillon
echo "lancement de putchar"
./../../build/nachos-step6 -x ./../../build/putchar
echo "lancement de putint"
./../../build/nachos-step6 -x ./../../build/putint
echo "lancement de putstring"
./../../build/nachos-step6 -x ./../../build/putstring
echo "lancement de la console synchrone avec les fichiers in0, in1, in2, in3"
./../../build/nachos-step6 -sc < in0 > out0
./../../build/nachos-step6 -sc < in1 > out1
./../../build/nachos-step6 -sc < in2 > out2
./../../build/nachos-step6 -sc < in3 > out3
echo "fin du lancer de l'étape 2"
echo ""