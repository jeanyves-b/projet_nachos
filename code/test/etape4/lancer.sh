#lancer de l'étape 4

./../../build/nachos-final -f > /dev/null
echo "lancement des programme de test de l'étape 4"
echo "lancement de makeprocess"
./../../build/nachos-final -cp ./../../build/etape4/makeprocess a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de matmult"
./../../build/nachos-final -cp ./../../build/etape4/matmult a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de forkexec"
./../../build/nachos-final -cp ./../../build/etape4/forkexec a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de forkerreur"
./../../build/nachos-final -cp ./../../build/etape4/forkerreur a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "fin du lancer de l'étape 4"
echo ""
