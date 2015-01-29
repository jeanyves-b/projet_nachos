#lancer de l'étape 5

echo "lancement des programme de test de l'étape 5"
echo "lancement de mkdir"
./../../build/nachos-step6 -cp ./../../build/etape5/mkdir a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de rmdir"
./../../build/nachos-step6 -cp ./../../build/etape5/rmdir a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de mkfile"
./../../build/nachos-step6 -cp ./../../build/etape5/mkfile a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de rmfile"
./../../build/nachos-step6 -cp ./../../build/etape5/rmfile a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "lancement de cd"
./../../build/nachos-step6 -cp ./../../build/etape5/cd a
./../../build/nachos-step6 -x a -rs 23
./../../build/nachos-step6 -r a
echo "fin du lancer de l'étape 5"
echo ""