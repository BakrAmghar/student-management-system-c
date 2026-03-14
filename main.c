#include <stdio.h>
#include "etudiant.h"

void Menu(){
    printf("\n");
    printf("=========================================\n");
    printf("           GESTION DES ETUDIANTS         \n");
    printf("=========================================\n");

    printf("\n======== AJOUTER ETUDIANT ========\n");
    printf("1 -> Ajouter en tete\n");
    printf("2 -> Ajouter en queue\n");


    printf("\n======== MODIFICATION ========\n");
    printf("3 -> Changer moyenne d'un etudiant\n");
    printf("4 -> Changer le nom d'un etudiant\n");
    printf("5 -> Supprimer un etudiant\n");
    printf("6 -> Trier par moyenne\n");


    printf("\n======== RECHERCHE / INFO ========\n");
    printf("7 -> Rechercher un etudiant par ID\n");
    printf("8 -> Afficher un etudiant precis\n");


    printf("\n======== AFFICHAGE / STATS ========\n");
    printf("9 -> Afficher tous les etudiants\n");
    printf("10 -> Taille de la liste\n");
    printf("11 -> La moyenne de la class\n");

    printf("\n======== FICHIERS ========\n");
    printf("12 -> Sauvegarder la liste\n");
    printf("13 -> Charger liste depuis le fichier\n");
    printf("14 -> Verifier les anomalies");
    printf("\n=========================================\n");

    printf("0 -> Quitter\n");

    printf("Votre choix : ");
}

int main()
{
 int choix;
 char id[50];
 char nom[50];
 float moy;
 etudiant *etu;

 do{
     //ART MADE BY ASCIIART.EU + gpt
    printf("  _____ _______ _    _ _____  ______ _   _ _______     __  __          _   _          _____ ______ __  __ ______ _   _ _______         \n");
    printf(" / ____|__   __| |  | |  __ \\|  ____| \\ | |__   __|   |  \\/  |   /\\   | \\ | |   /\\   / ____|  ____|  \\/  |  ____| \\ | |__   __|\n");
    printf("| (___    | |  | |  | | |  | | |__  |  \\| |  | |      | \\  / |  /  \\  |  \\| |  /  \\ | |  __| |__  | \\  / | |__  |  \\| |  | |    \n");
    printf(" \\___ \\   | |  | |  | | |  | |  __| | . ` |  | |      | |\\/| | / /\\ \\ | . ` | / /\\ \\| | |_ |  __| | |\\/| |  __| | . ` |  | |   \n");
    printf(" ____) |  | |  | |__| | |__| | |____| |\\  |  | |      | |  | |/ ____ \\| |\\  |/ ____ \\ |__| | |____| |  | | |____| |\\  |  | |      \n");
    printf("|_____/   |_|   \\____/|_____/|______|_| \\_|  |_|      |_|  |_/_/    \\_\\_| \\_/_/    \\_\\_____|______|_|  |_|______|_| \\_|  |_|   \n");
    printf("\n");
    printf("                           Made by Bakr Amghar\n");
        Menu();
        scanf("%d", &choix);

        switch(choix){
             case 1:
                printf("\nID : ");
                scanf("%s",id);

                printf("Nom : ");
                scanf("%s",nom);

                printf("Moyenne : ");
                scanf("%f",&moy);

                InsertionTete(id,moy,nom);
                printf("\nEtudiant ajoute en tete.\n");
                Pause();
                break;

            case 2:
                printf("\nID : ");
                scanf("%s",id);

                printf("Nom : ");
                scanf("%s",nom);

                printf("Moyenne : ");
                scanf("%f",&moy);

                InsertionQueue(id,moy,nom);
                printf("\nEtudiant ajoute en queue.\n");
                Pause();
                break;

            case 3:
                ChangerMoy();
                Pause();
                break;

            case 4:
                ChangerNom();
                Pause();
                break;

            case 5:
                Supprimer();
                Pause();
                break;

            case 6:
                TrierParMoyenne();
                Pause();
                break;

            case 7:
                etu = RechercheID();
                AffichagePrecis(etu);
                Pause();
                break;


            case 8:
                etu = RechercheID();
                AffichagePrecis(etu);
                Pause();
                break;

            case 9:
                Affichage();
                Pause();
                break;

            case 10:
                 CalculeTaille();
                 Pause();
                 break;

            case 11:
                Moyenne();
                Pause();
                break;

            case 12:
                SauvegarderListe();
                Pause();
                break;

            case 13:
                LoadEtudiants();
                break;

            case 14:
                VerificationDesAnomalies();
                Pause();
                break;


            case 0:
                printf("\nProgramme termine.\n");
                break;

            default:
                printf("\nChoix invalideeeeee\n\n");
        }

    }while(choix != 0);
return 0;
}
