#include <stdio.h>
#include <stdlib.h>
#define INFINITY 9999
#define MAX 500 // taille maximale du graph (à changer)

#define typeof(var) _Generic((var), \
                int: "int", \
                float: "float", \
                double: "double", \
                char: "char", \
                default: "unknown")


void algo_A(int** Graph, int n, int start, int end);
void printShortestPath(int pred[MAX], int start, int end);
int** matrice_adjacence(int n);
void affichier_matrice_adjacence(int** matrice, int size);
int menu(int** matrice, int size);
int CheckSize(int size);
int count_pred(int** matrice, int noeud, int size);
int* show_pred(int** matrice, int noeud, int n);
int** ImportMatrice(char* fichier, int* size);

/**
 * algo_A: fonction qui trouve le plus cours chemin entre deux sommet en utilisant l'algorithm A
 * @Graph: le graph etudie
 * @n: nombre des noeds dans le grph etudie
 * @start: le noeud ou on va commencer
 * @end: le noeud ou on va arreter
 * Retun: la fonction ne retourne rien .(Elle affiche le cours chemin)
*/
void algo_A(int** Graph, int n, int start, int end)
{
    int cost[MAX][MAX], distance[MAX], pred[MAX], visited[MAX], count, mindistance, nextnode, i, j;
    /**
     * cost: le cout des arretes
     * distances: la distance entre le noeud source et tous les noeuds du graph (tableau)
     * mindistance: la distance minimal trouvee a chaque iteration
     * nextnode: le prochain noeud a visiter
     * pred: tableau qui contient les predeccesseurs de chaque noeud
    */

    // creation de la matrice "cost"
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (Graph[i][j] == 0)
                cost[i][j] = INFINITY;
            else
                cost[i][j] = Graph[i][j];

    for (i = 0; i < n; i++) 
    {
        distance[i] = cost[start][i]; //saisir la distance initial entre le neoud source et les autres noeuds
        pred[i] = start; //Définissez les prédécesseurs initiaux pour chaque sommet sur le sommet source
        visited[i] = 0; //marquer tous les noeuds comme non visite
    }

    distance[start] = 0;
    visited[start] = 1;
    count = 1;

    while (count < n - 1) 
    {

        mindistance = INFINITY;

        for (i = 0; i < n; i++) //Boucle imbriquée pour trouver le sommet avec la distance minimale qui n'a pas été visité
            if (distance[i] < mindistance && !visited[i]) {
                mindistance = distance[i];
                nextnode = i;
            }

        visited[nextnode] = 1; //considerer le prochin neoud comme visite
        for (i = 0; i < n; i++) //Mettez à jour les distances et les prédécesseurs des voisins non visités de nextnode si un chemin plus court est trouvé
            if (!visited[i])
                if (mindistance + cost[nextnode][i] < distance[i]) {
                    distance[i] = mindistance + cost[nextnode][i];
                    pred[i] = nextnode;
                }
        count++;

        if (nextnode == end) {
            // arreter l'algorithme si le but est attenuer
            break;
        }
    }
    // affichage du resultat
    printf("Shortest Path from %d to %d:\n", start, end);
    printShortestPath(pred, start, end);
}

/**
 * printShortestPath - affichage du plus cours chemin trouver (fonction recursive)
 * @pred: le tableau des predeccesseurs
 * @start: debut
 * @end: fin
*/
void printShortestPath(int pred[MAX], int start, int end) {
    if (end == start) {
        printf("%d", start);
        return;
    }

    printShortestPath(pred, start, pred[end]);
    printf(" -> %d", end);
}

/**
 * matrice_adjacence = Définir la matrice d'adjacence de graphe orientée tand que les sommet ayant de poids.
 * @param n: La taille de la matrice n x n , c'est le nombre des sommets total.
 * @return: Return la matrice initialiser avec les poids de chaque arrêtes entre deux sommets/
 */
int** matrice_adjacence(int n) {
    // Allocate memory for the 2D array
    int** matrice = (int**)malloc(n * sizeof(int*));
    if (matrice == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        matrice[i] = (int*)malloc(n * sizeof(int));
        if (matrice[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }


    // Populer la matrice
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("valeur de la case [%d][%d]: ", i+1, j+1);
            scanf("%d", &matrice[i][j]);
        }
    }

    return matrice;
}

/**
 * Fonction qui affiche la liste d'adjacence de notre graphe
 * @param matrice : Matrice d'adjacence
 * @param n : Taille de notre matrice; nombre des sommets
 */
void affichier_matrice_adjacence(int** matrice, int size){
    for ( int i = 0 ; i < size ; i++){
        for ( int j = 0 ; j < size ; j++){
            printf("%d\t", matrice[i][j]);
        }
        printf("\n");

    }
    printf("\n");
}

