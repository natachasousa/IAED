/* File: vaccines.c
*  Student: Natacha Sousa, 107413
*  Description: This file contains all the functions to the vaccination management system..
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "vaccines.h"
#include "aux.h"
#include "structures.h"

/* Adds a new vaccine batch to the vaccine list.
 * @param *itr - pointer to input string
 * @param *vac_list - pointer tho vaccine list
 * @param *date_list - pointer to the date list
 * @param flag - language flag, 0 english, 1 portuguese
 * @param check_date - verifies if the experation date is after the last date
 * 
 */
void addBatch (char *itr, l_Vaccines *vac_list, list_date *date_list,
               int flag, int check_date){
    char *batch = NULL, *dateStr  = NULL, *name = NULL, *portionsStr = NULL;
    date expirationDate = {1,1,2025};
    unsigned int portions = 0;
    if(!readWord(&itr, &batch, flag) || batch == NULL) {
        printMessage (flag, "invalid batch", "lote inválido");
        return;
    }
    trimString(batch);
    if(!readWord(&itr,&dateStr, flag) || dateStr == NULL) {
        printMessage (flag, "invalid date", "data inválida");
        free(batch);
        return;
    }
    if(!readWord(&itr, &portionsStr, flag) || portionsStr == NULL) {
        printMessage (flag, "invalid quantity", "quantidade inválida");
        free(batch);
        free(dateStr);
        return;
    }
    if(!readWord(&itr, &name, flag) || name == NULL) {
        printMessage (flag, "invalid name", "nome inválido");
        free(batch);
        free(dateStr);
        free(portionsStr);
        return;
    }
    if(validBatch(batch, flag)) return;
    node_vac current = vac_list->head;
    while (current != NULL){
        if (strcmp(current->vaccine.batch, batch) == 0){
            printMessage (flag, "duplicate batch number",
                             "número de lote duplicado");
            return;
        }
        current = current->next;
    }
    if (!readDate(dateStr, &expirationDate, flag)) return;
    if (check_date) {
        date last_date = getLastDate(date_list);
        if (!isAfter(expirationDate, last_date)) {
            printMessage (flag, "invalid date", "data inválida");
            free (batch);
            free (dateStr);
            free (portionsStr);
            free (name);
            return;
        }
    }
    portions = atoi(portionsStr);
    if (portions <= 0){
        printMessage (flag, "invalid quantity", "quantidade inválida");
        return;
    }
    if(validName(name, flag)) return;
    if (vac_list->num_vaccines >= MAX_VACCINATIONS){
        printMessage (flag, "too many vaccines", "demasiadas vacinas");
        return;
    }
    node_vac newNode = (node_vac) malloc(sizeof(struct node_vaccine));
    if (!newNode){
        noMemory();
    }
    strcpy(newNode->vaccine.name, name);
    strcpy(newNode->vaccine.batch, batch);
    newNode->vaccine.expirationDate = expirationDate;
    newNode->vaccine.portions = portions;
    newNode->next = NULL;
    if (vac_list->head == NULL){
        vac_list->head = newNode;
        vac_list->tail = newNode;
    } else {
        vac_list->tail->next = newNode;
        vac_list->tail = newNode;
    }
    vac_list->num_vaccines++;
    printf ("%s\n", batch);
    free(batch);
    free(dateStr);
    free(portionsStr);
    free(name);
}

/* List the vaccines information.
 * @param *itr - pointer to input string 
 * @param *vaccine_list - pointer to vaccine list
 * @param flag - language flag, 0 english, 1 portuguese
 */
