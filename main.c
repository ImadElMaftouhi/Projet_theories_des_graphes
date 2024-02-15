//
// Created by imad el maftouhi on 1/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX 500
#define INFINITY 99999

typedef struct voisinage{
    int destination;
    int poid;
    struct voisinage *next;
}voisinage;

typedef struct Graphe{
    int sommets;
    struct voisinage *list[MAX];
}Graphe;

/**
 * Structure pour l'algorithme A
 * @poidChemin : Le poid de chemin entre le source et la destination
 * @listPred : Liste des sommets constituant le chemin determinée
*/
struct pathData{
    int poidChemin;
    int *listPred;
};

/*------------------ D E C L A R A T I O N S ---------------*/

int **AfficherMatriceAdjacence(struct Graphe *G);
void AjouterSommet(struct Graphe *G, int source, int destination, int poid);
void AjouterSommetSource(struct Graphe *G);
void SupprimerSommetSource(struct Graphe *G);
int* bellmanFord(struct Graphe* G, int source, int* distance);
void repondererSommet(struct Graphe *G, int* distance);
struct pathData** johnsons(struct Graphe *G, int **matrice);
struct pathData algo_A(int** Graph, int n, int start, int end);
void printShortestPath(int pred[MAX], int start, int end);
int count_pred(int** matrice, int noeud, int nbrSommet);
int* show_pred(int** matrice, int noeud, int n);
int succ_count(int** matrice, int noeud, int n);
int** ImporterFichier(char* fichier, int* nbrSommet);
int* show_succ(int** matrice, int noeud, int nbrSommet);
Graphe* ConvertirMatriceVersListe(Graphe* G, int** matrice, int taille);
int menu();

/*---------------------- F O N C T I O N S-------------------*/

/**
 * Fonction qui permet l'ajout d'un nouveau sommet au graphe
 * @param G : pointer de type << Graphe* >>
 * @param source : l'indice i des lignes de la matrice d'adjacence de graphe
 * @param destination : L'indice j des colonnes de la matrice d'adjacence
 * @param poid : Poid de la relation entre source et destination
 */
void AjouterSommet(struct Graphe *G, int source, int destination, int poid) {
    struct voisinage* temp = NULL;

    struct voisinage *newNode = (struct voisinage *)malloc(sizeof(voisinage));
    if (newNode == NULL){
        printf("\nL'allocation de la mémoire a échouée! Return ...");
        return;
    }
    newNode->destination = destination;
    newNode->poid = poid;
    newNode->next = NULL;

    temp = G->list[source];

    // insertion toujours a la fin de la liste d'adjacence
    if (temp == NULL) {
        G->list[source] = newNode;
        return;
    } else {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
        return;
    }
}


/**
 * Implementation d'algorithme de Bellmann-Ford pour trouver le plus courts chemin depuis un sommet initiale/depart vers
 * tous les autres sommets existent dans le graphe.\n
 * L'algorithme de Bellman-Ford est utilise pour les traitement des graphe orienté ou bien non orientée, tel que les
 * arcs sont pondéré soit positivement, soit negativement.
 * @param G : Pointeur de type << Graphe* >>
 * @param source : Sommet source
 * @param distance : Tableau des distances
 * @return : Meme tableau des distances saisit commet argument, mais cette fois remplit des distances.
 */
int* bellmanFord(struct Graphe* G, int source, int* distance) {
    int nbrSommet = G->sommets;
    struct voisinage * voisin = NULL;

    // Initialization
    for (int i = 0; i < nbrSommet; i++) {
        distance[i] = INFINITY;
    }
    distance[source] = 0;

    // First pass
    voisin = G->list[source];
    while (voisin != NULL) {
        if (distance[source] + voisin->poid < distance[voisin->destination]) {
            distance[voisin->destination] = distance[source] + voisin->poid;
        }
        voisin = voisin->next;
    }

    // Relaxation passes
    for (int iteration = 0; iteration < nbrSommet - 1; iteration++) {
        for (int current = 0; current < nbrSommet; current++) {
            voisin = G->list[current];
            while (voisin != NULL) {
                if (distance[current] + voisin->poid < distance[voisin->destination]) {
                    distance[voisin->destination] = distance[current] + voisin->poid;
                }
                voisin = voisin->next;
            }
        }
    }

    // Detection des cycles de poid total negative
    for (int current = 0; current < nbrSommet; current++) {
        voisin = G->list[current];
        while (voisin != NULL) {
            if (distance[current] != INFINITY && distance[current] + voisin->poid < distance[voisin->destination]) {
                printf("Cycle avec poids negatif detecte au sommet %d\n", current);
                return NULL;
            }
            voisin = voisin->next;
        }
    }

    return distance;
}


