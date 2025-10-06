/* File: aux.h
*  Student: Natacha Sousa, 107413
*  Description: This file contains all the headers and declarations of auxiliary functions.
*/

#ifndef AUX_H
#define AUX_H

#include "structures.h"

void printMessage(int flag, const char *en_message, const char *pt_message);

void noMemory();

char *createVectorItr (int size, int flag);

void readInstruction (char *itr_empty);

int readWord (char **itr, char **word, int flag);

char* trimString(char *str);

int readDate (const char *entr, date *dt, int flag);

int isAfter (date newDate, date lastDate);

int validName (const char *name, int flag);

int validBatch (char *batch, int flag);

void addDateList (list_date *date_list, date new_date, int flag);

date getLastDate(list_date *date_list);

int compareBatches (vaccineBatch *a, vaccineBatch *b);

int isBeforeOrEqual(date d1, date d2);

int equalDate(date d1, date d2);

void insertVaccineSorted (l_Vaccines *list, vaccineBatch new_batch, int flag);

void inicializeHashTable (user_hash_table *hash_table);

unsigned long hash (const char *str);

void addUserApplication (user_hash_table *hash_table, Application application);

int alreadyVaccinatedHash (user_hash_table *hash_table, const char *user_name,
                            const char *batch, date actual_date);

void toLowerCase(char *str);

#endif /*AUX_H*/