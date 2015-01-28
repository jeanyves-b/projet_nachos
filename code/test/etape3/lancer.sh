#lancer de l'étape 3

echo "lancement des programme de test de l'étape 3"
echo "lancement de userpage0"
./../../build/nachos-step6 -x -rs ./../../build/userpage0
echo "lancement de makethread"
./../../build/nachos-step6 -x -rs ./../../build/makethread
echo "lancement de threadautoexit"
./../../build/nachos-step6 -x -rs ./../../build/threadautoexit
echo "lancement de threadnojoin"
./../../build/nachos-step6 -x -rs ./../../build/threadnojoin
echo "lancement de threaderror"
./../../build/nachos-step6 -x -rs ./../../build/threaderror
echo "fin du lancer de l'étape 3"
echo ""