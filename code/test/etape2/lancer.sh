#copie les fichiers dans le systeme de fchier nachos
#executes les programmes
#remove les fichiers

echo "lancement des programme de test de l'étape 2"
./../../build/nachos-step6 -x ./../../build/getchar < a
./../../build/nachos-step6 -x ./../../build/getint < 22473
./../../build/nachos-step6 -x ./../../build/getstring < papillon
./../../build/nachos-step6 -x ./../../build/putchar
./../../build/nachos-step6 -x ./../../build/putint
./../../build/nachos-step6 -x ./../../build/putstring
./../../build/nachos-step6 -sc < in0 > out0
./../../build/nachos-step6 -sc < in1 > out1
./../../build/nachos-step6 -sc < in2 > out2
./../../build/nachos-step6 -sc < in3 > out3
echo "fin du lancer de l'étape 2"
echo ""