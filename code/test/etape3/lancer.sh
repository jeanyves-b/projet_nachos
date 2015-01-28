#lancer de l'étape 3

echo "lancement des programme de test de l'étape 3"
echo "lancement de userpage0"
./../../build/nachos-step6 -x ./../../build/userpage0 -rs 23
echo "lancement de makethread"
./../../build/nachos-step6 -x ./../../build/makethread -rs 23
echo "lancement de threadautoexit"
./../../build/nachos-step6 -x ./../../build/threadautoexit -rs 23
echo "lancement de threadnojoin"
./../../build/nachos-step6 -x ./../../build/threadnojoin -rs 23
echo "lancement de threaderror"
./../../build/nachos-step6 -x ./../../build/threaderror -rs 23
echo "fin du lancer de l'étape 3"
echo ""