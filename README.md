Voici quelques explications concernant notre projet :

******* COMPILATION ********

Pour compiler notre projet et créer l'exécutable associé, on commence par nettoyer les fichiers de la compilation précédente avec la commande : 

'make clean'

puis on compile avec la commande : 

'make'

On peut préciser à ce moment les dimensions que l'on veut donner au plateau et le nombre de ressources avec lequel on joue :

'make MAX_X=9 MAX_X=8 NUM_RESOURCES=7'   va créer un plateau de taille 9x8 et défini le nombre de ressources jouables à 7

On execute alors le jeu avec la commande : 

'./project'

******* OPTIONS ********

Il est possble d'ajouter des options à la ligne d'exécution de notre projet pour spécifier plusieurs choses:
-p x : précise qu'on jouera avec x joueurs
-c n : défini le mode de jeu à n
-s d : défini la seed à d
-v : affichage complet (affiche le plateau à chaque action d'un joueur, sinon n'affiche qu'un résumé du jeu)

exemple de commande d'exécution : 

'./project -p 2 -c 3' : execute le jeu avec deux joueurs dans le mode de jeu numéro 3 (plateau infernal) avec un affichage réduit

******** TEST ********

Il est aussi possible de tester les différents fichiers de notre projet avec la commande :

'make test' qui compile et lance tous nos fichiers de test ou alors avec la commande :

'make' puis en exécutant les fichiers de test de manière indépendantes , exemple : './test_building' ou './test_mine'