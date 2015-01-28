#lancer de l'étape 4

echo "lancement des programme de test de l'étape 4"
echo "lancement de makeprocess"
./../../build/nachos-step6 -x -rs ./../../build/makeprocess
echo "lancement de matmult"
./../../build/nachos-step6 -x -rs ./../../build/matmult
echo "lancement de forkexec"
./../../build/nachos-step6 -x -rs ./../../build/forkexec
echo "lancement de forkerror"
./../../build/nachos-step6 -x -rs ./../../build/forkerror
echo "lancement du shell"
./../../build/nachos-step6 -x -rs ./../../build/shell
echo "fin du lancer de l'étape 4"
echo ""