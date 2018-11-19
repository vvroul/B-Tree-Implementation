#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "assistingFunctions.h"
#include "filesList.h"

#define VOIDPOINTERSIZE 8


//Αυτές οι συναρτήσεις προυποθετουν ότι έχουμε πρόσβαση στον αναγνωριστικό αριθμό ανοιχτού αρχείου,εν ολίγοις στη λίστα που κρατά τ'ανοιχτά αρχεία.Αυτό γίνεται μέσω ενός δείκτη στην αρχή της λίστας 
//ο οποίος είναι global.



//*********************************************************************************************************************



int getThePl(int fileDesc)
//This function returns the number of pointers that can be stored within a leaf block
{
	//printf("Getting the pl\n");
	int keylength;
	int blocksize=1024;
	int recordpointer=8;//Sizeo of a void pointer to the address of the data block where the particular record is stored
	char fileName[30];
	strcpy(fileName,returntheName(openList,fileDesc));
	keylength=getkeyLength(filesList,fileName);
	int pl;
	pl=((blocksize-VOIDPOINTERSIZE-8-4-4)/(recordpointer+keylength));
	return (pl*0.7);
}



//*********************************************************************************************************************



int getThePi(int fileDesc)
//This function returns the number of pointers that can be stored within an internal block
{
	int keylength;
	int blocksize=1024;
	char fileName[30];
	strcpy(fileName,returntheName(openList,fileDesc));
	keylength=getkeyLength(filesList,fileName);
	int pi;
	pi=((blocksize-8-4-4+keylength)/(VOIDPOINTERSIZE+keylength));
	return (pi*0.7);
}



//*********************************************************************************************************************



int getThenumberOfRecords(int fileDesc)
//This function returns the number of records that can be stored in a data block 
{
	int maxnumofrecs;
	int blocksize=1024;
	int recordlength;
	char fileName[30];
	strcpy(fileName,returntheName(openList,fileDesc));
	recordlength=getdataLength(filesList,fileName);
	recordlength=recordlength+getkeyLength(filesList,fileName);
	maxnumofrecs=((blocksize-4-8-4)/recordlength);
	return maxnumofrecs;
}



//*********************************************************************************************************************



void* getTheRoot(int fileDesc)
//This functions returns the root of a B+ tree attached to the fileDesc file"
{	
	char fileName[30];
	strcpy(fileName,returntheName(openList,fileDesc));
	return gettheRoot(filesList,fileName);
}



//*********************************************************************************************************************



int getTheKeyLength(int fileDesc)
//Returns the key length
{
	char fileName[30];
	strcpy(fileName,returntheName(openList,fileDesc));
	return getkeyLength(filesList,fileName);
}



//*********************************************************************************************************************



int getTheDataLength(int fileDesc)
//Returns the key length
{
	char fileName[30];
	strcpy(fileName,returntheName(openList,fileDesc));
	return getdataLength(filesList,fileName);
}



//*********************************************************************************************************************

char getTheDataType(int fileDesc)//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//Returns the type of variable that is the data
{
	char fileName[30];
	strcpy(fileName,returntheName(openList,fileDesc));
	return getdataType(filesList,fileName);
}

//***********************************************************************************************************************

char getTheKeyType(int fileDesc)//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//Returns the type of variable that is the key
{
	char fileName[30];
	strcpy(fileName,returntheName(openList,fileDesc));
	return getkeyType(filesList,fileName);
}

//************************************************************************************************************************

int isItALeaf(void* start)
//Παίρνει ως είσσδο έναν δείκτη στην αρχή ενός μπλοκ.Επιστρέφει 1 αν είναι φύλλο,0 αν είναι εωτερικό
{
	//printf("The address I got is %x\n",start);
	if(memcmp(start,"L",sizeof(char))== 0)
	{
		//printf("Leaf\n");
		return 1;
	}
	else
	{
		//printf("Not a leaf\n");
		return 0;
	}
}



//*********************************************************************************************************************



