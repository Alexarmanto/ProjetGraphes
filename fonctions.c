#include "fonctions.h"

// --- OUTILS DE SAISIE SECURISEE ---

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int lire_entier_securise() {
    int valeur;
    while (scanf("%d", &valeur) != 1) {
        printf("Erreur : Veuillez entrer un nombre entier valide : ");
        vider_buffer();
    }
    return valeur;
}

// --- GESTION MEMOIRE ---

int** allouer_matrice(int n) {
    if (n <= 0) return NULL;

    int** mat = (int**)malloc(n * sizeof(int*));
    if (mat == NULL) {
        fprintf(stderr, "Erreur : Echec d'allocation memoire (lignes).\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        mat[i] = (int*)malloc(n * sizeof(int));
        if (mat[i] == NULL) {
            fprintf(stderr, "Erreur : Echec d'allocation memoire (colonnes).\n");
            for (int j = 0; j < i; j++) free(mat[j]);
            free(mat);
            exit(EXIT_FAILURE);
        }
    }
    return mat;
}

void liberer_matrice(int** mat, int n) {
    if (mat == NULL) return;
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

void initialiser_graphe(Graphe* g) {
    g->n_sommets = 0;
    g->L = NULL;
    g->P = NULL;
    g->est_charge = 0;
}

void detruire_graphe(Graphe* g) {
    if (g->est_charge) {
        liberer_matrice(g->L, g->n_sommets);
        liberer_matrice(g->P, g->n_sommets);
        g->est_charge = 0;
        g->n_sommets = 0;
    }
}

// --- CHARGEMENT ET FICHIERS ---

int charger_graphe_fichier(const char* nom_fichier, Graphe* g) {
    detruire_graphe(g);

    FILE* f = fopen(nom_fichier, "r");
    if (f == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier '%s'. Verifiez le nom.\n", nom_fichier);
        return 0;
    }

    if (fscanf(f, "%d", &g->n_sommets) != 1) {
        printf("Erreur : Format de fichier invalide (nombre de sommets manquant).\n");
        fclose(f); return 0;
    }

    if (g->n_sommets <= 0) {
        printf("Erreur : Le nombre de sommets doit etre positif.\n");
        fclose(f); return 0;
    }

    int n_arcs;
    if (fscanf(f, "%d", &n_arcs) != 1) {
        printf("Erreur : Format de fichier invalide (nombre d'arcs manquant).\n");
        fclose(f); return 0;
    }

    g->L = allouer_matrice(g->n_sommets);
    g->P = allouer_matrice(g->n_sommets);
    g->est_charge = 1;

    for (int i = 0; i < g->n_sommets; i++) {
        for (int j = 0; j < g->n_sommets; j++) {
            if (i == j) {
                g->L[i][j] = 0;
            } else {
                g->L[i][j] = INF;
            }
            g->P[i][j] = -1;
        }
    }

    int u, v, w;
    for (int k = 0; k < n_arcs; k++) {
        int lectures = fscanf(f, "%d %d %d", &u, &v, &w);
        if (lectures != 3) {
            printf("Avertissement : Fin de fichier inattendue ou format d'arc incorrect (arc %d).\n", k+1);
            break;
        }

        if (u < 0 || u >= g->n_sommets || v < 0 || v >= g->n_sommets) {
            printf("Avertissement : Arc %d->%d ignore (sommets hors limites 0-%d).\n", u, v, g->n_sommets-1);
            continue;
        }

        g->L[u][v] = w;
        g->P[u][v] = u;
    }

    fclose(f);
    printf("Succes : Graphe charge (%d sommets, %d arcs theoriques).\n", g->n_sommets, n_arcs);
    return 1;
}

// --- AFFICHAGE ---

void afficher_matrices(Graphe* g, int k) {
    printf("\n========================================\n");
    if (k == -1) printf("MATRICES INITIALES (D0, P0)");
    else printf("ETAPE k = %d (Passage par sommet %d)", k, k);
    printf("\n========================================\n");

    printf("MATRICE L (Poids des chemins) :\n");
    printf("      ");
    for(int i=0; i<g->n_sommets; i++) printf("%6d", i);
    printf("\n");

    for (int i = 0; i < g->n_sommets; i++) {
        printf("%4d |", i);
        for (int j = 0; j < g->n_sommets; j++) {
            if (g->L[i][j] == INF) printf("   INF");
            else printf("%6d", g->L[i][j]);
        }
        printf("\n");
    }

    printf("\nMATRICE P (Predecesseurs) :\n");
    printf("      ");
    for(int i=0; i<g->n_sommets; i++) printf("%6d", i);
    printf("\n");

    for (int i = 0; i < g->n_sommets; i++) {
        printf("%4d |", i);
        for (int j = 0; j < g->n_sommets; j++) {
            if (g->P[i][j] == -1) printf("   NIL");
            else printf("%6d", g->P[i][j]);
        }
        printf("\n");
    }
}

// --- ALGORITHME PRINCIPAL ---

void floyd_warshall(Graphe* g) {
    if (!g->est_charge) return;

    afficher_matrices(g, -1);

    for (int k = 0; k < g->n_sommets; k++) {
        for (int i = 0; i < g->n_sommets; i++) {
            for (int j = 0; j < g->n_sommets; j++) {

                if (g->L[i][k] != INF && g->L[k][j] != INF) {

                    int nouveau_poids = g->L[i][k] + g->L[k][j];

                    if (g->L[i][j] > nouveau_poids) {
                        g->L[i][j] = nouveau_poids;
                        g->P[i][j] = g->P[k][j];
                    }
                }
            }
        }
        afficher_matrices(g, k);
    }
}

// --- ANALYSE ET RESULTATS ---

int detecter_circuit_absorbant(Graphe* g) {
    for (int i = 0; i < g->n_sommets; i++) {
        if (g->L[i][i] < 0) return 1;
    }
    return 0;
}

void afficher_chemin_recursif(Graphe* g, int depart, int arrivee) {
    if (depart == arrivee) {
        printf("%d", depart);
        return;
    }

    int pred = g->P[depart][arrivee];

    if (pred == -1) {
        printf("(Chemin rompu)");
        return;
    }

    afficher_chemin_recursif(g, depart, pred);
    printf(" -> %d", arrivee);
}
