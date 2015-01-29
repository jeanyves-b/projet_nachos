#lancer de l'étape 5

./../../build/nachos-final -f > /dev/null
echo "lancement des programme de test de l'étape 5"
echo "lancement de mkdir"
./../../build/nachos-final -cp ./../../build/etape5/mkdir a  > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a  > /dev/null
echo "lancement de rmdir"
./../../build/nachos-final -cp ./../../build/etape5/rmdir a  > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a  > /dev/null
echo "lancement de mkfile"
./../../build/nachos-final -cp ./../../build/etape5/mkfile a  > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a  > /dev/null
echo "lancement de rmfile"
./../../build/nachos-final -cp ./../../build/etape5/rmfile a  > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a  > /dev/null
echo "lancement de cd"
./../../build/nachos-final -cp ./../../build/etape5/cd a  > /dev/null
./../../build/nachos-final -x a -rs 23
./../../build/nachos-final -r a > /dev/null
echo "fin du lancer de l'étape 5"
echo ""