void findTheNextNode(void* theblock,void* value1,int keyLength,void** nextnode)
//findTheNextNode():παίρνει ως είσοδο ένα εσωτερικό μπλοκ,ένα κλειδί και το μήκος του,κι επιστρέφει μια διέυθυνση στην αρχή του επόμενου,με βάση το κλειδί
{
	//printf("Im here to find the next node searching the node %x\n",theblock);
 	int numofpointers=0,i;
 	//Διασχίζουμε το μπλοκ από την αρχή,και συγκρίνουμε το ληφθέν κλειδί με τα ήδη αποθηκευμένα
 	void* current_key;//Ο δείκτης σε κλεισί που αθ χρησιμοποιήσουμε για τις συγκρίσεις
 	void* addresstoreturn=(char*)theblock+sizeof(char)+2*sizeof(int);//Τώρσ δείχνει στον πρώτο πρώτο δέικτη
	int* c_key;//DEBUGGING
	int* key;//DEBUGGING
	key=(int*)value1;

 	void* startblock=theblock;//Ίσως χρειαστεί αν θυμόμαστε την αρχή του μπλοκ
 	theblock=(char*)theblock+sizeof(char);//Προσπερνάμε τον χαρακτήρα που δείχνει τον τύπο του μπλοκ
 	memmove(&numofpointers,theblock,sizeof(int));
 	current_key=(char*)theblock+2*sizeof(int)+VOIDPOINTERSIZE;//Τώρα δείχνει στο πρώτο αποθηκευμένο κλειδί
 	for(i=0;i<numofpointers-1;++i)//numofpointers-1 γτ τρέχουμε για το πλήθος των κλειδιών
 	{
 		//printf("Searching\n");
 		c_key=(int*)current_key;
		printf("current_key=%d\n",(*c_key));
 		if(memcmp(value1,current_key,keyLength)<0)//Το κλειδί που αναζητάμε είναι μικρότερο από αυτό που βρισκόμαστε
 		{
 			printf("We seek for a smaller key\n");
 			//printf("Ill return %x\n",addresstoreturn );
 			//return addresstoreturn;
 			break;
 		}
 		else if(memcmp(value1,current_key,keyLength)>0)//Το κλεισί που αναζητάμε είναι μεγαλύτερο από το τρέχον
		{
			printf("We seek for a bigger key\n");//Στην ακριβώς από κάτω γραμμή προστέθηκε το +voidpointersize
			addresstoreturn=(char*)addresstoreturn+keyLength+VOIDPOINTERSIZE;//Τώρα δείχνει στον επόμενο δείκτη που απέχει απόσταση ίση με το μήκος ενός κλειδιού
			current_key=(char*)current_key+VOIDPOINTERSIZE;//Τώρα δειχνει στο επόμενο κλειδί που απέχει όσο ένας δείκτης
		}
		else//Ισότητα
		{
			//printf("We are on an equal key\n");
			//printf("Ill return %x\n",addresstoreturn );
			
			//return addresstoreturn;
			break;

		}
 	}
 	//printf("If you're reading this I return the last of the mohicans\n");
 	memmove(nextnode,addresstoreturn,VOIDPOINTERSIZE);
 	//printf("Ill return \n");
 	return ;
}



//*********************************************************************************************************************



void* UpdateRecords(void* root)
{
	//printf("in UPDATE!!\n");

	int* temp;
	void* numOfRecs; 
	root = (char*)root + sizeof(char);//Προσπερνάμε το χαρακτήρα
	//printf("Ater moving root\n");
	//memcpy(numOfRecs, root, sizeof(root));//Άρα εδώ έχουμε τον αριθμό των εγγραφών μας
	numOfRecs = root;	
	//printf("num of recs!\n");	
	temp = (int*)numOfRecs;
	*temp = *temp +1;//Τον αυξάνουμε κατά 1
	numOfRecs = (void*)temp;
	//printf("we have the number of records\n");
	memcpy(root, numOfRecs, sizeof(numOfRecs));//Ανανεώνουμε το πεδίο στο μπλοκ 
	//printf("doneondoe\n");
	root = (char*)root + (2* sizeof(int));
	return root;
}
	


//*********************************************************************************************************************



void* split(void* theBlocktoSplit,int fileDesc,void** newkey)
{
	printf("In split\n");
	//This function splits the argument node in half.It returns a pointer to the extra node
	void* newblock;//REturn a pointer to the new block
	void* startnewblock;//Remember the address of the new block
	void* startold=theBlocktoSplit;//Remember the address of the old block 
	//printf("Im in split block and Ill split the block %x\n",theBlocktoSplit );
	//printf("Going to allocate a new block\n");
	if(BF_AllocateBlock(fileDesc)<0)
	{
		BF_PrintError("Error allocating block!");
		return NULL;
	}
	int totalnumberofBlocks=BF_GetBlockCounter(fileDesc);
	//The old block will keep half its data,and the rest half goes to the new one
	if (BF_ReadBlock(fileDesc, totalnumberofBlocks -1 , &newblock) < 0)
	{
		BF_PrintError("Error while reading heap file!");
		return NULL;
	}
	startnewblock=newblock;
	//printf("The old block will keep half its data,and the rest half goes to the new one\n");
	memcpy(newblock, theBlocktoSplit, sizeof(char));//Μεταφέρουμε στο καινουργιο μπλοκ τον τύπο του παλιού
	newblock = (char*)newblock + sizeof(char)+2*sizeof(int);
	--totalnumberofBlocks;
	memmove(newblock,&totalnumberofBlocks,sizeof(int));
	newblock=(char*)newblock+sizeof(int);//Αντιγραφή της τάξης του μπλοκ
	moveRecords(fileDesc,startold,startnewblock,newkey);
	printf("The address of the newblock is:%x and of the old:%x\n",startnewblock,startold);
	return startnewblock;
}



//*********************************************************************************************************************



//Τα ίδια,καθώς και τον επιτρεπτό αιθμό εγγραφών στα μπλοκ δεδομένων αυτού του αρχείου

