#include <stdio.h>
#include <stdlib.h>
#include "HashTableAPI.h"
HTable *createTable(size_t size, int (*hashFunction)(size_t tableSize, int key),void (*destroyData)(void *data),void (*printData)(void *toBePrinted))
{

  HTable *hTable = malloc(sizeof(hTable)*1);  //allocate the memory for the table
  hTable->size=size;                          //set the hash table size to size

  hTable->table = malloc(sizeof(Node*)*size);  //allocate the memory for the array of node addresses

  //make sure the node addresses are initialized to NULL
  for (int i=0; i<size; i++)
  {
    hTable->table[i] = NULL;
  }

  hTable->destroyData=destroyData;
  hTable->hashFunction=hashFunction;
  hTable->printData=printData;

  return hTable;
}

Node *createNode(int key, void *data)
{
//creating new node and allocating space for node
  Node * newNode = malloc(sizeof(Node)*1);

  //initializing node elements
  newNode->key = key;
  newNode->data = data;
  newNode->next = NULL;

  //returning new node
  return newNode;
}

/** Deletes the entire hash table and frees memory of every element.
*@pre Hash Table must exist.
*@param hashTable pointer to hash table containing elements of data
**/
void destroyTable(HTable *hashTable)
{
  if (hashTable == NULL)
  {
    return;
  }
  Node *nodeHolder;
  for(size_t i = 0; i < hashTable->size; i++)
  {
    while(hashTable->table[i]!=NULL)
    {
      nodeHolder = hashTable->table[i];
      hashTable->destroyData(nodeHolder->data);
      hashTable->table[i] = hashTable->table[i]->next;
      free(nodeHolder);
    }
  }
  //free the hashtable itself
  free(hashTable->table);
  free(hashTable);
  return;

}

void insertData(HTable *hashTable, int key, void *data)
{

  //check if hashTable is NULL
  if (hashTable == NULL)
  {
    return;
  }

  int index;
  index = hashTable->hashFunction(hashTable->size,key);//calculate index

  if (hashTable->table[index] == NULL)
  {
    Node *newNode=createNode(key,data);
    hashTable->table[index]=newNode;
    return;
  }
  else
  {
    Node *nodeToSearch = hashTable->table[index]; //setting nodeToSearch to head node of table[index]
    while(nodeToSearch->next!=NULL &&nodeToSearch->key!= key)
    {
      nodeToSearch = nodeToSearch->next;
    }
    if (nodeToSearch->key == key)
    {
      nodeToSearch->data = data;
      return;
    }
    Node *newNode = createNode(key,data);
    nodeToSearch->next = newNode;
    return;
  }
}
/**Function to remove a node from the hash table
 *@pre Hash table must exist and have memory allocated to it
 *@post Node at key will be removed from the hash table if it exists.
 *@param hashTable pointer to the hash table struct
 *@param key integer that represents a piece of data in the table (eg 35->"hello")
 **/
void removeData(HTable *hashTable, int key)
{
  if (hashTable == NULL)
  {
    return;
  }

  int tableNumber = hashTable->hashFunction(hashTable->size,key);
  Node *nodeToSearch = hashTable->table[tableNumber];
  Node *nodeToDelete;
  if(nodeToSearch ==NULL)
  {
    return;
  }
  if (nodeToSearch->next==NULL || nodeToSearch->key==key)
  {
    if (nodeToSearch->key==key)
    {

      hashTable->destroyData(nodeToSearch->data);
      hashTable->table[tableNumber]=nodeToSearch->next;
      free(nodeToSearch);
      return;
    }
    else
    {
      return;
    }
  }
  while(nodeToSearch->next->next!=NULL &&nodeToSearch->next->key!= key)
  {
    nodeToSearch = nodeToSearch->next;
  }
  if (nodeToSearch->next->key == key)
  {
    hashTable->destroyData(nodeToSearch->next->data);
    nodeToDelete = nodeToSearch->next;
    nodeToSearch->next = nodeToSearch->next->next;
    free(nodeToDelete);
  }
  return;
}

void *lookupData(HTable *hashTable, int key)
{
  if (hashTable == NULL)
  {
    return NULL;
  }

  int tableNumber =  hashTable->hashFunction(hashTable->size,key);
  Node *nodeToSearch = hashTable->table[tableNumber];
  if(nodeToSearch==NULL)
  {
    return NULL;
  }
  while(nodeToSearch->next!=NULL &&nodeToSearch->key!= key)
  {
    nodeToSearch = nodeToSearch->next;
  }
  if(nodeToSearch->key == key)
  {
    return nodeToSearch->data;
  }
  else
  {
    return NULL;
  }
  return NULL;

}