void listVaccines (char *itr, l_Vaccines *vaccine_list, int flag) {
    char *vaccine_name = NULL;
    char *vaccine_names[MAX_VACCINATIONS]; 
    int name_count = 0; 
    while (readWord(&itr, &vaccine_name, flag)) {
        vaccine_names[name_count++] = vaccine_name;
    }
    int count = 0;
    node_vac current;
    if (name_count == 0) {
        l_Vaccines sorted_list = {0, {"", "", {0, 0, 0}, 0, 0}, NULL, NULL, NULL};
        current = vaccine_list->head;
        while (current != NULL) {
            insertVaccineSorted(&sorted_list, current->vaccine, flag);
            count++;
            current = current->next;
        }
        current = sorted_list.head;
        while (current != NULL) {
            printf("%s %s %02d-%02d-%04d %u %u\n",
                current->vaccine.name,
                current->vaccine.batch,
                current->vaccine.expirationDate.day,
                current->vaccine.expirationDate.month,
                current->vaccine.expirationDate.year,
                current->vaccine.portions,
                current->vaccine.applications);
            current = current->next;
        }
        while (sorted_list.head != NULL) {
            node_vac aux = sorted_list.head;
            sorted_list.head = sorted_list.head->next;
            free(aux);
        }
    } else {
        for (int i = 0; i < name_count; i++) {
            current = vaccine_list->head;
            int found = 0;
            while (current != NULL) {
                if (strcmp(trimString(current->vaccine.name),
                    trimString(vaccine_names[i])) == 0) {
                    printf("%s %s %02d-%02d-%04d %u %u\n",
                        current->vaccine.name,
                        current->vaccine.batch,
                        current->vaccine.expirationDate.day,
                        current->vaccine.expirationDate.month,
                        current->vaccine.expirationDate.year,
                        current->vaccine.portions,
                        current->vaccine.applications);
                    found = 1;
                }
                current = current->next;
            }
            if (!found) {
                printf(flag ? "%s: vacina inexistente\n" : "%s: no such vaccine\n",
                       vaccine_names[i]);
            }
            free(vaccine_names[i]); 
        }
    }
}

/* Applies a vaccine to a user if available and not already vaccinated.
 * @param *itr - pointer to input string 
 * @param *vaccine_list - pointer to vaccine list
 * @param actual_date - current application date
 * @param *user_hash - pointer to user hash table
 * @param flag - language flag, 0 english, 1 portuguese
 */
void vaccinationApplication(char *itr, l_Vaccines *vaccine_list, date actual_date, 
                            user_hash_table *user_hash, int flag) {
    char *user_name = strdup(itr);
    if (user_name == NULL) {
        noMemory(flag);
        return;
    }
    int vaccine_found = 0;
    if (user_name == NULL) {
        noMemory(flag);
    }
    char *vaccine_name = (char *)malloc(MAX_NAME * sizeof(char));
    if (vaccine_name == NULL) {
        noMemory(flag);
    }
    Application *user_applications = (Application *) malloc(sizeof(Application));
    if (user_applications == NULL) {
        noMemory(flag);  
        free(user_name);
        free(vaccine_name);
        return;
    }
    user_applications->user_name = strdup(itr);
    if (user_applications->user_name == NULL) {
        noMemory(flag);
        free(user_applications); 
        free(user_name);
        free(vaccine_name);
        return;
    }
    user_applications->user_name[0] = '\0'; 
    user_applications->applicationDate.year = 0;
    user_applications->applicationDate.month = 0;
    user_applications->applicationDate.day = 0;
    if (!readWord(&itr, &user_name, flag)) {
        free(user_name);
        free(vaccine_name);
        return;
    }
    if (!readWord(&itr, &vaccine_name, flag)) {
        free(user_name);
        free(vaccine_name);
        return;
    }
    node_vac current = vaccine_list->head;
    node_vac selected_vaccine = NULL;
    while (current != NULL) {
        if (strcmp(trimString(current->vaccine.name), 
            trimString(vaccine_name)) == 0) {
            vaccine_found = 1;
            if (current->vaccine.portions > 0) {
                if (!selected_vaccine || compareBatches(&current->vaccine,
                     &selected_vaccine->vaccine) < 0) {
    
                    selected_vaccine = current;   
                }
            }
        }
        current = current->next;
    }
    if (!vaccine_found){
        printMessage(flag, "no stock", "esgotado");
        free (user_name);
        free (vaccine_name);
        return;
    }
    if (selected_vaccine != NULL) {
        if (alreadyVaccinatedHash(user_hash, user_name,
            selected_vaccine->vaccine.batch, actual_date)){
            printMessage(flag, "already vaccinated", "já vacinado");
            free(user_name);
            free(vaccine_name);
            return;
        }else{
            printf("%s\n", selected_vaccine->vaccine.batch);
            strcpy(user_applications->user_name, user_name);
            strcpy(user_applications->batch_withdrawn,
                    selected_vaccine->vaccine.batch);
            user_applications->applicationDate = actual_date;
            addUserApplication (user_hash, *user_applications);
            selected_vaccine->vaccine.portions--;
            selected_vaccine->vaccine.applications++;
        }
    } else {
        printMessage(flag, "no stock", "esgotado"); 
    }
    free(user_name);
    free(vaccine_name);
}

