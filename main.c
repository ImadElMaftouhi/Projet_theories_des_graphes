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


