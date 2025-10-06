/* File: structures.h
*  Student: Natacha Sousa, 107413
*  Description: This files contains all the structures of vaccines.
*/

#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_VACCINATIONS 1000
#define MAX_BATCH 20
#define MAX_NAME 50
#define HASH_TABLE 1001

typedef struct listVaccine *list_vaccines;

typedef struct {
    int day;
    int month;
    int year;
} date;

typedef struct {
    char name[MAX_NAME];
    char batch[MAX_BATCH];
    date expirationDate; 
    unsigned int portions; 
    unsigned int applications;
} vaccineBatch;

typedef struct {
    char *user_name; 
    char batch_withdrawn [MAX_BATCH];
    date applicationDate;
} Application;

typedef struct node_application {
    Application application;
    struct node_application *next;
} node_application;

typedef struct {
    node_application *head;
    node_application *tail;
} applicationList;

typedef struct node_vaccine {
    vaccineBatch vaccine;

    struct node_vaccine *next;
    struct node_vaccine *head, *tail;
} *node_vac;

typedef struct listVac {
    int num_vaccines;

    vaccineBatch vaccine;
    list_vaccines ptr_list_vaccine;
    node_vac head, tail;

} l_Vaccines;

typedef struct node_date {
    date current_date;
    struct node_date* next;
} node_date;

typedef struct {
    node_date* head;
    node_date* tail;
} list_date;

typedef struct user_application_node{
    Application application;
    struct user_application_node *next;
} user_application_node;

typedef struct user_hash_entry {
    char *user_name;
    struct user_application_node *applications;
    struct user_hash_entry *next;
} user_hash_entry;

typedef struct userList {
    user_hash_entry *user;
    struct userList *next;
    struct userList *prev;
} userList;

typedef struct user_hash_table {
    user_hash_entry *table[HASH_TABLE];
    struct userList *insertion_order_head;
    struct userList *insertion_order_tail;
} user_hash_table;

#endif /*STRUCTURES_H*/