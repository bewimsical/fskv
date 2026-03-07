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
                    fprintf(stderr, "Invalid key value. Keys must be integers.");
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
        const char* command = argv[i];
        int commas = count_commas(command);
        //command must be c or a
        if (commas == 0) {
            //command is c - clear the database. an "are you sure you want to clear the database" would be nice here.
            if (*command == 'c') {
                clear(&data_list);
            //print the database
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
            //split into an array
            //check 0 index
            //perform task
        //command is p
        }else if (commas == 2) {
            //spit into an array
            //check 0 index
            //perfrom task
        //commas greater than 2 - this is a bad command
        }else {
            fprintf(stderr, "bad command\n");
        }

    }

    //clear the database

    //add data from list into database

    //test that the data is in the list
    struct node *current = data_list.head;
    while (NULL != current) {
        printf("%d, %s\n", current->data.key, current->data.name);
        current = current->next;

    }
        return 0;
}