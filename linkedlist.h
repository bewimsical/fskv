//
// Created by Katie Wims on 2/26/26.
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct data {
    int key;
    //TODO should I hardcode a fixed sized string? char name[100]; ?
    char* name;

};
//make this a typedef
//add a magic number
struct node {
    struct data data;
    struct node* next;
};

//add tail?
struct linked_list {
    struct node* head;
};
//add init
void init(struct linked_list* list);
void add(struct linked_list* list, int key, const char* name);
struct node* find(struct linked_list* list, int key);
void delete(struct linked_list* list, const int key);
void clear(struct linked_list* list);

#endif //LINKEDLIST_H