/* List all applications made by user or all users if none is specifeid.
 * @param *itr - pointer to input string
 * @param *hash_table - pointer to user hash table
 * @param flag - language flag, 0 english, 1 portuguese
 */
void listUserApplications(char *itr, user_hash_table *hash_table, int flag) {
    char *user_name = NULL;
    int found = 0;
    if (itr != NULL && strlen(itr) > 0) {
        user_name = strdup(itr);
        if (user_name == NULL) {
            noMemory(flag);
            return;
        }
        user_name = trimString(user_name);
        toLowerCase(user_name);
    }
    if (user_name == NULL || strlen(user_name) == 0) {
        for (int i = 0; i < HASH_TABLE; i++){
           user_hash_entry *entry = hash_table->table[i];
            while (entry != NULL) {
                user_application_node *app = entry->applications;
                while (app != NULL) {
                    printf("%s %s %02d-%02d-%04d\n",
                        app->application.user_name,
                        app->application.batch_withdrawn,
                        app->application.applicationDate.day,
                        app->application.applicationDate.month,
                        app->application.applicationDate.year);
                    app = app->next;
                }
                entry = entry->next;
            }
        }
    } else {
        if (!readWord(&itr, &user_name, flag)) {
            free(user_name);
            return;
        }
        unsigned long index = hash(user_name);
        user_hash_entry *entry = hash_table->table[index];
        while (entry != NULL) {
            entry->user_name = trimString(entry->user_name);
            user_name = trimString(user_name); 
            if (strcmp(entry->user_name, user_name) == 0) {
                user_application_node *current = entry->applications;
                while (current != NULL) {
                    printf("%s %s %02d-%02d-%04d\n", 
                           current->application.user_name,
                           current->application.batch_withdrawn,
                           current->application.applicationDate.day,
                           current->application.applicationDate.month,
                           current->application.applicationDate.year);
                    current = current->next;
                }
                found = 1;
                break;
            }
            entry = entry->next;
        }
        if (!found) {
            printf(flag ? "%s: utente inexistente\n" : "%s: no such user\n",
                   user_name);
        }
    }
}

/* Removes a vaccine batch from the list if unnused 
 * or sets stock to 0.
 * @param *itr - pointer to input string
 * @param *vac_list - pointer to vaccine list
 * @param flag - language flag, 0 english, 1 portuguese
 */
