#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// --- CONSTANTES ---
#define INF 1000000000  // Valeur "Infinie"
#define MAX_FILENAME 256

// --- STRUCTURES DE DONNEES ---
typedef struct {
    int n_sommets;      // Nombre de sommets
    int** L;            // Matrice des Longueurs (Poids)
    int** P;            // Matrice des Prédécesseurs
    int est_charge;     // Indicateur d'état (1 si chargé, 0 sinon)
} Graphe;

// --- PROTOTYPES DES OUTILS DE SAISIE ---
void vider_buffer();
int lire_entier_securise();

// --- PROTOTYPES DE LA GESTION MEMOIRE ET GRAPHE ---
int** allouer_matrice(int n);
void liberer_matrice(int** mat, int n);
void initialiser_graphe(Graphe* g);
void detruire_graphe(Graphe* g);
int charger_graphe_fichier(const char* nom_fichier, Graphe* g);

// --- PROTOTYPES DE L'ALGORITHME ET AFFICHAGE ---
void afficher_matrices(Graphe* g, int k);
void floyd_warshall(Graphe* g);
int detecter_circuit_absorbant(Graphe* g);
void afficher_chemin_recursif(Graphe* g, int depart, int arrivee);

#endif // FONCTIONS_H
