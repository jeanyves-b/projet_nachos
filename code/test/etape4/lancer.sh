#lancer de l'étape 4

echo "lancement des programme de test de l'étape 4"
echo "lancement de makeprocess"
./../../build/nachos-step6 -x ./../../build/makeprocess
echo "lancement de matmult"
./../../build/nachos-step6 -x ./../../build/matmult
echo "lancement de forkexec"
./../../build/nachos-step6 -x ./../../build/forkexec
echo "lancement de forkerror"
./../../build/nachos-step6 -x ./../../build/forkerror
echo "lancement du shell"
./../../build/nachos-step6 -x ./../../build/shell
echo "fin du lancer de l'étape 4"
echo ""