void removeBatch(char *itr, l_Vaccines *vac_list, int flag) {
    char *batch = NULL;
    if (!readWord(&itr, &batch, flag) || batch == NULL) {
        printMessage(flag, "invalid batch", "lote inválido");
        return;
    }
    trimString(batch);
    node_vac current = vac_list->head;
    node_vac previous = NULL;
    while (current != NULL) {
        if (strcmp(current->vaccine.batch, batch) == 0) {
            printf("%u\n", current->vaccine.applications);
            if (current->vaccine.applications == 0) {
                if (previous == NULL) {
                    vac_list->head = current->next;
                } else {
                    previous->next = current->next;
                }
                if (vac_list->tail == current) {
                    vac_list->tail = previous;
                }
                free(current);
                vac_list->num_vaccines--;
            } else {
                current->vaccine.portions = 0;
            }

            free(batch);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf(flag ? "%s: lote inexistente\n" : "%s: no such batch\n", batch);
    free(batch);
}

/* Deletes user vaccine applications based on name, date or/and
 * batch.
 * @param *itr - pointer to input string
 * @param *vac_list - pointer to vaccine list
 * @param current_date - current date
 * @param flag - language flag, 0 english, 1 portuguese
 * @param user_table - pointer to user hash table
 */
void deleteApplications(char *itr, l_Vaccines *vac_list, date current_date,
                        int flag, user_hash_table *user_table) {
    char *user_name = NULL, *date_str = NULL, *batch = NULL;
    int deleted_count= 0;
    int found_user = 0;
    date target_date;
    if (!readWord(&itr, &user_name, flag)) return;
    user_name = trimString(user_name);
    readWord(&itr, &date_str, flag);
    readWord(&itr, &batch, flag);
    int date_given = 0;
    if (date_str != NULL) {
        if (!readDate(date_str, &target_date, flag)) {
            printMessage(flag, "invalid date", "data inválida");
            free(user_name);
            free(date_str);
            free(batch);
            return;
        }
        if (!isBeforeOrEqual(target_date, current_date)) {
            printMessage(flag, "invalid date", "data inválida");
            free(user_name);
            free(date_str);
            free(batch);
            return;
        }
        date_given = 1;
    }
    if (batch != NULL) {
        trimString(batch);
        node_vac vac = vac_list->head;
        int batch_exists = 0;
        while (vac) {
            if (strcmp(vac->vaccine.batch, batch) == 0) {
                batch_exists = 1;
                break;
            }
            vac = vac->next;
        }
        if (!batch_exists) {
            printf(flag ? "%s: lote inexistente\n" : "%s: no such batch\n", batch);
            free(user_name);
            free(date_str);
            free(batch);
            return;
        }
    }
    unsigned long index = hash(user_name);
    user_hash_entry *entry = user_table->table[index];
    while (entry != NULL) {
        if (strcmp(entry->user_name, user_name) == 0) {
            found_user = 1;
            user_application_node *current = entry->applications;
            user_application_node *prev = NULL;
            while (current != NULL) {
                Application *app = &current->application;
                int match_date = !date_given || 
                equalDate(app->applicationDate, target_date);
                int match_batch = (batch == NULL) ||
                 strcmp(app->batch_withdrawn, batch) == 0;
                if (match_date && match_batch) {
                    node_vac vac = vac_list->head;
                    while (vac) {
                        if (strcmp(vac->vaccine.batch, app->batch_withdrawn) == 0){
                            vac->vaccine.applications--; 
                            break;
                        }
                        vac = vac->next;
                    }
                    user_application_node *to_delete = current;
                    if (prev == NULL) {
                        entry->applications = current->next;
                    } else {
                        prev->next = current->next;
                    }
                    current = current->next;
                    free(to_delete);
                    deleted_count++;
                    continue;
                }
                prev = current;
                current = current->next;
            }
            break;
        }
        entry = entry->next;
    }
    if (!found_user) {
        printf(flag ? "%s: utente inexistente\n" : "%s: no such user\n",
                user_name);
    } else {
        printf("%d\n", deleted_count);
    }
    free(user_name);
    free(date_str);
    free(batch);
}

/* Advances system date, if the new date is valid.
 * @param *itr - pointer to input string
 * @param *real_date - pointer to current system date 
 * @param *date_list - pointer to date list
 * @param flag - language flag, 0 english, 1 portuguese
 */
void advanceTime (const char *itr, date *real_date,
                  list_date *date_list, int flag) {
    const char *str_date = itr + 2;
    date new_date = {0,0,0};
    if (!readDate(str_date, &new_date, flag)) {  
        return;  
    } 
    if (new_date.year < real_date->year ||
        (new_date.year == real_date->year && new_date.month < real_date->month) ||
        (new_date.year == real_date->year && new_date.month == real_date->month &&
            new_date.day < real_date->day)){
            printMessage (flag, "invalid date", "data inválida");
            return;
    }
    *real_date = new_date;
    addDateList(date_list, new_date, flag);
    printf ("%02d-%02d-%04d\n", real_date->day, real_date->month, real_date->year);
}