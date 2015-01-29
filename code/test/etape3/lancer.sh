#lancer de l'étape 3

echo "lancement des programme de test de l'étape 3"
echo "lancement de userpage0"
./../../build/nachos-final -cp ./../../build/etape3/userpage0 a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de makethread"
./../../build/nachos-final -cp ./../../build/etape3/makethread a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de threadautoexit"
./../../build/nachos-final -cp ./../../build/etape3/threadautoexit a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de threadnojoin"
./../../build/nachos-final -cp ./../../build/etape3/threadnojoin a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de threaderror"
./../../build/nachos-final -cp ./../../build/etape3/threaderror a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "fin du lancer de l'étape 3"
echo ""