/* File: vaccines.h
*  Student: Natacha Sousa, 107413
*  Description: This file has all the headers and declarations of commands functions.
*/

#ifndef VACCINE_H
#define VACCINE_H

#include "vaccines.h"
#include "aux.h"
#include "structures.h"

void addBatch (char *itr, l_Vaccines *vac_list, list_date *date_list,
                 int flag, int check_date);

void listVaccines (char *itr, l_Vaccines *vaccine_list, int flag);

void vaccinationApplication(char *itr, l_Vaccines *vaccine_list, date actual_date, 
                            user_hash_table *user_hash, int flag);

void listUserApplications(char *itr, user_hash_table *hash_table, int flag);

void removeBatch(char *itr, l_Vaccines *vac_list, int flag);

void deleteApplications(char *itr, l_Vaccines *vac_list, date current_date,
                         int flag, user_hash_table *user_table);

void advanceTime (const char *itr, date *real_date, list_date *date_list, int flag);

#endif /*VACCINE_H*/