/**
 * Fonction qui permet d'ajouter un nouveau noeud "source" et le relationner avec tous les autres sommets avec un poids
 * égale a 0, ce noeud va nous permet d'éliminer les poid negative vers des poids positive sans perte d'informations
 * @param G : Pointeur de type << Graphe* >>
 */
void AjouterSommetSource(struct Graphe *G) {
    if (G->sommets == MAX) {
        printf("Error: Nombre maximun des voisins atteint! Return ...\n");
        return;
    }
    struct voisinage * noeud = NULL;

    int noeudIndex = G->sommets ;

    G->list[noeudIndex] = NULL;

    for (int i = 0; i < noeudIndex; i++) {
        noeud = (struct voisinage *)malloc(sizeof(voisinage));
        if ( noeud == NULL ){
            printf("\nError! Allocation de memoire echouee! Return ... ");
        }
        noeud->destination = noeudIndex;
        noeud->poid = 0;
        noeud->next = NULL;
        struct voisinage* temp = G->list[noeudIndex];
        if ( temp == NULL ){
            G->list[noeudIndex] = noeud;
            continue;
        }
        else{
            while ( temp->next != NULL ){
                temp = temp->next;
            }
            temp->next = noeud;
            continue;
        }
    }
    G->sommets++;
}


/**
 * Fonction qui permet de supprimer le sommet source ajouter pour eliminer les poids negative.
 * @param G : Pointeur de type << Graphe * >>
 */
void SupprimerSommetSource(struct Graphe *G) {
    struct voisinage* ptr = G->list[G->sommets];
    struct voisinage* temp = NULL;
    while ( ptr != NULL ){
        temp = ptr;
        ptr = ptr->next;
        free(temp);
    }
    free(G->list[G->sommets]);
    free(ptr);
    G->sommets--;
}


/**
 * Fonction qui permet de rééquilibrée/repondérer le point des relations
 * @param G : Pointeur de type << Graphe* >>
 * @param distance : Tableau des distances
 */
void repondererSommet(struct Graphe *G, int* distance) {
    int n = G->sommets;
    struct voisinage *voisin = NULL;
    for (int i = 0; i < n; i++) {
        voisin = G->list[i];
        while (voisin != NULL) {
            voisin->poid += distance[i] - distance[voisin->destination];
            voisin = voisin->next;
        }
    }
}


struct pathData** johnsons(struct Graphe *G, int** matrice) {
    int n = G->sommets;

    printf("\nEtape1 : Ajout de sommet source avec poid 0: \n ");

    AjouterSommetSource(G);  AfficherMatriceAdjacence(G);


    printf("\nEtape2 : Bellman-Ford pour obtenir les plus petites distances depuis le nouveau nœud: \n");
    int* distance = (int*)malloc(n * sizeof(int));
    if (!bellmanFord(G, n, distance)) {
        free(distance);
        return NULL;
    }

    for ( int i = 0 ; i < n ; i++ ){
        printf("\nDistance[%d] = %d", i ,distance[i]);
    }


    printf("\n\nEtape 3: Supprimer le noeud source et reponderer tous les arcs/aretes de graphe: \n ");
    
    SupprimerSommetSource(G);  repondererSommet(G, distance);


    int ** temp = AfficherMatriceAdjacence(G);

