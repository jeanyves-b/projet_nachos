#lancer de l'étape 5

./../../build/nachos-final -f > /dev/null
echo -e "Lancement de \033[33;1mMKDIR\033[0m: "
./../../build/nachos-final -cp ./../../build/etape5/mkdir a  > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a  > /dev/null
echo -e "Lancement de \033[33;1mRMDIR\033[0m: "
./../../build/nachos-final -cp ./../../build/etape5/rmdir a  > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a  > /dev/null
echo -e "Lancement de \033[33;1mMKFILE\033[0m: "
./../../build/nachos-final -cp ./../../build/etape5/mkfile a  > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a  > /dev/null
echo -e "Lancement de \033[33;1mRMFILE\033[0m: "
./../../build/nachos-final -cp ./../../build/etape5/rmfile a  > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a  > /dev/null
echo -e "Lancement de \033[33;1mCD\033[0m: "
./../../build/nachos-final -cp ./../../build/etape5/cd a  > /dev/null
./../../build/nachos-final -x a -rs 23 -d z
./../../build/nachos-final -r a > /dev/null
echo 
