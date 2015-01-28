#lancer de l'étape 4

echo "lancement des programme de test de l'étape 4"
echo "lancement de makeprocess"
./../../build/nachos-step6 -x ./../../build/makeprocess -rs 26
echo "lancement de matmult"
./../../build/nachos-step6 -x ./../../build/matmult -rs 26
echo "lancement de forkexec"
./../../build/nachos-step6 -x ./../../build/forkexec -rs 26
echo "lancement de forkerror"
./../../build/nachos-step6 -x ./../../build/forkerror -rs 26
echo "lancement du shell"
./../../build/nachos-step6 -x ./../../build/shell -rs 26
echo "fin du lancer de l'étape 4"
echo ""