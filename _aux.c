/* File: aux.c
*  Student: Natacha Sousa, 107413
*  Description: This file contains all the auxiliary functions that are used in the project.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "aux.h"
#include "structures.h"
#include "vaccines.h"

/* This function prints de message, if the user want's in portuguese or english
*  @param flag
*  @param *en_message - message in english
*  @param *pt_message - message in potuguese
*/
void printMessage(int flag, const char *en_message, const char *pt_message) {
    if (flag == 1){
        printf("%s\n", pt_message);
    } else {
        printf("%s\n", en_message);
    }
}

/* This function checks if the function runs out of memory.
*  @param flag - language flag, 0 english, 1 portuguese
*/
void noMemory(int flag) {
    printMessage (flag, "no memory", "sem memória");
    exit (EXIT_FAILURE);
}

/* This function creates a vector to save a bottom.
*  @param size - vector size
*  @param flag - language flag, 0 english, 1 portuguese
*  @return vector - a pointer to allocated vector
*/
char *createVectorItr (int size, int flag) {
    char *vetor = (char*)malloc(size *sizeof(char));
    if (vetor == NULL) noMemory(flag);
    return vetor;
}

/* This function reads an instruction.
*  @param *itr_empty - buffer to store the input string
*/
void readInstruction (char *itr_empty) {
    int i = 0;
    char j;
    while ((j = getchar()) != '\n') {
        itr_empty [i++] = j;
    }
    itr_empty [i] = '\0';
}

/* This function counts de spaces until the next word
*  @param **itr - pointer to the instruction string
*  @param **word - pointer to the word extreacted
*  @param flag - language flag, 0 english, 1 portuguese
*/
int readWord (char **itr, char **word, int flag) {
    int count = 0, i = 0, start, n, j, tab;
    char *instruction = *itr;
    while (instruction[i] == ' ' || instruction[i] == '\t') {
        i++;
    }
    if (instruction[i] == '"') {
        i++;
        start = i;
        while (instruction[i++] != '"')
            count++;
    }
    else{
        start = i;
        while (instruction[i] != ' ' && instruction[i] != '\t' 
            && instruction[i] != '\0') {
                i++;
                count++;
        }
    }
    if (count == 0) return 0;
    *word = (char*)malloc((count + 1) * sizeof(char));
    if (*word == NULL) noMemory(flag);
    for (n = start, j = 0; j < count; n++, j++) 
        (*word)[j] = instruction[n];
    (*word)[j] = '\0';
    tab = i;
    (*itr) += tab;
    return tab;
} 

/* This function trims a string, it takes the spaces in a string.
 *  @param *str - string to trim
 *  @return str - pointer to the trimmed string
*/
char* trimString(char *str) {
    while (isspace((unsigned char)*str)) str++; 
    if (*str == 0) return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--; 
    *(end + 1) = '\0'; 
    return str;
}

/* This function reads a date.
*  @param *entr - string containing date
*  @param *dt - pointer to date structure to fill
*  @return 1 in case of success or 0 in case of failure
*  @param flag - language flag, 0 english, 1 portuguese
*/
int readDate (const char *entr, date *dt, int flag) {
    char *ptr;
    date temp;
    temp.day = strtol(entr, &ptr, 10);
    temp.month = strtol(ptr + 1, &ptr, 10);
    temp.year = strtol(ptr + 1, NULL, 10);
    if (temp.year < 2025 || temp.month < 1 || temp.month > 12 
        || temp.day < 1 || temp.day > 31) {
        printMessage (flag, "invalid date", "data inválida");
        return 0; 
    }
    if ((temp.month == 4 || temp.month == 6 || temp.month == 9 
        || temp.month == 11) && temp.day > 30) {
        printMessage (flag, "invalid date", "data inválida");        
        return 0;
    }
    int leap_year = (temp.year % 4 == 0 && 
        (temp.year % 100 != 0 || temp.year % 400 == 0));
    if (temp.month == 2) {
        if (temp.day > (leap_year ? 29 : 28)) {
            printMessage (flag, "invalid date", "data inválida");
            return 0;
        }
    }
    if (temp.year == 2015 && (temp.month < 1 || (temp.month == 1 &&
                                                     temp.day < 1))){
        printMessage (flag, "invalid date", "data inválida");
        return 0;
    }
    *dt = temp;
    return 1;
}

