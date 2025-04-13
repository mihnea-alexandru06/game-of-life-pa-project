#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "list.h"

listnode_t* create_node(const int l, const int c) {
    listnode_t *ptr = malloc(sizeof(listnode_t));

    if(ptr == NULL)
        return NULL;
    
    //Initializez
    ptr->coords.l = l;
    ptr->coords.c = c;
    ptr->next = NULL;

    return ptr;
}

void insert_sorted(listnode_t **head_ptr, const int l, const int c) {
    listnode_t *ptr = create_node(l, c);
    if(ptr == NULL)
        return;
    
    listnode_t *curr;

    //Daca lista e goala sau noul listnode_t trebuie inserat inaintea head-ului
    if(*head_ptr == NULL || (*head_ptr)->coords.l > l || (((*head_ptr)->coords.l) == l && (*head_ptr)->coords.c > c)) {
        ptr->next = *head_ptr;
        *head_ptr = ptr;
    }else {
        curr = *head_ptr;

        /*
        Parcurg pana cand gasesc pozitia corecta, cam lunga expresia asa ca
        Initial am avut:

        while(curr->next != NULL && (curr->next->coords.l < l || (curr->next->coords.l == l && curr->next->coords.c < c)))
        curr = curr->next;

        Care face aceeasi chestie, dar arata foarte urat...
        */

        //Cat timp exista nod urmator
        while(curr->next != NULL) {
            //Obtin linia si coloana urmatorului nod
            int next_l = curr->next->coords.l;
            int next_c = curr->next->coords.c;

            //Daca nodul urmator este dupa pozitia unde inserez, dau break
            if (next_l > l || (next_l == l && next_c >= c))
                break;

            //Avansez
            curr = curr->next;
        }

        //Inserez
        ptr->next = curr->next;
        curr->next = ptr;
    }
}

void free_list(listnode_t *head_ptr) {
    listnode_t *ptr = head_ptr;
    listnode_t *next;

    //Dau free la fiecare listnode_t in parte, parcurgand
    while(ptr != NULL) {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
}

void print_list(FILE *out, listnode_t *head_ptr) {
    listnode_t *ptr = head_ptr;
    
    //Afisez lista
    while(ptr != NULL) {
        fprintf(out, " %d %d", ptr->coords.l, ptr->coords.c);
        ptr = ptr->next;
    }

    fprintf(out, "\n");
}