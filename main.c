
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"HashTableAPI.h"
#include"main.h"

int main()
{
  FILE *fp;
  char filename[100];
  char vaultPassword[100];
  char passwordAttempt[100];
  char readPassword[50];
  int key;
  char option;
  memset(&filename[0], 0, sizeof(filename));
  memset(&vaultPassword[0], 0, sizeof(vaultPassword));
  memset(&passwordAttempt[0], 0, sizeof(passwordAttempt));
  HTable *hashTable = createTable(17,hashFunction,destroyData,NULL);

  printf("Welcome to the password vault\n");
  printf("Please enter your personal password vault file name,\nor if you don't have one, please enter the name of the one you wish:");
  scanf("%s",filename);

  if((fp = fopen(filename, "r")) == NULL)
  {
    printf("filename doesn't exist, a new file will be created at the end of your session\n");
    printf("enter the personal password vault's password:");
    scanf("%s",vaultPassword);
  }
  else
  {
    printf("enter the personal password vault's password:");
    fscanf(fp,"%s",vaultPassword);
    scanf("%s",passwordAttempt);
    while(strcmp(passwordAttempt,vaultPassword)!=0 && strcmp(passwordAttempt,"quit")!=0)
    {
      printf("the password was incorrect, either try again or type quit to exit\n");
      scanf("%s",passwordAttempt);
    }
    if (strcmp(passwordAttempt,"quit")==0)
    {
      fclose(fp);
      return 0;
    }
    memset(readPassword, 0, sizeof(char)*50);
    while (fscanf(fp,"%d %s\n",&key,readPassword)!=EOF)
    {
      createReadPassword(hashTable,key,readPassword);
      memset(readPassword, 0, sizeof(char)*50);

    }
    fclose(fp);

  }
  while(option!='4')
  {
    printf("Enter the number corresponding to the option you wish to select\n");
    printf("1. change password vault's password\n");
    printf("2. change/add new password\n");
    printf("3. access password previously entered\n");
    printf("4. save all changes and quit\n");
    getchar();
    scanf("%c",&option);
    getchar();
    switch(option)
    {
      case '1':
        printf("enter a new password for the personal password vault's password\n");
        scanf("%s",vaultPassword);
        break;
      case '2':
        createPassword(hashTable);
        break;
      case '3':
        accessPassword(hashTable);
        break;
      case '4':
        saveFile(filename,hashTable,vaultPassword);
        break;
      default :
        printf("The input you entered wasn't valid, please try again\n");
    }



  }

  return 0;
}

void createPassword(HTable *hashTable)
{
  char* charKey = malloc(sizeof(char)*50);
  char* password = malloc(sizeof(char)*50);
  memset(charKey, 0, sizeof(char)*50);
  memset(password, 0, sizeof(char)*50);
  int key;
  printf("Enter the name for which the password is for\n");
  printf("Note: this will be what is required in order to retrieve the password later\n");
  scanf("%s",charKey);
  key = stringToInt(charKey);
  printf("Enter the password\n");
  scanf("%s",password);

  insertData(hashTable,key,(void*)password);
  printf("password added");
  return;
}
void createReadPassword(HTable *hashTable, int key, char* password)
{
  char* readPassword = malloc(sizeof(char)*50);
  strcpy(readPassword,password);
  insertData(hashTable,key,(void*)readPassword);
  return;
}

void accessPassword(HTable *hashTable)
{
  char charKey[50];
  memset(&charKey[0], 0, sizeof(charKey));
  int key;
  printf("Enter the name for which the password is for\n");
  scanf("%s",charKey);
  key = stringToInt(charKey);
  printf("The password for that name is %s\n",(char*)lookupData(hashTable,key));
  return;
}

void saveFile(char* filename, HTable *hashtable,char* password)
{
  FILE *fp;
  fp = fopen(filename, "w");
  fprintf(fp,"%s\n",password);
  for (int i=0; i<17; i++)
  {
    while(hashtable->table[i]!=NULL)
    {
      fprintf(fp,"%d %s\n",hashtable->table[i]->key,(char *)hashtable->table[i]->data);
      hashtable->table[i]=hashtable->table[i]->next;
    }
  }
  return;
}

int stringToInt(char* key)
{
  int counter = 0;
  for(int i = 0; key[i] != '\0'; ++i)
  {
    counter=counter +key[i];
  }
  return counter;
}

int hashFunction(size_t tableSize, int key)
{
  return key%tableSize;
}

void destroyData(void* data)
{
  free(data);
  return;
}