void* WriteRecord(void* value1,void* value2,int fileDesc)
{	
	//printf("In the WriteRecord\n");
	char dataInfo[] = "D";
	int totalnumberofblocks=BF_GetBlockCounter(fileDesc);
	if(totalnumberofblocks < 0)
	{
		BF_PrintError("Error getting block counter!");
		return NULL;
	}
	else
	{
		//printf("Got block counter,going to search for the block\n");
		void* block;
		int i;
		int found=0;
		int gotanew=0;//Boolean
		for(i=0;i<totalnumberofblocks;++i)
		{
			//printf("Searchinf all teh blocks\n");
			if (BF_ReadBlock(fileDesc, i , &block) < 0)
			{
				BF_PrintError("Error while reading heap file!");
				return NULL;
			}

			if(memcmp(block, dataInfo, sizeof(char))==0)
			{
				//void* rememeberme=block;
				//printf("Found a data block\n");
				int keyLength,dataLength;
				keyLength=getTheKeyLength(fileDesc);
				dataLength=getTheDataLength(fileDesc);
				int numofrecs;
				//int* temp=&numofrecs;
				//memcpy(numofrecs, block , sizeof(block));
				block=(char*)block+sizeof(char);
				//memmove(temp,block,sizeof(int));
				memmove(&numofrecs,block,sizeof(int));
				//numofrecs = (char*)numofrecs + sizeof(char);
				//temp  =(int*)numofrecs;
				int maxrecs = getThenumberOfRecords(fileDesc);	
				//printf("maxrecs=%d\n",maxrecs);
				//printf("numofrecs=%d\n",numofrecs);
				//printf("CHANGE\n");
				if (numofrecs< maxrecs)
				{
					
					found=1;
					//printf("There's enough space here\n");
					void* returnad;

					++numofrecs;
					memmove(block,&numofrecs,sizeof(int));
					--numofrecs;
					block = (char*)block+(2*sizeof(int))+(numofrecs * (keyLength + dataLength));
					returnad=block; //memcpy(returnad, block, sizeof(block));
					memmove(block, value1, keyLength);
					block = (char*)block + keyLength;
					memmove(block, value2, dataLength);
					if (BF_WriteBlock(fileDesc, i ) < 0)
					{
						BF_PrintError("Error while writing to block!");
						return NULL;
					}
					//printf("Returning\n");
					return returnad;
				}
				else
				{
					gotanew=1;
				}
			}
		}
		if((found==0)||(gotanew==1))
		{//We couldn't find a block with enough space to write

			//printf("We couldn't find a block to write,so we'll allocate a new one\n");
			void* returnad = (void*)malloc(VOIDPOINTERSIZE);
			int totalnumberofBlocks;
			if(BF_AllocateBlock(fileDesc)<0)
			{
				BF_PrintError("Error allocating block!");
				return NULL;
			}
			int numofrecords=1;
			//void* num = &recs;
			totalnumberofblocks=BF_GetBlockCounter(fileDesc);
			if (BF_ReadBlock(fileDesc, totalnumberofblocks - 1 , &block) < 0)
			{
				BF_PrintError("Error while reading block!");
				return NULL;
			}
			int* id=&totalnumberofBlocks;
			(*id) = totalnumberofBlocks - 1;
			//printf("Okay,now going to write\n");
			//Write the info
			memmove(block, dataInfo, sizeof(char));
			block = (char*)block + sizeof(char);
			memmove(block, &numofrecords, sizeof(int));
			block = (char*)block + sizeof(int);
			memmove(block, id, sizeof(int));
			block = (char*)block + sizeof(int);
			memmove(returnad, block, VOIDPOINTERSIZE); //Before passing the char, maybe??? (same results)
			//printf("Return address copied!\n");
			//returnad=block;
			//Write the data
			int keyLength=getTheKeyLength(fileDesc);
			int dataLength=getTheDataLength(fileDesc);
			memmove(block, value1, keyLength);
			block = (char*)block + keyLength;
			memmove(block, value2, dataLength);
			if (BF_WriteBlock(fileDesc, totalnumberofblocks -1 ) < 0)
			{
				BF_PrintError("Error while writing to block!");
				return NULL;
			}
			//printf("Wrote successfully in the data block;going back\n");
			return returnad;
		}
	}
}	



//*********************************************************************************************************************



