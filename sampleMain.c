#include <stdio.h>
#include "AM.h"

#define FILENAME1 "COMMANDER_LEXA"
#define FILENAME2 "CLARKE_GRIFFIN"
#define FILENAME3 "ASAMI_SATO"
#define FILENAME4 "HOLLSTEIN"
#define FILENAME5 "PVRIS"

fileNodePointer filesList;
openNodePointer openList;
scanNodePointer scanList;


int main(void)
{	
	AM_Init();
	//Initializing the three lists
	createList(&filesList);
	createOpenList(&openList);
	createScanList(&scanList);
	printf("Created the 3 lists.\n");


	char fileName[30];
	strcpy(fileName,FILENAME1);
	printf("First we create a file with filename %s\n",FILENAME1);

	//Creating the file
	if(AM_CreateIndex(fileName,'i',4,'i',4)<0)
	{
		AM_PrintError("Error;cannot create the Ιndex\n");
		return -1;
	}

	//Opening the file
	if (AM_OpenIndex(fileName) < 0)
	{
		AM_PrintError("Error while opening file!\n");
		return -1;
	}

	
	int fileDesc, scanDesc;
	char* recsFile = "mixedNumbers.txt";
	fileDesc =returnthefileDesc(openList,fileName);
	if(fileDesc<0)
	{
		printf("No such open file\n");
		return -1;
	}
	FILE* myFile = fopen(recsFile, "r");
	if (myFile == NULL)
	{
		printf("Error opening records file!\n");
		exit(1);
	}
	int value1, value2;
	char v1[30], v2[30];
	int testVal = 4;


	//Inserting values, building the tree
	while (!feof(myFile))
	{
		fscanf(myFile, "%d", &value1);
		fscanf(myFile, "%d", &value2);
		printf("%d,%d\n",value1,value2);

		if(AM_InsertEntry(fileDesc, &value1, &value2) != AME_OK)
		{
			AM_PrintError("Error while inserting values!\n");
		}
		printf("Finished with inserting that record\n");
	}


	//Printing the data 
	printf("Done with the insertion,going to print\n");
	searchANDprint(fileDesc);


	//Opening new scan to the file
	if ((scanDesc = AM_OpenIndexScan(fileDesc, EQUAL, &testVal)) < 0)
	{
		AM_PrintError("Error while opening scan!\n");
		exit(1);
	}
	else
	{
		printf("We found all the information needed and scanDesc=%d\n",scanDesc);
	}

	//Closing the scan
	if (AM_CloseIndexScan(scanDesc) < 0)
	{
		AM_PrintError("Error while closing scan index!\n");
	}

	//Closing the file index
	if (AM_CloseIndex(fileDesc) < 0)
	{
		AM_PrintError("Error while closing index!\n");
		return -1;
	}

	//Destroying the file
	if (AM_DestroyIndex(fileName) < 0)
	{
		AM_PrintError("Error while deleting file with filename!\n");
		return -1;
	}


	//Destroying the 3 lists
	destroytheList(&filesList);
	destroytheOpenList(&openList);
	destroytheScanList(&scanList);
}