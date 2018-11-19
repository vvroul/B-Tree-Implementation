#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesList.h"

//This file contains functions about the list that keeps track of the files that are created,the one that keeps track of the open files and the one that keeps track of the files that have an open scan



//*********************************************************************************************************************



//These functions are for the list that keeps track of the files we have created



int emptyList(fileNodePointer root)
//This function checks whether the list is empty or not
{
	if(root==NULL)
	{
		printf("It's empty\n");
		return 1;//Truth
	}
	else
	{
		printf("It's not empty\n");
		return 0;//False
	}
}



void createList(fileNodePointer* root)
//Creates an empty new list of files
{
	(*root)=NULL;
	printf("Returning from creation\n");
}



void addtoList(fileNodePointer* root,char* fileName,int keyLength,int dataLength,char keyType,char dataType,void* block)
//Adds a new node to the list
{
	printf("In addToList\n");
	printf("%x\n",block);
	fileNodePointer newnode=malloc(sizeof(fileNode));
	if(newnode==NULL)
	{
		printf("Cannot allocate memory for a new node\n");
		return;
	}
	//Created the new node
	strcpy(newnode->fileName,fileName);
	newnode->dataLength=dataLength;
	newnode->keyLength=keyLength;
	newnode->dataType=dataType;
	newnode->keyType=keyType;
	newnode->root=block;
	newnode->next=NULL;
	//Initialized the node
	if((*root)==NULL)
	{//This is the first node to be added
		printf("This is the first node to be added\n");
		(*root)=newnode;
	}
	else//This is not the first node to be added
	{
		fileNodePointer current=(*root);
		fileNodePointer previous;
		while(current!=NULL)
		{
			previous=current;
			current=current->next;
		}		
		previous->next=newnode;
	}//Added the new node at the end of the list
}



int searchforfileintheList(fileNodePointer currentnode,char* fileName)	
//Searches for a file named fileName in the list.
//If this file exists,it returns 1,otherwise it returns 0
{
	printf("This is the name I'm searching\n");
	while(currentnode!=NULL)
	{
		printf("Current file's name %s\n",currentnode->fileName);
		if(strcmp(currentnode->fileName,fileName)==0)
		{
			printf("It's already here\n");
			return 1;
		}
		currentnode=currentnode->next;
	}
	printf("It's not here\n");
	return 0;
}



void destroytheList(fileNodePointer* root)
//Destroys the list
{
	if((*root)==NULL)//It's empty
	{
		return;
	}
	else
	{
		fileNodePointer current,previous;
		current=(*root);
		while(current!=NULL)
		{
			previous=current;
			current=current->next;
			free(previous);
		}
		//free((*root));
		return;
	}
}



int deleteanodefromList(fileNodePointer *root,char* fileName)
//Deletes the node for the file named fileName from the list
{
	printf("Going to delete the file %s from the list\n",fileName);
	fileNodePointer temp,current,previous;
	current=(*root);
	previous=NULL;
	while(current!=NULL)
	{
		if(strcmp(current->fileName,fileName)==0)
		{
			break;
		}
		else
		{
			previous=current;
			current=current->next;
		}
	}
	if(current==NULL)
	{
	//There's no such file
		printf("There is no such file\n");
		return -1;//Nothing to delete
	}
	else		
	{
		if(current==(*root))
		{//We have to delete the first node
			(*root)=current->next;
			free(current);
		}
		else
		{
			previous->next=current->next;
			free(current);
		}
	}
	return 1;//AME_OK
}



int getkeyLength(fileNodePointer root,char* fileName)
//Returns the keyLegth of the file named fileName
{
	fileNodePointer current=root;
	while(current!=NULL)
	{
		if(strcmp(fileName,current->fileName)==0)
		{
			return current->keyLength;
		}
		current=current->next;
	}
	return 0;//Error
}



int getdataLength(fileNodePointer root,char* fileName)
//Returns the dataLength of the file named fileName
{
	fileNodePointer current=root;
	while(current!=NULL)
	{
		if(strcmp(fileName,current->fileName)==0)
		{
			return current->dataLength;
		}
		current=current->next;
	}
	return 0;//Error
}

char getkeyType(fileNodePointer root,char* fileName)
//Returns the type of variable that is the key
{
	fileNodePointer current=root;
	while(current!=NULL)
	{
		if(strcmp(fileName,current->fileName)==0)
		{
			return current->keyType;
		}
		current=current->next;
	}
	return 0;//Error
}