/**
 * Fonction simple qui verifier si la taile saisit par l'utilisateur se trouve dans l'intervale [0,9998]
 * @param n : La taille saisit par l'utilisateur
 * @return : Return 1 si la valeur est bonne, 0 sinon
 */
int CheckSize(int n){
    return n > 0 && n < 9998 ? 1 : 0;
}

/**
 * Fonction qui compte les nombres des successeures d'un sommet/noeud
 * @param matrice : La matrice d'adjacence de notre graphe
 * @param noeud : Notre noeud fixe
 * @param n : la taille de la matrice d'adjacence
 * @return Return le nombre des successeurs.
 */
int succ_count(int** matrice, int noeud, int n){
    int count = 0;
    for ( int i = 0 ; i < n ; i++){
        if ( matrice[noeud][i] ){
            count++;
        }
    }
    return count;
}

/**
 * Fonction de type int(*)[] qui returne la liste des noeud successeures d'un sommet/noeud fixe.
 * @param matrice La matrice d'adjacence.
 * @param noeud Notre node, sommet initiale de chaque arrête.
 * @param size Taile de la matrice d'adjacence.
 * @return address de notre liste des successeurs
 */
int* show_succ(int** matrice, int noeud, int size) {
    noeud -= 1;
    int n = succ_count(matrice, noeud, size), counter = 1;

    int* liste_succ = (int*)malloc( (n+1) * sizeof(int) ) ;

    liste_succ[0] = n;
//    la premiere valeur va garder l'information sur le nombre total des successeurs afin d'éviter l'appel plusieurs fois
//    à la fonction qui va compter cette valeur.

    for (int i = 0; i < size; i++) {
        if ( matrice[noeud][i] ) {
            liste_succ[counter] = i+1;
            counter++;
        }
    }

    return liste_succ;
}

/**
 * Fonction qui compte le nombre des prédécesseurs d'un sommet donnée
 * @param matrice : La matrice d'adjacence de notre graphe
 * @param noeud : Noeud donnée
 * @param size : La taille de notre matrice d'adjacence
 * @return : Return de type int, c'est le nombres des prédécesseurs.
 */
int count_pred(int** matrice, int noeud, int size){
    int counter = 0;
    for (int i = 0 ; i < size ; i++){
        if ( matrice[i][noeud] ){
            // notez que: **(matrice + noeud*size + i ) == matrice[i][noeud]
            counter++;
        }
    }
    return counter;
}

/**
 * Fonction de type int(*)[] qui returne la liste des noeud prédécesseurs d'un sommet/noeud fixe.
 * @param matrice La matrice d'adjacence.
 * @param noeud Notre node, sommet finale de chaque arrête.
 * @param size Taile de la matrice d'adjacence.
 * @return address de notre liste des prédécesseurs
 */
int* show_pred(int** matrice, int noeud, int size){
    noeud -= 1;
    int n = count_pred(matrice, noeud, size), counter = 1;
    int* liste_pred = (int*)malloc( (n+1) * sizeof(int));
    liste_pred[0] = n;

    for ( int i = 0 ; i < size ; i ++){
        if ( matrice[i][noeud] ){
            liste_pred[counter] = i+1;
            counter++;
        }
    }
    return liste_pred;
}

/**
 * Cette fonction a comme but d'importer un fichier qui contient la matrice d'adjacence d'un graphe.\n
 * il faut obligatoirement respecter la format de la matrice indiquer comme suit:\n
 * \n
 *   *,*,*,\n
 *   *,*,*,\n
 *   *,*,*\n
 *   \n
 * Aucun espace n'est autoriser, ni aprés la matrice, ni avant, ni entre les valeurs.\n
 * les valeurs sont séparer justement par des virgules.\n
 * la retourne a la ligne signifie une nouvelle ligne.\n
 * La dernier colonne dans la dernier ligne ne doit pas se terminer pas une virgule.\n
 * @param fichier : Chemin de fichier ( Relative/Absolut ) contenant le nom de fichier
 * @param size : Passage par reference a la taille de la matrice ( A determinée d'après la fonction )
 * @return : Retourner l'address dans la mémoire de la matrice d'adjacence.
 */
