//
// Created by Katie Wims on 2/26/26.
//

#include "linkedlist.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//TODO difference between using NULL and 0?

void init(struct linked_list* list) {
    list->head = NULL;
    // the_list->tail = 0;
    // srand(time(0));
    // the_list->magic_number = rand();
}

//should I return true or false?
//TODO change data to a key and a name
//TODO do i even need the data struct?
void add(struct linked_list* list, const int key, const char* name) {
    //allocate memory for the data and nodes
    struct node* p_node = (struct node*) malloc(sizeof(struct node));
    //struct data data = (struct data*) malloc(sizeof(struct data));
    char* p_name =  (char*) malloc(strlen(name)+1);
    //check if allocation was successful
    if (!p_node || !p_name) {
        //free both of them
        printf("Memory allocation failed\n");
        exit(1);//return instead
    }
    //add data
    strcpy(p_name, name);

    p_node->data.key = key;
    p_node->data.name = p_name;

    //add note to list
    p_node->next = list-> head;
    list->head = p_node;

    //tail stuff
}

struct node* find(struct linked_list* list, int key) {
    struct node *current = list->head;

    while (current != NULL) {
        if (current->data.key == key) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
//private helper method
static void free_node(struct node* node) {
    //free the name pointer
    free(node->data.name);
    //node->data.name = 0;
    //free the data pointer
    //free(node->data);
    //free the node pointer
    free(node);
}

bool delete(struct linked_list* list, const int key) {
    //trail current "tc"
    struct node *previous = NULL;
    struct node *current = list->head;

    if (current == NULL) {
        return false;
    }
    while (current!= NULL) {
        if (current->data.key == key) {
            if (previous == NULL) {
                list->head = current->next;
            }else{
                previous->next = current->next;

            }
            //deallocate memory
            free_node(current);
            return true;

        }
        previous = current;
        current = current->next;
    }
    return false;
}

void clear(struct linked_list* list) {
    struct node *current = list->head;
    while (current!= NULL) {
        struct node *next = current->next;
        free_node(current);
        current = next;
    }
    list->head = NULL;
}


