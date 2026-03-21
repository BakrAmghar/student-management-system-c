#include "etudiant.h"
#include <stdlib.h>
#include <string.h>

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

int InsertionQueue(char id[], float moy, char nom[]){
    if (RechercheID_GUI(id) != NULL) return 0; // Check for duplicate before adding

    etudiant *nv = CreerEtudiant(id, moy, nom);
    if(!nv) return 0;

    if(queue == NULL){
        head = queue = nv;
    } else {
        queue->next = nv;
        nv->prev = queue;
        queue = nv;
    }
    return 1;
}

etudiant* RechercheID_GUI(char id_to_find[]){
    etudiant *temp = head;
    while(temp != NULL){
        if(strcmp(temp->id, id_to_find) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

int Modifier_GUI(char old_id[], char new_id[], char new_nom[], float new_moy) {
    // BUG FIX: If user is trying to change to an ID that someone else already has
    if (strcmp(old_id, new_id) != 0) {
        if (RechercheID_GUI(new_id) != NULL) {
            return 0; // Failure: New ID already exists
        }
    }

    etudiant *temp = RechercheID_GUI(old_id);
    if (temp != NULL) {
        strcpy(temp->id, new_id);
        strcpy(temp->nom, new_nom);
        temp->moyenne = new_moy;
        return 1; // Success
    }
    return 0;
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

// --- NEW SORTING LOGIC ---

int compID(const void *a, const void *b) {
    return strcmp((*(etudiant**)a)->id, (*(etudiant**)b)->id);
}

int compNom(const void *a, const void *b) {
    return strcmp((*(etudiant**)a)->nom, (*(etudiant**)b)->nom);
}

int compMoy(const void *a, const void *b) {
    float m1 = (*(etudiant**)a)->moyenne;
    float m2 = (*(etudiant**)b)->moyenne;
    if (m1 < m2) return 1; // Descending for Ranking
    if (m1 > m2) return -1;
    return 0;
}

void TrierListe(int mode) {
    if (!head || !head->next) return;

    int count = 0;
    etudiant *temp = head;
    while(temp) { count++; temp = temp->next; }

    etudiant **arr = (etudiant**)malloc(count * sizeof(etudiant*));
    temp = head;
    for(int i=0; i<count; i++) { arr[i] = temp; temp = temp->next; }

    if(mode == 1) qsort(arr, count, sizeof(etudiant*), compID);
    else if(mode == 2) qsort(arr, count, sizeof(etudiant*), compNom);
    else qsort(arr, count, sizeof(etudiant*), compMoy);

    head = arr[0];
    head->prev = NULL;
    for(int i=0; i<count-1; i++) {
        arr[i]->next = arr[i+1];
        arr[i+1]->prev = arr[i];
    }
    queue = arr[count-1];
    queue->next = NULL;
    free(arr);
}