void searchANDprint(int fileDesc)
 //Finds a data block and prints the data
{
	int numofblocks=BF_GetBlockCounter(fileDesc);
	if(numofblocks<=0)
	{
		printf("Error with blocks\n");
		return;
	}
	int value1,value2,i;
	int keyLength=getTheKeyLength(fileDesc);
	int dataLength=getTheDataLength(fileDesc);
	void* block;		
	for(i=0;i<numofblocks;++i)
	{
		//printf("Searchinf all teh blocks\n");
		if (BF_ReadBlock(fileDesc, i , &block) < 0)
		{
			BF_PrintError("Error while reading heap file!");
			return ;
		}
		if(memcmp(block,"D",sizeof(char))==0)
		{
			//printf("We found a data block\n");
		
			int numofrecords=0;
			block=(char*)block+sizeof(char);
			memmove(&numofrecords,block,sizeof(int));
			printf("There are %d records in this block\n",numofrecords);
			block=(char*)block+2*sizeof(int);
			for(i=0;i<numofrecords;++i)
			{
				memmove(&value1,block,sizeof(int));
				block=(char*)block+keyLength;
				memmove(&value2,block,sizeof(int));
				block=(char*)block+dataLength;
				printf("%d\t%d\n",value1,value2);
			}
			//printf("Done with one data block,let's go to the next\n");
		}
	}
}



//*********************************************************************************************************************



void* getTheNewKey(void* newnode)
//Returns a pointer to the first key of the newnode
{
	void* thekey = newnode;
	if (memcmp(newnode, "L", sizeof(char)) == 0)
	{//Ο ληφθείς κόμβος είναι φύλλο
		thekey = (char*)thekey + sizeof(char)+2 * sizeof(int);
	}
	/*else if (memcmp(newnode, "I", sizeof(char)) == 0)
	{//Ο ληφθείς κόμβος είναι εσωτερικός
		thekey = (char*)thekey + sizeof(char)+2 * sizeof(int)+voidpointersize;
	}*/
	return thekey;
}



//*********************************************************************************************************************



void* makeNewRoot(void* splitBlock, void* newnode, int fileDesc,void*newkey)
//Creates a new root for the tree,and returns a pointer to it
{
	printf("I in makeNewRoot and the fileDesc is %d\n",fileDesc);
	//First we need to allocate a new block for our file
	int error=BF_AllocateBlock(fileDesc) ;
	if (error< 0)
	{
		printf("Error;cannot allocate a block\n");
		printf("This is the error code %d\n",error);
		return NULL;
	}
	//The new root is an internal node by default
	void* root;
	void* roottoreturn;
	int numofblocks = BF_GetBlockCounter(fileDesc);
	printf("The number of blocks in the file is %d\n",numofblocks );
	if (numofblocks <= 0)
	{
		printf("Error with number of blocks\n");
		return NULL;
	}
	--numofblocks;
	if (BF_ReadBlock(fileDesc, numofblocks, &root) < 0)
	{
		printf("Error reading block\n");
		return NULL;
	}
	printf("Read the new block;the address to the root is:%x\n",root);
	roottoreturn = root;
	memmove(root, "I", sizeof(char));
	printf("Moved the identifier char\n");
	//Η ρίζα είναι εσωτερικός κόμβος^^
	root = (char*)root + sizeof(char);
	int numofpointers = 2;
	memmove(root, &numofpointers, sizeof(int));
	printf("Wrote the number of pointers\n");
	root = (char*)root + sizeof(int);
	memmove(root, &numofblocks, sizeof(int));
	root = (char*)root + sizeof(int);
	printf("Wrote block id\n");
	//Τώρα που γράψαμε τ'απαραίτητα αναγνωριστικά,πρέπει να εγγράψουμε στη ρίζα τους πρώτους 2 δείκτες σε μπλοκ,και το πρώτο κλειδί
	void* startold = splitBlock;
	void* startnew = newnode;
	printf("The address of the newblock is:%x and of the old:%x\n",startnew,startold);
	/*if (memcmp(splitBlock, "L", sizeof(char)) == 0)
	{
		startnew = (char*)startnew + sizeof(char)+2 * sizeof(int);
	}
	else if (memcmp(splitBlock, "I", sizeof(char)) == 0)
	{
		startnew = (char*)startnew + sizeof(char)+2 * sizeof(int)+voidpointersize;
	}*/
	printf("Pointer points to the first key now\n");
	//Τώρα και ο δείκτης δείχνει στο πρώτο κλειδί του μπλοκ τους.Το κλειδί αυτό θα είναι το πρώτο κλειδί της καινούργιας ρίζας
	int keyLength = getTheKeyLength(fileDesc);
	memmove(root, &splitBlock, VOIDPOINTERSIZE);
	printf("Send the address of the old block\n");
	root = (char*)root + VOIDPOINTERSIZE;
	memmove(root, newkey, keyLength);
	printf("Send the new key\n");
	root = (char*)root + keyLength;
	memmove(root, &newnode, VOIDPOINTERSIZE);
	printf("Send the address of the new block\n");
	//^^Αντίγραψε τα κλειδιά και τους δείκτες στην καινούργια ρίζα
	error=BF_WriteBlock(fileDesc,numofblocks);
	if (error< 0)
	{
		printf("Error writing\n");
		printf("This is my error %d\n",error);
		return NULL;
	}
	//^^Γράψε τα δεδομένα στο μπλοκ
	printf("Returning the address to the root:%x\n",roottoreturn);
	return roottoreturn;//Επίστρεψε τον δείκτη στην αρχή του
}



