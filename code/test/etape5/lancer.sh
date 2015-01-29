#lancer de l'étape 5

echo "lancement des programme de test de l'étape 5"
echo "lancement de mkdir"
./../../build/nachos-final -cp ./../../build/etape5/mkdir a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de rmdir"
./../../build/nachos-final -cp ./../../build/etape5/rmdir a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de mkfile"
./../../build/nachos-final -cp ./../../build/etape5/mkfile a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de rmfile"
./../../build/nachos-final -cp ./../../build/etape5/rmfile a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "lancement de cd"
./../../build/nachos-final -cp ./../../build/etape5/cd a
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a
echo "fin du lancer de l'étape 5"
echo ""