/* This function checks if the date on the input is after the last saved date.
*  @param newDate - new date
*  @param lastDate - reference date
*  @return 1 in case of failure or 0 in case of success
*/
int isAfter (date newDate, date lastDate) {
    if (newDate.year > lastDate.year) return 1;
    if (newDate.year == lastDate.year &&
        newDate.month > lastDate.month) return 1;
    if (newDate.year == lastDate.year && newDate.month == lastDate.month &&
        newDate.day > lastDate.day) return 1;
    if (newDate.year == lastDate.year && newDate.month == lastDate.month &&
        newDate.day == lastDate.day) return 1;
    return 0;
}

/* This function checks if the name is valid.
*  @param *name - string name to validate
*  @return 1 in case of failure or 0 in case of success
*  @param flag - language flag, 0 english, 1 portuguese
*/
int validName (const char *name, int flag){
    if (strlen(name) > MAX_NAME){
        printMessage (flag, "invalid name", "nome inválido");
        return 1;
    }
    for (int i = 0; name[i] != '\0'; i++){
        if (isspace(name[i])){
            printMessage (flag, "invalid name", "nome inválido");
            return 1;
        } 
    }
    return 0;
}

/* This function checks if a batch is valid.
*  @param *batch -string batch
*  @param flag - language flag, 0 english, 1 portuguese
*  @return 1 in case of failure or 0 in case of success
*/
int validBatch (char *batch, int flag){
    if (batch == NULL){
        printMessage (flag, "invalid batch", "lote inválido");
    }
    size_t batch_length = strlen(batch);
    if (batch_length > MAX_BATCH) { 
        printMessage (flag, "invalid batch", "lote inválido");
        return 1;
    }
    for (size_t i = 0; i < strlen(batch); i++) {
        if(!isxdigit(batch[i]) || (isalpha(batch[i]) && !isupper(batch[i]))){
            printMessage (flag, "invalid batch", "lote inválido");
            return 1;
        }
    }
    return 0;
}

/* This function add a date to a list of dates.
*  @param *date_list - pointer to a list
*  @param new_date - date to add
*  @param flag - language flag, 0 english, 1 portuguese
*/
void addDateList (list_date *date_list, date new_date, int flag) {
    node_date* new_node = (node_date*) malloc(sizeof(node_date));
    if (!new_node){
        noMemory(flag);
    }
    new_node->current_date = new_date;
    new_node->next = NULL;
    if (date_list->tail == NULL){
        date_list->head = new_node;
        date_list->tail = new_node;
    } else {
        date_list->tail->next = new_node;
        date_list->tail = new_node;
    }
}

/* This function search the date list and get the last date inserted.
*  @param *date_list - pointer to the list
*  @return last_date, is the last date inserted in the list
*/
date getLastDate(list_date *date_list) {
    if (date_list->head == NULL) {
        date aux_date = {0,0,0};
        return aux_date;
    }
    date last_date = date_list->head->current_date;
    node_date* current = date_list->head->next;
    while (current != NULL) {
        if (isAfter(current->current_date, last_date)){
            last_date = current->current_date;
        }
        current = current->next;
    }
    return last_date;
}

/* This function compare the batches of the vaccines.
*  @param *a - first batch
*  @param *b - second batch
*  @return result of strcmp: <0, 0, >0
*/
int compareBatches (vaccineBatch *a, vaccineBatch *b) {
    if (a->expirationDate.year != b->expirationDate.year)
        return a->expirationDate.year - b->expirationDate.year;
    if (a->expirationDate.month != b->expirationDate.month)
        return a->expirationDate.month - b->expirationDate.month;
    if (a->expirationDate.day != b->expirationDate.day)
        return a->expirationDate.day - b->expirationDate.day;
    return strcmp (a->batch, b->batch);
}

