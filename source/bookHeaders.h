  /*This program is free software: you can redistribute it and/or modify
   *it under the terms of the GNU General Public License as published by
   *the Free Software Foundation, either version 3 of the License, or
   *(at your option) any later version.

   *This program is distributed in the hope that it will be useful,
   *but WITHOUT ANY WARRANTY; without even the implied warranty of
   *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   *GNU General Public License for more details.

   *You should have received a copy of the GNU General Public License
   *along with this program.  If not, see <https://www.gnu.org/licenses/>.
   */

#ifndef _BOOKHEADERS_H__
#define _BOOKHEADERS_H__
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define uSize 51
#define COMP 48
#define ENJIN 49
#define MATHS 50
#define PHYS 51
#define CHEM 52
#define BIO 53
#define ARTS 54
#define delim 21
#define recSize 1611
#define shinlf 22

/*Limitations:
 *fields: author should be less than 300 characters
 *fields: rack should be less than 99999
 *fields: title should be less than 300 characters
 *fields: description should be 1000 chars or less
 */

struct books *head;
struct books *tail;
int tailID;

typedef struct books b_struct;

//tail id gives the id of the last member in the database. Note: first member starts from 1.
extern int tailID;

//put this at the beginning of the code to initialize the api described here.
extern void initializeBooks(FILE *fptr);

//adds records to the given file pointer. Note: char *descript can be NULL
extern void addRecord(FILE *fl, char c, char *auth, char *rack, char *name, char *descript);

//edits category of a given record
extern void editCategory(int theID, FILE *fp, char c);

//as the name suggests
extern void editName(int theID, FILE *fp, char *arr);

//as the name suggests
extern void editRack(int theID, FILE *fp, char *arr);

//as the name suggests
extern void editTitle(int theID, FILE *fp, char *arr);

//edits the description. Note, cannot be NULL.
extern void editDes(int theID, FILE *fp, char *arr);

//This function returns the number of records matched according to the c. All the possible categories are described in the preprocessor #define. The int *arr will be filled with the record id. This id could be used to retrieve the record etc. The array should be large enough to contain all the id's
extern int list_id_category(char c, int *arr);

//similar to above, this function sorts by rack number.
extern int list_id_rack(int rck, int *arr);

//fills retArr with the record of id: theID. return array is delimited by shinlf. There will still exist a delimiter even if description is empty, which is soon followed by the null terminator. retArr should be the size of recSize. NOTE: I still haven't tested the max length per record scenario.
extern void returnRecord(FILE *fp, int theID, char *retArr);

//fills arrID with records that match with the string char *str. Uses strstr of the string.h header
extern int searchName(FILE *fp, int *arrID, char *str);

//similar to above, searches the title for matches
extern int searchTitle(FILE *fp, int *arrID, char *str);

//similar to above
extern int searchDes(FILE *fp, int *arrID, char *str);

//helper function that returns the bStruct pointer to the corresponding id
extern b_struct *retIDbook(int theID);

//freeing memory allocated by the structure. Use this at the end
extern void closeBooks();

//an ugly way to handle deletion of records. This function takes an array of integers with IDs tagged for deletion. NOTE: arrID should always have the size of arrID[loop] due to internal assignments. The return pointer is the new file pointer. ie: use fp = delRecords(fp, {1, 4, 2, 0}, 3, "data/index");
extern FILE *delRecords(FILE *fp, int *arrID, int loop, char *name);

//returns 0 if char *username is not found. Else, it returns the position of the the record.
extern int checkUsername(char *username, FILE *fp);

//similar to delRecords in usage, give the username and the authData address.
extern FILE *delUser(char *username, FILE *fp, char *dbName);

//this function returns the total number of users to create a unique database name
extern int userCount(FILE *fp);

//adds a user to the authData file. This also needs a unique database address, which in turn creates a new database.
extern void addusr(char *username, char *pass, FILE *fp, char *dbLocation);

//the most important function. This function can be used for authentication and retrieve the relevant database name in retArr. retArr should probably be a null terminated array with uSize size. returns 1 when authentication fails. returns 0 if authentication is successful. retArr will have a null terminator at the beginning in case of failure.
extern int begin_auth(char *username, char *pass, FILE *fp, char *retArr);
#endif
