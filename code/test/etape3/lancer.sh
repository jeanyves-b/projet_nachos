#lancer de l'étape 3

echo "lancement des programme de test de l'étape 3"
echo "lancement de userpage0"
./../../build/nachos-step6 -cp ./../../build/etape3/userpage0 a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de makethread"
./../../build/nachos-step6 -cp ./../../build/etape3/makethread a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de threadautoexit"
./../../build/nachos-step6 -cp ./../../build/etape3/threadautoexit a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de threadnojoin"
./../../build/nachos-step6 -cp ./../../build/etape3/threadnojoin a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de threaderror"
./../../build/nachos-step6 -cp ./../../build/etape3/threaderror a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "fin du lancer de l'étape 3"
echo ""