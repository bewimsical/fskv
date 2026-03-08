#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arm/limits.h>

#include "linkedlist.h"

int count_commas(const char* input) {
    int commas = 0;

    while (*input != '\0') {
        if (*input == ',') {
            commas++;
        }
        input++;
    }
    return commas;
}

long convert_key(const char* key) {
    char* endptr;
    errno = 0;
    //convert key to a long
    const long key_val = strtol(key, &endptr, 10);
    // check for error
     if (errno != 0 || endptr == key || *endptr != '\0' || key_val <= 0) {
         fprintf(stderr, "Invalid key value\n");
         return(LONG_MAX);
     }
    return key_val;
}

void split_string(char* command, int count, const char *data[]) {

    char *token;

    //separate into array at the comma
    if ((token = strtok(command, ",")) != NULL) {
        data[0] = token;

        for (int i = 1; i < count; ++i) {
            if ((token = strtok(NULL, ",")) != NULL){
                data[i] = token;
            }
        }
    }
}

int main(const int argc, char* argv[]){

    struct linked_list data_list;
    init(&data_list);

    FILE *fp = fopen("../database.txt", "r");
    //cannot open file
    if (fp == NULL) {
        printf("fskv: cannot open file\n");//hmm?
        return(1);
    }
    //setup char buffer for fgets
    const int buffer_size = 200;
    char buffer[buffer_size];
    //set up token for splitting line at comma
    char *token;

    //read and add lines to list until the end of the file
    while (fgets(buffer, buffer_size, fp)) {
        // Replace the newline with a null terminator
        buffer[strcspn(buffer, "\r\n")] = '\0';
        //check comma count
        int commas = 0;
        commas = count_commas(buffer);
        if (commas != 1) {
            fprintf(stderr, "Bad data from database.\n");
            return 1;
        }

        //setup array to store the kv pairs from db
        const char *data[2] = {0,0};

        //separate into array at the comma
         if ((token = strtok(buffer, ",")) != NULL) {
             data[0] = token;
             if ((token = strtok(NULL, ",")) != NULL){
                 data[1] = token;
             }

            //check if the key is null --
            if (data[0] != NULL) {
                //convert the key to a long
                long key_val = convert_key(data[0]);
                //check if there was a conversion error
                if (key_val == LONG_MAX || key_val == LONG_MIN) {
                    fprintf(stderr, "Invalid key value. Keys must be integers.\n");
                    return 1;
                }
                //cast key to an integer
                const int key = (int) key_val;
                //add to linked list
                add(&data_list, key, data[1]);
            }
        }
    }
    //close the file
    fclose(fp);

    for (int i = 1; i < argc; ++i) {
        char* command = argv[i];
        int commas = count_commas(command);
        //command must be c or a
        if (commas == 0) {
            //command is c - clear the database. an "are you sure you want to clear the database" would be nice here.
            if (*command == 'c') {
                clear(&data_list);
                //command is a - print the database
            }else if ( *command == 'a') {
                struct node *current = data_list.head;
                while (NULL != current) {
                    printf("%d, %s\n", current->data.key, current->data.name);
                    current = current->next;
                }
            }else {
                fprintf(stderr, "bad command\n");
            }
            // command is g or d
        }else if (commas == 1) {
            const char *data[2] = {0,0};
            //split into an array
            split_string(command,commas+1,data);
            int key;
            if (data[1] != NULL) {
                //convert the key to a long
                long key_val = convert_key(data[1]);
                //check if there was a conversion error
                if (key_val == LONG_MAX || key_val == LONG_MIN) {
                    fprintf(stderr, "Invalid key value. Keys must be integers.\n");
                    return 1;
                }
                //cast key to an integer
                key = (int) key_val;
            }else {
                fprintf(stderr, "Missing key value.\n");
                return 1;
            }
            //command is g - print the database entry
            if (*data[0] == 'g') {
                struct node *the_node = find(&data_list, key);
                if (the_node != NULL) {
                    printf("%d,%s\n", key, the_node->data.name);
                }else {
                    printf("%d not found\n", key);
                }
                //command is d - delete the database entry
            }else if (*data[0] == 'd') {
                if (false ==  delete(&data_list, key)) {
                    printf("%d not found\n", key);
                }
            }else {
                fprintf(stderr, "bad command\n");
            }

            //command is p
        }else if (commas == 2) {
            const char *data[3] = {0,0,0};
            //spit into an array
            split_string(command,commas+1,data);
            int key;
            if (data[1] != NULL) {
                //convert the key to a long
                long key_val = convert_key(data[1]);
                //check if there was a conversion error
                if (key_val == LONG_MAX || key_val == LONG_MIN) {
                    fprintf(stderr, "Invalid key value. Keys must be integers.");
                    return 1;
                }
                //cast key to an integer
                key = (int) key_val;
            }else {
                fprintf(stderr, "Missing key value.\n");
                return 1;
            }
            //command is p - put entry into the database
            if (*data[0] == 'p') {
                struct node *the_node = find(&data_list, key);
                if (the_node != NULL) {
                    //free the node
                    free(the_node->data.name);
                    //malloc new pointer
                    char* p_name =  (char*) malloc(strlen(data[2])+1);
                    //copy value into the new pointer
                    strcpy(p_name, data[2]);
                    //assign to data.name
                    the_node->data.name = p_name;
                }else {
                    add(&data_list,key,data[2]);
                }
            }else {
                fprintf(stderr, "bad command\n");
            }
        }else {
            fprintf(stderr, "bad command\n");
        }
    }

    //clear the database
    fp = fopen("../database.txt", "w");
    //add data from list into database
    if (fp == NULL) {
        printf("fskv: cannot open file\n");//hmm?
        return(1);
    }

    struct node *current = data_list.head;
    while (NULL != current) {
        printf("%d, %s\n", current->data.key, current->data.name);
        fprintf(fp,"%d, %s\n", current->data.key, current->data.name);

        current = current->next;

    }
    fclose(fp);
    return 0;
}