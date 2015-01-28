#lancer de l'étape 3

echo "lancement des programme de test de l'étape 3"
echo "lancement de userpage0"
./../../build/nachos-step6 -x ./../../build/userpage0
echo "lancement de makethread"
./../../build/nachos-step6 -x ./../../build/makethread
echo "lancement de threadautoexit"
./../../build/nachos-step6 -x ./../../build/threadautoexit
echo "lancement de threadnojoin"
./../../build/nachos-step6 -x ./../../build/threadnojoin
echo "lancement de threaderror"
./../../build/nachos-step6 -x ./../../build/threaderror
echo "fin du lancer de l'étape 3"
echo ""