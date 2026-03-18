#include "etudiant.h"
#include <stdlib.h>
#include <string.h>

// Actual variables live here
etudiant *head = NULL;
etudiant *queue = NULL;

etudiant* CreerEtudiant(char id[], float moy, char nom[]){
    etudiant *nv = (etudiant*)malloc(sizeof(etudiant));
    if(!nv) return NULL;
    strcpy(nv->id, id);
    nv->moyenne = moy;
    strcpy(nv->nom, nom);
    nv->next = NULL;
    nv->prev = NULL;
    return nv;
}

void InsertionQueue(char id[], float moy, char nom[]){
    etudiant *nv = CreerEtudiant(id, moy, nom);
    if(queue == NULL){
        head = queue = nv;
    } else {
        queue->next = nv;
        nv->prev = queue;
        queue = nv;
    }
}

etudiant* RechercheID_GUI(char id_to_find[]){
    etudiant *temp = head;
    while(temp != NULL){
        if(strcmp(temp->id, id_to_find) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

void Modifier_GUI(char old_id[], char new_id[], char new_nom[], float new_moy) {
    etudiant *temp = RechercheID_GUI(old_id);
    if (temp != NULL) {
        strcpy(temp->id, new_id);
        strcpy(temp->nom, new_nom);
        temp->moyenne = new_moy;
    }
}

void Supprimer_GUI(char id_to_delete[]){
    etudiant *etu = RechercheID_GUI(id_to_delete);
    if(etu == NULL) return;
    if(etu->prev == NULL && etu->next == NULL){
        head = queue = NULL;
    } else if(etu->prev == NULL){
        head = etu->next;
        head->prev = NULL;
    } else if(etu->next == NULL){
        queue = etu->prev;
        queue->next = NULL;
    } else {
        etu->prev->next = etu->next;
        etu->next->prev = etu->prev;
    }
    free(etu);
}

void ViderListe() {
    etudiant *current = head;
    while (current != NULL) {
        etudiant *next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
    queue = NULL;
}