#lancer de l'étape 4

echo "lancement des programme de test de l'étape 4"
echo "lancement de makeprocess"
./../../build/nachos-final -cp ./../../build/etape4/makeprocess a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de matmult"
./../../build/nachos-final -cp ./../../build/etape4/matmult a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de forkexec"
./../../build/nachos-final -cp ./../../build/etape4/forkexec a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de forkerror"
./../../build/nachos-final -cp ./../../build/etape4/forkerror a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement du shell"
./../../build/nachos-final -cp ./../../build/etape4/shell a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "fin du lancer de l'étape 4"
echo ""