/* This function orders the vaccines.
*  @param *list - pointer to the list
*  @param new_batch - batch to insert
*  @param flag - language flag, 0 english, 1 portuguese
*/
void insertVaccineSorted (l_Vaccines *list, vaccineBatch new_batch, int flag){
    node_vac new_node = (node_vac) malloc (sizeof(struct node_vaccine));
    if (!new_node){
        noMemory(flag);
    }
    new_node->vaccine = new_batch;
    new_node->next = NULL;
    if (list->head == NULL || compareBatches(&new_batch, &list->head->vaccine) < 0){
        new_node->next = list->head;
        list->head = new_node;
        return;
    }
    node_vac current = list->head;
    while (current->next != NULL && 
        compareBatches (&new_batch, &current->next->vaccine) > 0){
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
}

/* This function verifies if the date is before or equal 
 * to the actual date in the sistem.
 * @param d1 - first date
 * @param d2 - second date
 * @return 1 if d1 <= d2, otherwise 0
 */
int isBeforeOrEqual(date d1, date d2) {
    if (d1.year < d2.year) {
        return 1; 
    } else if (d1.year > d2.year) {
        return 0;  
    } else { 
        if (d1.month < d2.month) {
            return 1; 
        } else if (d1.month > d2.month) {
            return 0; 
        } else {
            if (d1.day <= d2.day) {
                return 1; 
            } else {
                return 0; 
            }
        }
    }
}

/* This function verifies if the date inserted is equal
 *  @param d1 - first date
 *  @param d2 - second date
 *  @return 1 if dates are equal 0 otherwise
*/
int equalDate(date d1, date d2) {
    return (d1.year == d2.year && d1.month == d2.month && d1.day == d2.day);
}

/* This funcion inicializes the hash table.
 * @param hash_table - pointer to the hash table to inicialize
*/
void inicializeHashTable (user_hash_table *hash_table) {
    for (int i = 0; i < HASH_TABLE; i++) {
        hash_table->table[i] = NULL;
    }
    hash_table->insertion_order_head = NULL;
    hash_table->insertion_order_tail = NULL;
}

/* Hash function
 * @param str - String to hash
 * @return result - hash value module HASH_TABLE
 */
unsigned long hash (const char *str) {
    unsigned long hash = 5381;
    int aux;
    while ((aux = *str++)) {
        hash = ((hash << 5) + hash) + aux;
    }
    unsigned long result = hash % HASH_TABLE;  
    return result;
}

/* Adds an application to user in hash table, if doesen't exist creates a new entry.
 * @param hash_table - pointer to hash table
 * @param application - application that is going to be added
 */
void addUserApplication(user_hash_table *hash_table, Application application) {
    unsigned long index = hash(application.user_name);
    user_hash_entry *entry = hash_table->table[index];

    if (application.user_name == NULL || strlen(application.user_name) == 0) {
        return;
    }
    while (entry != NULL) {
        if (strcmp(entry->user_name, application.user_name) == 0) {
            user_application_node *new_application = 
            malloc(sizeof(user_application_node));
            if (new_application == NULL) {
                return;
            }
            new_application->application = application;
            new_application->next = NULL;
            if (entry->applications == NULL) {
                entry->applications = new_application;
            } else {
                user_application_node *current = entry->applications;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = new_application;
            }
            return;
        }
        entry = entry->next;
    }
    entry = malloc(sizeof(user_hash_entry));
    if (entry == NULL) return;
    entry->user_name = malloc(strlen(application.user_name) + 1);
    if (entry->user_name == NULL) {
        free(entry);
        return;
    }
    strcpy(entry->user_name, application.user_name);
    entry->applications = malloc(sizeof(user_application_node)); 
    if (entry->applications == NULL) {
        free(entry->user_name);
        free(entry);
        return;
    }
    entry->applications->application = application;
    entry->applications->next = NULL;
    entry->next = hash_table->table[index];
    hash_table->table[index] = entry;
}

/* Verifies if a user has already been vaccinated with a specific vaccination batch,
 * on a given date.
 * @param hash_table - pointer to the hash table.
 * @param user_name - name of the user.
 * @param batch - batch identifier.
 * @param actual_date - date to check.
 * @return 1 if user is already vaccinated with the same batch on the given date,
 *  0 otherwise
 */
int alreadyVaccinatedHash (user_hash_table *hash_table, const char *user_name,
                           const char *batch, date actual_date) {
    unsigned long index = hash(user_name);
    user_hash_entry *entry = hash_table->table[index];
    while (entry != NULL) {
        if(strcmp(entry->user_name, user_name) == 0) {
            user_application_node * current = entry->applications;
            while (current != NULL) {
                if(strcmp(current->application.batch_withdrawn, batch) == 0 &&
                    current->application.applicationDate.day == actual_date.day &&
                    current->application.applicationDate.month == actual_date.month &&
                    current->application.applicationDate.year == actual_date.year) {
                    return 1; 
                }
                current = current->next;
            }
            return 0;
        }
        entry = entry->next;
    }
    return 0;
}

/* Converts a string to lowercase.
 * @param str - string to convert
 */
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}