#include "fonctions.h"

// --- MAIN ---

int main() {
    Graphe g;
    initialiser_graphe(&g);

    int continuer_programme = 1;
    char nom_fichier[MAX_FILENAME];

    printf("--- PROJET GRAPHES : FLOYD-WARSHALL ---\n");

    while (continuer_programme) {
        printf("\nEntrez le nom du fichier contenant le graphe (ex: graphe.txt) : ");
        scanf("%255s", nom_fichier);
        vider_buffer();

        if (charger_graphe_fichier(nom_fichier, &g)) {

            // Exécution de l'algorithme
            floyd_warshall(&g);

            // Vérification Circuit Absorbant (Point 6)
            int circuit_detecte = detecter_circuit_absorbant(&g);

            if (circuit_detecte) {
                printf("\n/!\\ ALERTE : Circuit absorbant detecte (cycle de poids negatif).\n");
                printf("Les plus courts chemins NE SONT PAS garantis etre corrects.\n");
            }

            // POINT 7 : Recherche de chemins
            printf("\n--- RECHERCHE DE CHEMINS ---\n");
            int continuer_chemin = 1;
            while (continuer_chemin) {
                printf("\nVoulez-vous afficher un chemin ? (1: Oui, 0: Non) : ");
                int choix = lire_entier_securise();
                vider_buffer();

                if (choix == 0) {
                    continuer_chemin = 0;
                } else {
                    int depart, arrivee;
                    printf("Sommet de depart (0 a %d) : ", g.n_sommets - 1);
                    depart = lire_entier_securise();
                    vider_buffer();

                    printf("Sommet d'arrivee (0 a %d) : ", g.n_sommets - 1);
                    arrivee = lire_entier_securise();
                    vider_buffer();

                    // 1. Vérification validité sommets
                    if (depart < 0 || depart >= g.n_sommets || arrivee < 0 || arrivee >= g.n_sommets) {
                        printf("Erreur : Indices de sommets invalides (doivent etre entre 0 et %d).\n", g.n_sommets - 1);
                    }
                    // 2. GESTION DU CIRCUIT ABSORBANT SPÉCIFIQUE
                    // Affiche un message d'erreur clair si un circuit a été détecté globalement.
                    else if (circuit_detecte) {
                        printf("RESULTAT : Le graphe contient un circuit absorbant.\n");
                        printf("Le chemin minimal entre %d et %d est INDÉFINI.\n", depart, arrivee);
                    }
                    // 3. Vérification Chemin Impossible (L[i][j] = INF)
                    else if (g.L[depart][arrivee] == INF) {
                        printf("Resultat : Pas de chemin existant entre %d et %d. (Chemin impossible)\n", depart, arrivee);
                    }
                    // 4. Affichage du Chemin
                    else {
                        printf("Chemin minimal (Cout = %d) : ", g.L[depart][arrivee]);
                        afficher_chemin_recursif(&g, depart, arrivee);
                        printf("\n");
                    }
                }
            }
        }

        // Nettoyage mémoire
        detruire_graphe(&g);

        printf("\n--------------------------------------------\n");
        printf("Voulez-vous traiter un autre graphe ? (1: Oui, 0: Non) : ");
        int choix_fin = lire_entier_securise();
        vider_buffer();
        if (choix_fin == 0) continuer_programme = 0;
    }

    printf("\nFin du programme.\n");
    return 0;
}
