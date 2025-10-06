/* iaed - ist1107413 - project
*  File: main.c
*  Student: Natacha Sousa, 107413
*  Description: This file contains the function that treats all the commands.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "aux.h"
#include "structures.h"
#include "vaccines.h"

#define DIM_INSTRUCTION 65535 /*16 bits*/

/* Main function for vaccination management system.
 * 
 * This function inicializes the system, processes command-line has
 * arguments to determine the language for messages (english or potuguese),
 * and enters a loop to handle various user instructions.
 * 
 * Supported commands:
 * - 'q': Quit program.
 * - 'c': Add a new vaccine batch.
 * - 'l': List all available vaccines (or by name).
 * - 'a': Register a vaccine application to a user.
 * - 'r': Remove a vaccine batch availability.
 * - 'd': Delete user applications of a vaccine.
 * - 'u': List all applications by user.
 * - 't': Advance the current date.
 * 
 * @param argc - argument count
 * @param argv - argument vector, if the first argument is "pt",
 * portuguese messages are enable
 */
int main (int argc, char *argv[]) {

    int flag = 0;

    if(argc > 1) {
        if (strcmp(argv[1], "pt") == 0){
            flag = 1;
        }
    }

    char *itr = createVectorItr(DIM_INSTRUCTION, flag);
    date actual_date = {1,1,2025};
    l_Vaccines vac_list = {0, {"", "", {0, 0, 0}, 0, 0}, NULL, NULL, NULL};
    list_date* date_list = (list_date*) malloc(sizeof(list_date));
    int check_date = 0;

    user_hash_table *user_hash = calloc(1, sizeof(user_hash_table));
    inicializeHashTable(user_hash);


    while (1){
        readInstruction(itr);
        switch (itr[0]) {
        case 'q':
            exit(EXIT_SUCCESS);
            break;
        case 'c':
            addBatch(itr + 1, &vac_list, date_list, flag, check_date);
            break;
        
        case 'l':
            listVaccines(itr + 1, &vac_list, flag);
            break;
        case 'a':
            vaccinationApplication(itr + 1, &vac_list, actual_date, user_hash, flag);
            break;
        case 'r':
            removeBatch(itr + 1, &vac_list, flag);
            break;
        case 'd':
            deleteApplications(itr + 1, &vac_list, actual_date, flag, user_hash);
            break;
        case 'u':
            listUserApplications(itr + 1, user_hash, flag);
            break;
        case 't':
            advanceTime (itr, &actual_date, date_list, flag);
            check_date = 1;
            break;
        }
    }
}