//*********************************************************************************************************************



void changeTheRoot(int fileDesc, void* newroot)
//Changes the root of the B+ tree assosiated with the file fileDesc
{
	char filename[30];
	strcpy(filename,returntheName(openList, fileDesc));
	//printf("Calling the second,for file %s\n",filename);
	changetheRoot(filesList, filename, newroot);
	return;
}



//*********************************************************************************************************************



int isItAData(void* start)
//Ξ Ξ±Ξ―ΟΞ½ΞµΞΉ Ο‰Ο‚ ΞµΞ―ΟƒΟƒΞ΄ΞΏ Ξ­Ξ½Ξ±Ξ½ Ξ΄ΞµΞ―ΞΊΟ„Ξ· ΟƒΟ„Ξ·Ξ½ Ξ±ΟΟ‡Ξ® ΞµΞ½ΟΟ‚ ΞΌΟ€Ξ»ΞΏΞΊ.Ξ•Ο€ΞΉΟƒΟ„ΟΞ­Ο†ΞµΞΉ 1 Ξ±Ξ½ ΞµΞ―Ξ½Ξ±ΞΉ Ο†ΟΞ»Ξ»ΞΏ,0 Ξ±Ξ½ ΞµΞ―Ξ½Ξ±ΞΉ ΞµΟ‰Ο„ΞµΟΞΉΞΊΟ
{
	if (memcmp(start, "D", sizeof(char)) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



//*********************************************************************************************************************



int BPTSearch(int fileDesc, void* keyValue, int op)
{
	printf("In Search!\n");
	int keyLength = getTheKeyLength(fileDesc);
	int dataLength = getTheDataLength(fileDesc);
	int succeeded = 0;
	//block points to the root
	void* block = getTheRoot(fileDesc);//Now block points to the start of the root-block
	int isData = isItAData(block);
	void* previousblock = block;//ΞΟ„Ξ±Ξ½ ΞΈΞ± ΞΌΟ€ΞΏΟΞΌΞµ ΟƒΟ„Ξ·Ξ½ ΞµΟ€Ξ±Ξ½Ξ¬Ξ»Ξ·ΟΞ·,Ο‡ΟΞµΞΉΞ±Ξ¶ΟΞΌΞ±ΟƒΟ„Ξµ ΞΊΞ±ΞΉ Ο„Ξ·Ξ½ Ο„ΞΉΞΌΞ® Ο„ΞΏΟ… Ο€ΟΞΏΞ·Ξ³ΞΏΟΞΌΞµΞ½ΞΏΟ… ΞΌΟ€Ξ»ΞΏΞΊ
	while (!isItAData)
	{
		printf("In while searching for data block\n");
		void* nextblock;
		findTheNextNode(previousblock, keyValue, keyLength,&nextblock);
		isData = isItAData(nextblock);
		if (isData)
		{
			block = nextblock;
			break;
		}
		else
		{
			previousblock = nextblock;
		}
	}
	printf("We're gonna search in the data block\n");
	int value1, i;
	int count = 0;
	int numofrecords = 0, dataOrder;
	block = (char*)block + sizeof(char);//Ξ ΟΞΏΟƒΟ€ΞµΟΞ½Ξ¬ΞΌΞµ Ο„ΞΏΞ½ Ο€ΟΟΟ„ΞΏ Ο‡Ξ±ΟΞ±ΞΊΟ„Ξ®ΟΞ±
	memmove(&numofrecords, block, sizeof(int));//ΞΟ‡ΞΏΟ…ΞΌΞµ Ο„ΞΏΞ½ Ξ±ΟΞΉΞΈΞΌΟ Ο„ΞΏΞ½ ΞµΞ³Ξ³ΟΞ±Ο†ΟΞ½ ΟƒΟ„ΞΏ ΞΌΟ€Ξ»ΞΏΞΊ Ξ΄ΞµΞ΄ΞΏΞΌΞ­Ξ½Ο‰Ξ½7
	printf("There are %d records in this block\n", numofrecords);
	block = (char*)block + sizeof(int);
	memmove(&dataOrder, block, sizeof(int));//Ξ£Ξµ Ο€ΞΏΞΉΞΏ ΞΌΟ€Ξ»ΞΏΞΊ Ξ²ΟΞΉΟƒΞΊΟΞΌΞ±ΟƒΟ„Ξµ
	block = (char*)block + sizeof(int);
	memmove(&value1, block, keyLength);// Ξ¤ΞΏ Ο€ΟΟΟ„ΞΏ ΞΊΞ»ΞµΞΉΞ΄Ξ―
	printf("Searching in block %d for the value\n", dataOrder);
	switch (op)
	{
	case 1:
		for (i = 0; i<numofrecords; ++i)
		{
			if (memcmp(&value1, keyValue, keyLength) == 0)
			{
				printf("Value that EQUALS Key : %d\n", value1);
				count++;
				succeeded = 1;
			}
			block = (char*)block + keyLength + VOIDPOINTERSIZE;//Ξ•Ο€ΟΞΌΞµΞ½ΞΏ ΞΊΞ»ΞµΞΉΞ΄Ξ― Ο„ΞΏΟ… ΞΌΟ€Ξ»ΞΏΞΊ
			memmove(&value1, block, sizeof(int));
		}
		printf("There are %d values that meet op %d\n", count, op);
		break;
	case 2:
		for (i = 0; i<numofrecords; ++i)
		{
			if ((memcmp(&value1, keyValue, keyLength) > 0) || (memcmp(&value1, keyValue, keyLength) < 0))
			{
				printf("Value NOT EQUALS Key : %d\n", value1);
				count++;
				succeeded = 1;
			}
			block = (char*)block + keyLength + VOIDPOINTERSIZE;//Ξ•Ο€ΟΞΌΞµΞ½ΞΏ ΞΊΞ»ΞµΞΉΞ΄Ξ― Ο„ΞΏΟ… ΞΌΟ€Ξ»ΞΏΞΊ
			memmove(&value1, block, sizeof(int));
		}
		printf("There are %d values that meet op %d\n", count, op);
		break;
	case 3:
		for (i = 0; i<numofrecords; ++i)
		{
			if (memcmp(&value1, keyValue, keyLength) < 0)
			{
				printf("Value LESS THAN Key : %d\n", value1);
				count++;
				succeeded = 1;
			}
			block = (char*)block + keyLength + VOIDPOINTERSIZE;//Ξ•Ο€ΟΞΌΞµΞ½ΞΏ ΞΊΞ»ΞµΞΉΞ΄Ξ― Ο„ΞΏΟ… ΞΌΟ€Ξ»ΞΏΞΊ
			memmove(&value1, block, sizeof(int));
		}
		printf("There are %d values that meet op %d\n", count, op);
		break;
	case 4:
		for (i = 0; i<numofrecords; ++i)
		{
			if (memcmp(&value1, keyValue, keyLength) > 0)
			{
				printf("Value GREATER THAN key : %d\n", value1);
				count++;
				succeeded = 1;
			}
			block = (char*)block + keyLength + VOIDPOINTERSIZE;//Ξ•Ο€ΟΞΌΞµΞ½ΞΏ ΞΊΞ»ΞµΞΉΞ΄Ξ― Ο„ΞΏΟ… ΞΌΟ€Ξ»ΞΏΞΊ
			memmove(&value1, block, sizeof(int));
		}
		printf("There are %d values that meet op %d\n", count, op);
		break;
	case 5:
		for (i = 0; i<numofrecords; ++i)
		{
			if ((memcmp(&value1, keyValue, keyLength) < 0) || (memcmp(&value1, keyValue, keyLength) == 0))
			{
				printf("Value LESS_THAN_OR_EQUAL Key : %d\n", value1);
				count++;
				succeeded = 1;
			}
			block = (char*)block + keyLength + VOIDPOINTERSIZE;//Ξ•Ο€ΟΞΌΞµΞ½ΞΏ ΞΊΞ»ΞµΞΉΞ΄Ξ― Ο„ΞΏΟ… ΞΌΟ€Ξ»ΞΏΞΊ
			printf("passing key length\n");
			memmove(&value1, block, sizeof(int));
		}
		printf("There are %d values that meet op %d\n", count, op);
		break;
	case 6:
		for (i = 0; i<numofrecords; ++i)
		{
			if ((memcmp(&value1, keyValue, keyLength) > 0) || (memcmp(&value1, keyValue, keyLength) == 0))
			{
				printf("Value  GREATER_THAN_OR_EQUAL Key : %d\n", value1);
				count++;
				succeeded = 1;
			}
			block = (char*)block + keyLength + VOIDPOINTERSIZE;//Ξ•Ο€ΟΞΌΞµΞ½ΞΏ ΞΊΞ»ΞµΞΉΞ΄Ξ― Ο„ΞΏΟ… ΞΌΟ€Ξ»ΞΏΞΊ
			memmove(&value1, block, sizeof(int));
		}
		printf("There are %d values that meet op %d\n", count, op);
	default:
		printf("You gave wrong op code %d!\n", op);
		break;
	}
	printf("Done with the searching!\n");
	return succeeded;
}



//*********************************************************************************************************************



int moveRecords(int fileDesc,void* oldBlock,void* newBlock,void** newkey)
/*Η συνάρτηση αυτή δέχεται τον αναγνωριστικό αριθμό ανοίγματος αρχείου,ένα δείκτη στην αρχή του 
μπλοκ που σπάσαμε (παλιό),κι ένα δείκτη στην αρχή του νέου που δεσμεύσαμε,και μεταφέρει όσες εγγραφές χρειάζεται 
από το ένα στο άλλο*/
{
	printf("Im moving records because of split\n");
	printf("I'll move pairs from the oldblock %x to the newblock %x\n",oldBlock,newBlock);
	void* startold=oldBlock;
	void* startnew=newBlock;
	int newblockorder,oldblockorder;
	//Αρχικά παίρνουμε το "σειριακό" αριθμό κάθε μπλοκ
	startnew=(char*)startnew+sizeof(char)+sizeof(int);
	startold=(char*)startold+sizeof(char)+sizeof(int);
	memmove(&newblockorder,startnew,sizeof(int));
	memmove(&oldblockorder,startold,sizeof(int));
	//Διακρίνουμε 2 περιπτώσεις μεταφοράς:α)τα μπλοκ είναι φύλλα και β)τα μπλοκ είναι εσωτερικά
	if(memcmp(oldBlock,"L",sizeof(char))==0)
	{
		printf("The block is a leaf\n");
		int p,i,n;
		int j;
		int keyLength=getTheKeyLength(fileDesc);
		oldBlock=(char*)oldBlock+sizeof(char);//Τώρα δείχνει στον αριθμό εγγραφών
		memmove(&p,oldBlock,sizeof(int));
		printf("The old block had %d pairs\n",p);
		j=ceil(p/2);
		n=p-j;
		printf("The old block gets to keep %d pairs\n",j);
		memmove(oldBlock,&j,sizeof(int));
		oldBlock=(char*)oldBlock+2*sizeof(int);
		for(i=0;i<j;++i)
		{
			oldBlock=(char*)oldBlock+keyLength+VOIDPOINTERSIZE;	
		}
		newBlock=(char*)newBlock+sizeof(char);
		memmove(newBlock,&n,sizeof(int));
		printf("The new block gets to have %d pairs\n",n );
		newBlock=newBlock+2*sizeof(int);
		printf("OK,now copying data from the old block to the new\n");
		for(i=j;i<p;++i)
		{
			//printf("Cpying pairs...\n");
			memmove(newBlock,oldBlock,keyLength);
			oldBlock=(char*)oldBlock+keyLength;
			newBlock=(char*)newBlock+keyLength;
			memmove(newBlock,oldBlock,VOIDPOINTERSIZE);
			oldBlock=(char*)oldBlock+VOIDPOINTERSIZE;
			newBlock=(char*)newBlock+VOIDPOINTERSIZE;
		}
		free((*newkey));
		(*newkey)=NULL;
	}
	else if(memcmp(oldBlock,"I",sizeof(char))==0)
	{
		printf("I came here because a pretty internal node got split\n");
		int p,i,n;
		int j;
		int keyLength=getTheKeyLength(fileDesc);
		int dataLength=getTheDataLength(fileDesc);
		oldBlock=(char*)oldBlock+sizeof(char);//Τώρα δείχνει στον αριθμό εγγραφών
		memmove(&p,oldBlock,sizeof(int));
		j=floor(p/2);
		n=p-j;
		memmove(oldBlock,&j,sizeof(int));
		oldBlock=(char*)oldBlock+2*sizeof(int)+VOIDPOINTERSIZE;
		for(i=0;i<j;++i)
		{
			oldBlock=(char*)oldBlock+keyLength+VOIDPOINTERSIZE;	
		}
		memmove((*newkey),oldBlock,keyLength);
		oldBlock=(char*)oldBlock+keyLength+VOIDPOINTERSIZE;
		newBlock=(char*)newBlock+sizeof(char);
		memmove(newBlock,&n,sizeof(int));
		newBlock=newBlock+2*sizeof(int)+VOIDPOINTERSIZE;
		for(i=j+1;i<p;++i)
		{
			memmove(newBlock,oldBlock,(keyLength+VOIDPOINTERSIZE));
			oldBlock=(char*)oldBlock+keyLength+VOIDPOINTERSIZE;
			newBlock=(char*)newBlock+keyLength+VOIDPOINTERSIZE;
		}
	}
	printf("Now going to write the data to the blocks\n");
	int returnValue1,returnValue2;
	returnValue1=BF_WriteBlock(fileDesc,oldblockorder);
	returnValue2=BF_WriteBlock(fileDesc,newblockorder);
	if(returnValue1<0)
	{
		printf("Error writing data in oldBlock\n");
	}
	if(returnValue2<0)
	{
		printf("Error writing data in newBlock\n");
	}
	if((returnValue1*returnValue2)==0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}



//*********************************************************************************************************************



int moveToWriteInLeaf(int fileDesc,void* leafStart,void* key,void* pointer,int place)
//Moves the records in a leaf in order to write the new pair <key,pointer> to the right position-place
//Returns a negative number in case of error;1 otherwise
{
	//Place points the position where the new pair must be inserted
	//To do so,we need to move certain pairs
	int numofpairs;//Ο αριθμός των ζευγαριών <κλειδί,δείκτης> μέσα στο φύλλο
	int blockorder;
	int keyLength=getTheKeyLength(fileDesc);
	int dataLength=getTheDataLength(fileDesc);
	void* leaf=leafStart;//Κρατάμε τόν δείκτη στην αρχή του φύλλου
	leafStart=(char*)leafStart+sizeof(char);//Τώρα δείχνει στον αριθμό των εγγραφών
	memmove(&numofpairs,leafStart,sizeof(int));
	printf("Number of pairs : %d\n", numofpairs);
	//++numofpairs;
	//memmove(leafStart,&numofpairs,sizeof(int));//Τώρα θα έχουμε ένα ζευγάρι παραπάνω
	--numofpairs;//Χρειαζόμαστε την αρχική τιμή
	leafStart=(char*)leafStart+sizeof(int);
	memmove(&blockorder,leafStart,sizeof(int));//Ο "σειριακός" αριθμός του μπλοκ
	leafStart=(char*)leafStart+sizeof(int);//Τώρα ο δείκτης δείχνει στο κλειδί του πρώτου ζευγαριού
	int i;
	printf("I need to put it in place:%d\n",place);
	for(i=0;i<place;++i)
	{//Πορχωράμε μέχρι να φθάσουμε στη σωστή θέση
		//printf("in chaingin place loop for leaf...\n");
		leafStart=(char*)leafStart+keyLength+VOIDPOINTERSIZE;
	}
	//Αρχικά θα μετακινήσουμε τα ήδη εγγεγραμμένα ζεύγη μία θέση δεξιότερα.
	void* newposition=leafStart;
	//Αυτή είναι η καινούργια θέση απ'όπου θα ξεκινούν οι παλιές εγγραφές
	newposition=(char*)newposition+keyLength+VOIDPOINTERSIZE;
	if(place==0)
	{
		memmove(newposition,leafStart,(numofpairs*(keyLength+VOIDPOINTERSIZE)));//Μετακίνηση
	}
	else
	{
		memmove(newposition,leafStart,((numofpairs-i-1)*(keyLength+VOIDPOINTERSIZE)));//Μετακίνηση
	}
	//memmove(newposition,leafStart,((numofpairs-place)*(keyLength+VOIDPOINTERSIZE)));//Μετακίνηση
	memmove(leafStart,key,keyLength);//Και ακολούθως θα εισάγουμε το νέο ζευγάρι στη σωστή θέση
	leafStart=(char*)leafStart+keyLength;
	memmove(leafStart,&pointer,VOIDPOINTERSIZE);
	int returnValue=BF_WriteBlock(fileDesc,blockorder);//Εγγράφουμε τις αλλαγές στο επίπεδο μπλοκ
	if(returnValue<0)
	{
		printf("Error writing block\n");
	}
	return returnValue;
}



//*********************************************************************************************************************



int moveToWriteInInternal(int fileDesc,void* internalStart,void* key,void*pointer,int place)
//Moves the records in an internal node/block in order to write the new pair <key,pointer> to the right position-place
//Returns a negative number in case of error;1 otherwise
{
	int numofpointers, i;//Ο αριθμός των δεικτών μέσα στο μπλοκ
	int blockorder;//Ο "σειριακός" αριθμός του μπλοκ
	int keyLength=getTheKeyLength(fileDesc);
	internalStart=(char*)internalStart+sizeof(char);//Τώρα δείχνει στον αριθμό των δεικτών
	memmove(&numofpointers,internalStart,sizeof(int));
	--numofpointers;//Χρειαζόμαστε την παλιά τιμή
	internalStart=(char*)internalStart+sizeof(int);
	memmove(&blockorder,internalStart,sizeof(int));
	internalStart=(char*)internalStart+sizeof(int)+VOIDPOINTERSIZE;//Τώρα δείχνει στο πρώτο κλειδί
	for(i=0;i<place;++i)
	{//Προχωράμε μέχρι να φθάσουμε στη σωστή θέση
		internalStart=(char*)internalStart+keyLength+VOIDPOINTERSIZE;
	}
	//Αρχικά θα μετακινήσουμε τα ήδη εγγεγραμμένα ζεύγη μία θέση δεξιότερα.
	void* newposition=internalStart;
	//Αυτή είναι η καινούργια θέση απ'όπου θα ξεκινούν οι παλιές εγγραφές
	newposition=(char*)newposition+keyLength+VOIDPOINTERSIZE;
	if(place==0)
	{	
		memmove(newposition,internalStart,((numofpointers-1)*(keyLength+VOIDPOINTERSIZE)));//Μετακίνηση
	}
	else
	{
		memmove(newposition,internalStart,(((numofpointers-1)-i-1)*(keyLength+VOIDPOINTERSIZE)));//Μετακίνηση
	}
	memmove(internalStart,key,keyLength);//Και ακολούθως θα εισάγουμε το νέο ζευγάρι στη σωστή θέση
	internalStart=(char*)internalStart+keyLength;
	memmove(internalStart,&pointer,VOIDPOINTERSIZE);
	int returnValue=BF_WriteBlock(fileDesc,blockorder);//Εγγράφουμε τις αλλαγές στο επίπεδο μπλοκ
	if(returnValue<0)
	{
		printf("Error writing block\n");
	}
	return returnValue;
}