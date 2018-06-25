#ifndef _MAIN_
#define _MAIN_

#include <stdio.h>
#include <stdlib.h>

void createPassword(HTable *hashTable);

void accessPassword(HTable *hashtable);

void savefile();

int stringToInt(char* key);

int hashFunction(size_t tableSize, int key);

void destroyData(void* data);

void saveFile(char* filename, HTable *hashtable,char* password);

void createReadPassword(HTable *hashTable, int key, char* password);





#endif
