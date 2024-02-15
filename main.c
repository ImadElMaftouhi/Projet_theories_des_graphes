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






/*---------------------- F O N C T I O N S-------------------*/



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
