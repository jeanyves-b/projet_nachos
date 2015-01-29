#lancer de l'étape 4

echo "lancement des programme de test de l'étape 4"
echo "lancement de makeprocess"
./../../build/nachos-step6 -cp ./../../build/etape4/makeprocess a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de matmult"
./../../build/nachos-step6 -cp ./../../build/etape4/matmult a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de forkexec"
./../../build/nachos-step6 -cp ./../../build/etape4/forkexec a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de forkerror"
./../../build/nachos-step6 -cp ./../../build/etape4/forkerror a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement du shell"
./../../build/nachos-step6 -cp ./../../build/etape4/shell a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "fin du lancer de l'étape 4"
echo ""