#lancer de l'étape 4

echo "lancement des programme de test de l'étape 4"
echo "lancement de makeprocess : test de création de processus"
./../../build/nachos-step6 -x ./../../build/makeprocess
echo "lancement de matmult : test de stress de la mémoire virtuelle"
./../../build/nachos-step6 -x ./../../build/matmult
echo "lancement de forkexec : test de la fonction utilisateur forkexec"
./../../build/nachos-step6 -x ./../../build/forkexec
echo "lancement de forkerror : test des retour d'erreur de la fonction forkexec"
./../../build/nachos-step6 -x ./../../build/forkerror
echo "lancement du shell"
./../../build/nachos-step6 -x ./../../build/shell
echo "fin du lancer de l'étape 4"
echo ""