int** ImportMatrice(char* fichier, int* size) {
    FILE* file = fopen(fichier, "r");
    if (file == NULL) {
        printf("Error lors d'ouverture de fichier: %s\n", fichier);
        return NULL;
    }

    // Determine la taille de la matrice
    int rows = 0, cols = 0;
    char ch;
    while (fscanf(file, "%c", &ch) == 1) {
        if (ch == '\n') {
            rows++;
        }
        else if (ch == ',' || ch <= '9' /*|| (ch >= '0' && ch <= '9')*/) {

            if (ch == ',' && rows == 0) {
                cols++;
            }

        } else {
            printf("Character étranger dans la matrice: %c !. verifiez:\n", ch);
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);

    // Verifiez si vraiment la matrice est carrée
    if (rows != cols) {
        printf("Error: La matrice n'est pas carrée! \n"
               "\n\t\t\tDimension trouvée =>  Ligne: %d  \t Colonne %d ", rows, cols);
        return NULL;
    }

    *size = rows;

    // Allocate memory for the matrice
    int** matrice = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrice[i] = (int*)malloc(cols * sizeof(int));
    }

    // Read the matrice from the file
    file = fopen(fichier, "r");
    if (file == NULL) {
        fprintf(stderr, "Error lors d'ouverture de fichier: %s\n", fichier);
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fscanf(file, "%d,", &matrice[i][j]) != 1) {
                printf("Error de lecture de la matrice d'adjacence de fichier %s.\n", fichier);
                return NULL;
            }
        }
    }

    fclose(file);
    return matrice;
}


//
///**
// * Menu des operations valables dans ce programme. Pratique pour l'affichage de menu aprés chaque execution.
// * @param matrice
// * @param size
// * @return
// */
//int menu(int** matrice, int size ){
//    int  *liste= NULL, noeud = 0;
//
//    printf("\nVoici le menu des operations valable: "
//           "\n\t0. Quitter le programme."
//           "\n\t1. Définir la matrice d'adjacence de votre matrice."
//           "\n\t2. Affichier la matrice d'adjacence."
//           "\n\t3. Trouver le plus courts chemin possible entre deux sommet."
//           "\n\t4. Importer la matrice d'adjacence depuis un fichier .txt "
//           "\n\t5. Trouver l'ensembles des successeurs d'un sommet spécifiques."
//           "\n\t6. Trouver les prédécesseurs d'un sommet spécifiques."
//           "\n\t\tVotre Choix => ");
//
//    int choix;
//    scanf("%d", &choix);
//
//    switch(choix){
//
//        case 0:
//            printf("\n\t\tAu Revoir !");
//            exit(1);
//
//        case 1:
//            printf("\nCombien des noeuds sont present dans votre matrice ? => ");
//            scanf("%d", &size);
//
//            while ( !CheckSize(size) ){
//                printf("\nTaille saisie est non valid, ressayer => ");
//                scanf("%d", &size);
//            }
//
//            matrice = matrice_adjacence(size);
//
//            return menu(matrice, size);
//
//        case 2:
//
//            if (matrice == NULL){
//                printf("Aucun matrice n'est encore définie!");
//
//                return menu(matrice, size);
//            }
//
//            printf("\nVoici la matrice d'adjacence: \n ");
//            affichier_matrice_adjacence(matrice, size);
//
//            return menu(matrice, size);
//
//        case 3:
//
//            printf("Sommet initialse de depart ? => ");
//            int StartNode; scanf("%d", &StartNode);
//
//            printf("Sommet finale  ? => ");
//            int EndNode; scanf("%d", &EndNode);
//
//            algo_A(matrice, size, StartNode, EndNode);
//
//            return menu(matrice, size);
//
//        case 4:
//            printf("\nChemin (Absolut / Relative) de votre fichier (includée le type de fichier!) ? => ");
//            char fichier[250];
//            scanf(" %[^\n]", fichier);
//            int** temp = ImportMatrice(fichier, &size);
//            if ( temp == NULL){
//                printf("\nAucun matrice n'est importée");
//                free(temp);
//                return menu(matrice, size);
//            }
//            else{
//                printf("\nImportation a réussit! ");
//                matrice = temp;
//                return menu(matrice, size);
//            }
//
//        case 5:
//            //Affichiez l'ensemble des successeurs d'un sommet
//
//            printf("\nQuel noeud voudrais-vous affichier ces successeurs? (1 - %d)=> ", size);
//            scanf("%d", &noeud);
//
//            liste = show_succ(matrice, noeud, size);
//
//            printf("Voici la liste des successuers de sommet numero %d\t", noeud);
//            for ( int i = 1 ; i <= liste[0] ; i++){
//                printf("\t%d ;", liste[i]);
//            }
//
//
//            return menu(matrice, size);
//
//        case 6:
//            printf("\nQuel noeud voudrais-vous affichiez ces prédécesseur? (1 - %d)=> ", size);
//            scanf("%d", &noeud);
//
//            liste = show_pred(matrice, noeud, size);
//
//            printf("Voici la liste des prédécesseur de sommet numero %d\n", noeud);
//            for ( int i = 1 ; i <= liste[0] ; i++){
//                printf("\t%d ;", liste[i]);
//            }
//
//            return menu(matrice, size);
//
//        default :
//            printf("\nChoix non valide, ressayer");
//            return menu(matrice, size);
//    }
//}
//
//// juste pour tester
//int main() {
//
//    //initialisation de la matrice d'adjacence.
//    int** matrice = NULL;
//    int size = 0;
//
//    menu(matrice, size);
//
//
//    return 0;
//}
//