    // Etape 4 : Executer l'algorithme de Dijkstra pour chaque sommet de graphe
    struct pathData** distancesMinimal = (struct pathData**)malloc(n * sizeof(struct pathData*));

    if (distancesMinimal == NULL) {
        printf("\n 1st level memory allocation failed \n");
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        distancesMinimal[i] = (struct pathData*)malloc(n * sizeof(struct pathData));
        
        if (distancesMinimal[i] == NULL) {
            printf("2nd level Memory allocation failed \n");
            return NULL;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            
            struct pathData tempResult = algo_A(temp, n, i, j);

            // Assign values to distancesMinimal
            distancesMinimal[i][j].poidChemin = tempResult.poidChemin;

            // Allocate memory for listPred
            if ( distancesMinimal[i][j].poidChemin == INFINITY ){
                distancesMinimal[i][j].listPred = NULL;
                continue;
            }
            
            distancesMinimal[i][j].listPred = (int*)malloc(n * sizeof(int));

            // Copy listPred from tempResult to distancesMinimal
            memcpy(distancesMinimal[i][j].listPred, tempResult.listPred, n * sizeof(int));
        }
    }


    // Fin. return ...
    free(distance);

    return distancesMinimal;

    // when returning distanceMinimal, I can't seem to get the list listPred in the struct but I only get the poidChemin, even tho it is accessible in this function, propose a solution
}


/**
 * Une fonction qui permet l'affichage de notre matrice
 * @param G : Pointeur de type << Graphe* >>
 */