char getdataType(fileNodePointer root,char* fileName)
//Returns the type of variable that is the data
{
	fileNodePointer current=root;
	while(current!=NULL)
	{
		if(strcmp(fileName,current->fileName)==0)
		{
			return current->dataType;
		}
		current=current->next;
	}
	return 0;//Error
}



void* gettheRoot(fileNodePointer root,char* fileName)
//Returns the root of the B+ tree attached to the file named fileName
{
	fileNodePointer current=root;
	while(current!=NULL)
	{
		if(strcmp(fileName,current->fileName)==0)
		{
			printf("I'll return %x\n",current->root);
			return current->root;
		}
		current=current->next;
	}
	printf("I'll return NULL\n");
	return NULL;//Error
}



void changetheRoot(fileNodePointer root, char* fileName,void* newRoot)
//Changes the root of the B+ tree assosiated with the file named fileName
{
	printf("In second change the root\n");
	fileNodePointer current = root;
	while (current != NULL)
	{
		printf("Searching for the file\n");
		if (strcmp(fileName, current->fileName) == 0)
		{
			printf("Found it\n");
			current->root = newRoot;
			break;
		}
		current = current->next;
	}
	return;
}



//*********************************************************************************************************************



//These functions are for the list that keeps track of the open files



int emptyopens(openNodePointer root)
//This function checks whether the list is empty or not
//If it is empty,it returns 1;otherwise it returns 0
{
	if(root==NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



void createOpenList(openNodePointer* root)
//Creates an empty new list of files
{
	(*root)=NULL;
}



int addtoOpenList(openNodePointer* root,int fileDesc,char* fileName)
//Adds a new node to the list
{
	int numofelements=numOfOpenElements((*root));
	if(numofelements>=MAXOPENFILES)
	{
		return -1;//The list is full
	}
	openNodePointer newnode=malloc(sizeof(openNode));
	if(newnode==NULL)
	{
		printf("Cannot allocate memory for a new node\n");
		return -2;//No memory
	}
	//Created the new node
	strcpy(newnode->fileName,fileName);
	newnode->id=fileDesc;
	newnode->position=numofelements+1;
	newnode->next=NULL;
	//Initialized the node
	if((*root)==NULL)
	{//This is the first node to be added
		(*root)=newnode;
	}
	else//This is not the first node to be added
	{
		openNodePointer current=(*root);
		openNodePointer previous;
		while(current!=NULL)
		{
			previous=current;
			current=current->next;
		}		
		previous->next=newnode;
	}//Added the new node at the end of the list
	return 1;//Success story
}



int searchforfileintheOpenList(openNodePointer currentnode,char* fileName)
//Searches for a file named fileName in the list.
//If this file exists,it returns 1;otherwise it returns 0	
{
	while(currentnode!=NULL)
	{
		if(strcmp(currentnode->fileName,fileName)==0)
		{
			printf("It's already here\n");
			return 1;
		}
		currentnode=currentnode->next;
	}
	printf("It's not here\n");
	return 0;
}



void destroytheOpenList(openNodePointer* root)
//Destroys the list
{
	if((*root)==NULL)//It's empty
	{
		return;
	}
	else
	{
		openNodePointer current,previous;
		current=(*root);
		while(current!=NULL)
		{
			previous=current;
			current=current->next;
			free(previous);
		}
		//free((*root));
		return;
	}
}



int deleteanodefromOpenList(openNodePointer *root,int fileDesc)
//Deletes the node for the file named fileName from the list
{
	openNodePointer temp,current,previous;
	current=(*root);
	previous=NULL;
	while(current!=NULL)
	{
		if(current->id==fileDesc)
		{
			break;
		}
		else
		{
			previous=current;
			current=current->next;
		}
	}
	if(current==NULL)
	{
	//There's no such file
		printf("There is no such file\n");
		return -1;//Nothing to destroy
	}
	else		
	{
		if(current==(*root))
		{//We have to delete the first node
			(*root)=current->next;
			free(current);
		}
		else
		{
			previous->next=current->next;
			free(current);
		}
	}
	return 1;//FINE
}



int numOfOpenElements(openNodePointer root)
//This function counts how many elements the list contains
{
	int numofelements=0;
	openNodePointer current=root;
	while(current!=NULL)
	{
		++numofelements;
		current=current->next;
	}
	return numofelements;
}



char* returntheName(openNodePointer root,int fileDesc)
//Returns the name of the file with fileDesc,if it is in the list;otherwise NULL
{
	openNodePointer current=root;
	while(current!=NULL)
	{
		if(current->id==fileDesc)
		{
			return current->fileName;
		}
		current=current->next;
	}
	return NULL;
}



int returnthefileDesc(openNodePointer root,char* fileName)
//Returns the fileDesc of a file named fileName
{
	printf("Searching for the filename %s\n",fileName);
	openNodePointer current=root;
	while(current!=NULL)
	{
		if(strcmp(current->fileName,fileName)==0)
		{
			printf("Found it,id=%d\n",current->id);
			return current->id;
		}
		current=current->next;
	}
	return -1;
}



//*********************************************************************************************************************



//These functions are for the list that keeps track of the files under scan



int emptyscans(scanNodePointer root)
//This function checks whether the list is empty or not
//If it is empty,it returns 1;otherwise it returns 0
{
	if(root==NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



void createScanList(scanNodePointer* root)
//Creates an empty new list of files
{
	(*root)=NULL;
}



int addtoScanList(scanNodePointer* root,char* fileName)
//Adds a new node to the list
{
	int numofelements=numOfScanElements((*root));
	if(numofelements>=MAXSCANS)
	{
		return -1;//The list is full
	}
	scanNodePointer newnode=malloc(sizeof(scanNode));
	if(newnode==NULL)
	{
		printf("Cannot allocate memory for a new node\n");
		return -2;//No memory
	}
	//Created the new node
	strcpy(newnode->fileName,fileName);
	newnode->position=numofelements+1;
	newnode->next=NULL;
	//Initialized the node
	if((*root)==NULL)
	{//This is the first node to be added
		(*root)=newnode;
	}
	else//This is not the first node to be added
	{
		scanNodePointer current=(*root);
		scanNodePointer previous;
		while(current!=NULL)
		{
			previous=current;
			current=current->next;
		}		
		previous->next=newnode;
	}//Added the new node at the end of the lis
	return 1;//Success
}



int searchforfileintheScanList(scanNodePointer currentnode,char* fileName)
//Searches for a file named fileName in the list.
//If this file exists,it returns 1;otherwise it returns 0	
{
	while(currentnode!=NULL)
	{
		if(strcmp(currentnode->fileName,fileName)==0)
		{
			printf("It's already here\n");
			return 1;
		}
		currentnode=currentnode->next;
	}
	printf("It's not here\n");
	return 0;
}



void destroytheScanList(scanNodePointer* root)
//Destroys the list
{
	if((*root)==NULL)//It's empty
	{
		return;
	}
	else
	{
		scanNodePointer current,previous;
		current=(*root);
		while(current!=NULL)
		{
			previous=current;
			current=current->next;
			free(previous);
		}
		//free((*root));
		return;
	}
}



int deleteanodefromScanList(scanNodePointer *root,int scanDesc)
//Deletes the node for the file named fileName from the list
{
	scanNodePointer temp,current,previous;
	current=(*root);
	while(current!=NULL)
	{
		if(current->position==scanDesc)
		{
			break;
		}
		else
		{
			previous=current;
			current=current->next;
		}
	}
	if(current==NULL)
	{
	//There's no such file
		printf("There is no such file\n");
		return -1;//Nothing to destroy
	}
	else		
	{
		if(current==(*root))
		{//We have to delete the first node
			(*root)=current->next;
			free(current);
		}
		else
		{
			previous->next=current->next;
			free(current);
		}
	}
	return 1;//OK
}



int numOfScanElements(scanNodePointer root)
//This function counts how many elements the list contains
{
	int numofelements=0;
	scanNodePointer current=root;
	while(current!=NULL)
	{
		++numofelements;
		current=current->next;
	}
	return numofelements;
}



int returnthePosition(scanNodePointer root,char* fileName)
//Returns the fileDesc of a file named fileName
{
	scanNodePointer current = root;
	while(current!=NULL)
	{
		if(strcmp(current->fileName,fileName)==0)
		{
			printf("Found the position\n");
			return current->position;
		}
		current=current->next;
	}
	return -1;
}



char* returnthescanName(scanNodePointer root,int scanDesc)
//Returns the name of the file with fileDesc,if it is in the list;otherwise NULL
{
	scanNodePointer current=root;
	while(current!=NULL)
	{
		if(current->position==scanDesc)
		{
			return current->fileName;
		}
		current=current->next;
	}
	return NULL;
}