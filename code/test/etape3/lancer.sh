#lancer de l'étape 3

./../../build/nachos-final -f > /dev/null
./../../build/nachos-final -init > /dev/null
echo "lancement des programme de test de l'étape 3"
echo "lancement de userpage0"
./../../build/nachos-final -cp ./../../build/etape3/userpages0 a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de makethread"
./../../build/nachos-final -cp ./../../build/etape3/makethreads a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de threadautoexit"
./../../build/nachos-final -cp ./../../build/etape3/threadsautoexit a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de threadnojoin"
./../../build/nachos-final -cp ./../../build/etape3/threadsnojoin a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "lancement de threaderror"
./../../build/nachos-final -cp ./../../build/etape3/threaderror a > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "fin du lancer de l'étape 3"
echo ""