int **AfficherMatriceAdjacence(struct Graphe *G) {
    if ( G == NULL){
        printf("\nThe G graph is NULL\n");
        return NULL;
    }
    struct voisinage *temp = NULL;

    int ** matrice = (int**)malloc( G->sommets * sizeof(int*) );
    
    for (int i = 0 ; i < G->sommets ; i++){
        matrice[i] = (int*)malloc(sizeof(int) * G->sommets);
    }


    int poid = 0, width = 7;
    printf("\nMatrice d'adjacence de graphe :\n");
    for (int i = 0; i < G->sommets; i++) {
        for (int j = 0; j < G->sommets; j++) {
            temp = G->list[i];
            poid = 0;

            while (temp != NULL) {
                if (temp->destination == j) {
                    poid = temp->poid;
                    break;
                }
                temp = temp->next;
            }  

            matrice[i][j] = poid;
            printf("%-*d",width , poid);
            if (j < G->sommets - 1) {
                printf(" ");
            }

        }
        printf("\n");
    }
    return matrice;
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
 * @param nbrSommet : Passage par reference a la taille de la matrice ( A determinée d'après la fonction )
 * @return : Retourner l'address dans la mémoire de la matrice d'adjacence.
 */
int** ImporterFichier(char* fichier, int* nbrSommet) {
    FILE* file = fopen(fichier, "r");
    if (file == NULL) {
        printf("Error lors d'ouverture de fichier: %s\n", fichier);
        return NULL;
    }

    // Determiner la taille de la matrice
    int rows = 0, cols = 0;
    char ch;

    while (fscanf(file, "%c", &ch) == 1) {
        if (ch == '\n') {
            rows++;
        }
        else if ( ch == ',' || ch <= '9') {

            if ( ch == ','  && rows == 0) {
                cols++;
            }

        } else {
            printf("Character étranger dans la matrice: << %c >> \tVerifier dans le fichier\n ", ch);
            return NULL;
        }
    }
    fclose(file);

    // Verifiez si vraiment la matrice est carrée
    if (rows != cols) {
        printf("Error: La matrice n'est pas carrée! \n"
               "\n\t\t\tDimension trouvée =>  Ligne: %d  \t Colonne %d ", rows, cols);
        return NULL;
    }

    *nbrSommet = rows;

    int** matrice = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrice[i] = (int*)malloc(cols * sizeof(int));
    }

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


Graphe* ConvertirMatriceVersListe(Graphe* G, int** matrice, int taille){
    G = (Graphe*)malloc(sizeof(Graphe));
    G->sommets = taille;

    for ( int i = 0 ; i < MAX ; i ++){
        G->list[i] = NULL;
    }
    for (int i = 0; i < G->sommets; i++)
    {
        for (int j = 0; j < G->sommets; j++)
        {
            if (matrice[i][j] != 0)
            {
                AjouterSommet(G, i, j, matrice[i][j]);
            }
        }
    }
    return G;
}


void matriceAdjacence(int nbrSommet, Graphe** G, int*** matrice){
    int poid;

    Graphe* TempG = (struct Graphe *) malloc(sizeof(struct Graphe));

    int** TempM = (int**)malloc( nbrSommet * sizeof(int*));

    if (TempM == NULL) {
        printf("\nError de la reservation pour la matrice\n");
        exit(EXIT_FAILURE);
    }

    if (TempG == NULL) {
        printf("\nError de reservation de la mémoire pour le pointeur de type << Graphe* >>");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nbrSommet ; i++) {
        TempM[i] = (int*)malloc( nbrSommet * sizeof(int));
        if (TempM[i] == NULL) {
            printf( "\nError de la reservation"
                    " de le mémoire pour la case matrice[%d] \n", i);
            exit(EXIT_FAILURE);
        }
    }


    TempG->sommets = nbrSommet;
    for (int i = 0; i < nbrSommet; i++) {
        TempG->list[i] = NULL;
    }

    for (int i = 0; i < nbrSommet; i++)
        for (int j = 0; j < nbrSommet; j++) {
            printf("Value de la case [%d][%d] ? => ", i + 1, j + 1);
            scanf("%d", &poid);
            TempM[i][j] = poid;
            AjouterSommet(TempG, i, j, poid);
        }

    *G = TempG;
    *matrice = TempM;
}


/**
 * Fonction qui compte le nombre des prédécesseurs d'un sommet donnée
 * @param matrice : La matrice d'adjacence de notre graphe
 * @param noeud : Noeud donnée
 * @param nbrSommet : La taille de notre matrice d'adjacence
 * @return : Return de type int, c'est le nombres des prédécesseurs.
 */
int count_pred(int** matrice, int noeud, int nbrSommet){
    int counter = 0;
    for (int i = 0 ; i < nbrSommet ; i++){
        if ( matrice[i][noeud] ){
            // notez que: **(matrice + noeud*nbrSommet + i ) == matrice[i][noeud]
            counter++;
        }
    }
    return counter;
}


/**
 * Fonction de type int(*)[] qui returne la liste des noeud prédécesseurs d'un sommet/noeud fixe.
 * @param matrice La matrice d'adjacence.
 * @param noeud Notre node, sommet finale de chaque arrête.
 * @param nbrSommet Taile de la matrice d'adjacence.
 * @return address de notre liste des prédécesseurs
 */
int* show_pred(int** matrice, int noeud, int nbrSommet){
    noeud -= 1;
    int n = count_pred(matrice, noeud, nbrSommet), counter = 1;
    int* liste_pred = (int*)malloc( (n+1) * sizeof(int));
    liste_pred[0] = n;

    for ( int i = 0 ; i < nbrSommet ; i ++){
        if ( matrice[i][noeud] ){
            liste_pred[counter] = i+1;
            counter++;
        }
    }
    return liste_pred;
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
 * algo_A: fonction qui trouve le plus cours chemin entre deux sommet en utilisant l'algorithm A
 * @Graph: le graph etudie
 * @n: nombre des noeuds dans le graphe etudiée
 * @start: le noeud ou on va commencer
 * @end: le noeud ou on va arreter
 * Retun: la fonction ne retourne rien .(Elle affiche le cours chemin)
*/
struct pathData algo_A(int** Graph, int n, int start, int end)
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

    while (count < n )
    {

        mindistance = INFINITY;

        for (i = 0; i < n; i++) //Boucle imbriquée pour trouver le sommet avec la distance minimale qui n'a pas été visité
            if (distance[i] < mindistance && !visited[i]) {
                mindistance = distance[i];
                nextnode = i;
            }

        visited[nextnode] = 1; // considérer le neoud suivant comme visité

        for (i = 0; i < n; i++) // Mise à jour des distances et des prédécesseurs des voisins non visités du nœud suivant si un chemin plus court est trouvé
            if (!visited[i])
                if (mindistance + cost[nextnode][i] < distance[i]) {
                    distance[i] = mindistance + cost[nextnode][i];
                    pred[i] = nextnode;
                }
        
        count++;

        if (nextnode == end) {
            // arrêter l'algorithme si l'objectif est atteint
            break;
        }
    }

    // printf("\nChemin le plus court de %d à %d avec une valeur total de %d:\n", start, end, distance[end]);
    // printShortestPath(pred, start, end);

    //traitement de return 
    struct pathData data;

    data.listPred = pred;
    data.poidChemin = distance[end];

    return data;
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
 * Fonction de type int(*)[] qui returne la liste des noeud successeures d'un sommet/noeud fixe.
 * @param matrice La matrice d'adjacence.
 * @param noeud Notre node, sommet initiale de chaque arrête.
 * @param nbrSommet Taile de la matrice d'adjacence.
 * @return address de notre liste des successeurs
 */
int* show_succ(int** matrice, int noeud, int nbrSommet) {
    noeud -= 1;
    int n = succ_count(matrice, noeud, nbrSommet), counter = 1;

    int* liste_succ = (int*)malloc( (n+1) * sizeof(int) ) ;

    liste_succ[0] = n;
//    la premiere valeur va garder l'information sur le nombre total des successeurs afin d'éviter l'appel plusieurs fois
//    à la fonction qui va compter cette valeur.

    for (int i = 0; i < nbrSommet; i++) {
        if ( matrice[noeud][i] ) {
            liste_succ[counter] = i+1;
            counter++;
        }
    }

    return liste_succ;
}


/**
 * Un menu des operations valables dans notre application de recherche des plus courts chemins
 * @return : Le choix d'utilisateur
 */
int menu(){
    printf("\n---------------------------Menu--------------------------------------"
           "\n-\t 0. Quitter"
           "\n-\t 1. Definir une matrice d'adjacence d'une graphe ponderee"
           "\n-\t 2. Importer la matrice d''adjacence depuis un fichier"
           "\n-\t 3. Afficher la matrice d'adjacence de graphe,"
           "\n-\t 4. Algorithme de Johnson le plus courts chemin."
           "\n-\t 5. Algorithme A pour le plus courts chemin."
           "\n-\t 6. Afficher l'ensemble des successeurs d'un sommet."
           "\n-\t 7. Afficher l'ensemble des predecesseurs d'un sommet."
           "\n-\t ------Votre Choix =>"
    );
    int choix;
    scanf("%d", &choix);
    while ( choix < 0 ||choix > 7){
        printf("Choix non valide! Ressayer => ");
        scanf("%d", &choix);
    }
    return choix;
}


int main()
{
    // Initialisation de graphe.
    struct Graphe *G = NULL;

    // Initialisation de la matrice d'adjacence :
    int **matrice;

    int nbrSommet, choix, poid, noeud = 0, *liste;
    char path[250];

    struct pathData  **ResultJohnson = NULL;

    while ( (choix = menu() ) ){

        switch ( choix ) {

            case 0:
                printf("\nAu revoir!.");
                free(ResultJohnson);
                free(G);
                for (int i = 0; i < nbrSommet; i++) {
                    free(matrice[i]);
                }
                free(matrice);
                exit(1);

            case 1:
                printf("\nNombre des sommets dans le graphe ? => ");
                scanf("%d", &nbrSommet);

                while (nbrSommet <= 0) {
                    printf("\nNombre non valide, ressayer => ");
                    scanf("%d", &nbrSommet);
                }
                matriceAdjacence(nbrSommet, &G, &matrice);

                break;

            case 2:
                printf("\nChemin (Absolut / Relative) de votre fichier (includee le type de fichier!) ? => ");
                char fichier[250];
                scanf(" %[^\n]", fichier);
                matrice = ImporterFichier(fichier, &nbrSommet);
                if (matrice == NULL) {
                    printf("\nAucun matrice n'est importee");
                    free(matrice);
                    break;
                } else {
                    G = ConvertirMatriceVersListe(G, matrice, nbrSommet);
                    printf("\n\t\tL'importation s'est terminee avec succes!\n");
                    break;
                }

            case 3:
                AfficherMatriceAdjacence(G);
                printf("\n\n");
                break;

            case 4:
                ResultJohnson = johnsons(G, matrice);
                // I'm having problems at this level, the function returns an array of type struct pathdata but I can only access the poidChemin and not the listChemin
                
                if (ResultJohnson == NULL) {
                    printf("Cycle avec poid negative detectee.\n");
                    return 0;
                }

                printf("\n Matrice des poids total des plus courts chemin entre tous sommets: \n");

                int width = 5;
                printf("\t");
                
                for (int i = 0; i < nbrSommet; i++) {
                    printf("%-*d ", width, i);
                }
                printf("\n\n");

                for (int i = 0; i < nbrSommet; i++) {
                    printf("%d\t", i);
                    for (int j = 0; j < nbrSommet; j++) {
                        if (ResultJohnson[i][j].poidChemin == INFINITY) {
                            /*Representer l'infinit avec X
                             * c.-à-d. qu'il n'existe pas de chemin/chaine entre les deux sommets
                             */
                            printf("%-*c", width, 'X');
                        } else {
                            printf("%-*d", width, ResultJohnson[i][j].poidChemin);
                        }
                        if (j < nbrSommet - 1) {
                            printf(" ");
                        }
                    }
                    printf("\n");
                }
                printf("\n");

                printf("\n\n Les chemin/chaines des plus courts chemin \n");
                for (int i = 0; i < nbrSommet; i++) {
                    for (int j = 0; j < nbrSommet; j++) {
                        if ( i == j )
                            continue;
                        printf("\n %d => %d : \n\t", i , j);
                        if (ResultJohnson[i][j].listPred != NULL) {
                            printShortestPath(ResultJohnson[i][j].listPred, i , j);
                        } else {
                            printf("Aucun chemin possible!.");
                        }
                    }
                }

                

               
                for (int i = 0; i < nbrSommet; i++) {
                    free(ResultJohnson[i]);
                }
                free(ResultJohnson);
                break;

            case 5:

                printf("Sommet initialse : ? => ");
                int StartNode; scanf("%d", &StartNode);

                printf("Sommet finale : ? => ");
                int EndNode; scanf("%d", &EndNode);

                 struct pathData data = algo_A(matrice,
                                         nbrSommet,
                                         StartNode,
                                         EndNode);

                // affichage du resultat

                printf("\nChemin le plus court de %d à %d avec un point total de %d:\n", 
                StartNode,
                EndNode,
                data.poidChemin);

                printf("\n");

                printShortestPath(data.listPred, StartNode, EndNode);

                break;

            case 6:
                //Affichiez l'ensemble des successeurs d'un sommet

                printf("\nQuel noeud voudrais-vous affichier ces successeurs? (1 - %d)=> ", nbrSommet);
                scanf("%d", &noeud);

                liste = show_succ(matrice, noeud, nbrSommet);

                printf("Voici la liste des successuers de sommet numero %d\t", noeud);
                for ( int i = 1 ; i <= liste[0] ; i++){
                    printf("\t%d ;", liste[i]);
                }

                break;

            case 7:
                printf("\nQuel noeud voudrais-vous affichiez ces prédécesseur? (1 - %d)=> ", nbrSommet);
                scanf("%d", &noeud);

                liste = show_pred(matrice, noeud, nbrSommet);

                printf("Voici la liste des prédécesseur de sommet numero %d\n", noeud);
                for ( int i = 1 ; i <= liste[0] ; i++){
                    printf("\t%d ;", liste[i]);
                }

                break;
            default:
                printf("\nChoix non valide! Return ... ");
                break;

        }
    }
    return 0;
}
