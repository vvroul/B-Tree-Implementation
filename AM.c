#ifndef AM_H_
#define AM_H_
#include "AM.h"

#define VOIDPOINTERSIZE 8

/* Error codes */
int AM_errno;



//*********************************************************************************************************************



void AM_Init( void )
{
	BF_Init();
}



//*********************************************************************************************************************



int AM_CreateIndex(
  char *fileName, /* όνομα αρχείου */
  char attrType1, /* τύπος πρώτου πεδίου: 'c' (συμβολοσειρά), 'i' (ακέραιος), 'f' (πραγματικός) */
  int attrLength1, /* μήκος πρώτου πεδίου: 4 γιά 'i' ή 'f', 1-255 γιά 'c' */
  char attrType2, /* τύπος πρώτου πεδίου: 'c' (συμβολοσειρά), 'i' (ακέραιος), 'f' (πραγματικός) */
  int attrLength2 /* μήκος δεύτερου πεδίου: 4 γιά 'i' ή 'f', 1-255 γιά 'c' */
)
{
	//We need to check whether the file already exists or not
	printf("Going to search if the file exists already\n");
	if(searchforfileintheList(filesList,fileName)==1)
	{
		printf("The file already exists!\n");
		AM_errno = AME_CRIN;
		return -1;
	}
	
	printf("Ok,going to create the file.\n");
	if(BF_CreateFile(fileName)<0)//Create the file
	{
		printf("Cannot create file.\n");
		AM_errno = AME_CRIN;
		return -1;
	}
	int heapDesc=BF_OpenFile(fileName);
	printf("Ok,going to open the file.\n");
	if(heapDesc<0)//Open it
	{
		printf("Cannot open file.\n");
		printf("%d\n",BF_OpenFile(fileName));
		AM_errno = AME_CRIN;
		return -1;
	}
	printf("heapDesc=%d\n",heapDesc);
	printf("Allocate a block for it\n");
	if(BF_AllocateBlock(heapDesc)<0)//Allocate a block for it
	{
		printf("Cannot allocate a block for this file\n");
		printf("%d\n",BF_AllocateBlock(heapDesc));
		AM_errno = AME_CRIN;
		return -1;
	}
	//Update the data structure that holds the info about all the files that are created
	printf("Get the num of blocks\t");
	int numofblocks=BF_GetBlockCounter(heapDesc);
	printf("%d\n",numofblocks);
	if(numofblocks<=0)
	{
		printf("Error,%d\n",numofblocks);
		AM_errno = AME_CRIN;
		return -1;
	}
	void* block;
	void* startblock;
	if(BF_ReadBlock(heapDesc,numofblocks-1,&block)<0)
	{
		AM_errno = AME_CRIN;
		return -1;
	}
	printf("%x\n",block);
	startblock=block;
	char leafInfo[]="L";
	int numofrecs=0;
	int* pointertorecs=&numofrecs;
	numofblocks=numofblocks-1;
	int* pointertonumofblocks=&numofblocks;
	memcpy(block,leafInfo,sizeof(char));
	char type[3];
	char* address=(char*)block;
	strcpy(type,address);
	printf("The type is %s\n",type);
	block=(char*)block+sizeof(char);
	memcpy(block,pointertorecs,sizeof(int));
	pointertorecs=(int*)block;
	printf("The current numner of records is %d\n",(*pointertorecs));
	block=(char*)block+sizeof(int);
	memcpy(block,pointertonumofblocks,sizeof(int));
	pointertonumofblocks=(int*)block;
	printf("The current numner of blocks is %d\n",(*pointertonumofblocks));
	block=startblock;
	numofblocks=numofblocks+1;
	if(BF_WriteBlock(heapDesc,numofblocks-1)<0)
	{
		AM_errno = AME_CRIN;
		return -1;
	}
	else
	{
		printf("%x\n",startblock);
		printf("Ok,going to add it in the list\n");
		addtoList(&filesList,fileName,attrLength1,attrLength2, attrType1, attrType2, block);
		printf("Added it in the list\n");
	}
	if(BF_CloseFile(heapDesc)<0)
	{
		AM_errno = AME_CRIN;
		return -1;
	}
	else
	{
		printf("RETURNING\n");
		return AME_OK;
	}
}



//*********************************************************************************************************************



int AM_DestroyIndex(
  char *fileName /* όνομα αρχείου */
)
{
	printf("I will destroy the file %s\n",fileName);
	if (deleteanodefromList(&filesList,fileName) < 0)
	{
		AM_errno = AME_DIN;
		return AME_DIN;
	}
	else 
	{
		printf("Index Destroyed\n");
		return AME_OK;
	}
}



//*********************************************************************************************************************



int AM_OpenIndex (
  char *fileName /* όνομα αρχείου */
)
{
	printf("I will open the file %s\n",fileName);
	int heapDesc=BF_OpenFile(fileName);
	printf("ok,going to open the file\n");
	if(heapDesc<0)//Open it
	{
		printf("Cannot open file\n");
		printf("%d\n",BF_OpenFile(fileName));
		AM_errno = CANNOT_OPEN_FILE;
		return CANNOT_OPEN_FILE;//Can't open the file
	}
	printf("heapDesc=%d\n",heapDesc);
	switch(addtoOpenList(&openList,heapDesc,fileName))
	{
		case -1:
			AM_errno = LIST_FULL;
			return LIST_FULL;
		case -2:
			AM_errno = NO_MEMORY;
			return NO_MEMORY;
		default:printf("Opened file %s\n",fileName);
		return AME_OK;
	}
}



//*********************************************************************************************************************



int AM_CloseIndex (
  int fileDesc /* αριθμός που αντιστοιχεί στο ανοιχτό αρχείο */
)
{
	printf("I will close the file %d\n",fileDesc);
	if(deleteanodefromOpenList(&openList,fileDesc)==1)
	{
		printf("Closed\n");
		return AME_OK;
	}
	AM_errno = NO_SUCH_OPEN;
	return NO_SUCH_OPEN;
}



//*********************************************************************************************************************



int AM_InsertEntry(int fileDesc,void* value1,void* value2)
{
	/*Η πρώτη δουλειά που πρέπει να κάνουμε είναι να φθάσουμε στο φύλλο.Εδώ υπάρχουν 2 περιπτώσεις:
	-Ο κόμβος/ρίζα που θα δοθεί ως είσοδος να είναι φύλλο
	-Ο κόμβος/ρίζα να είναι εωτερικός και να πρέπει να φθάσουμε στο φύλλο
	*/
	int keyLength=getTheKeyLength(fileDesc);
	int dataLength=getTheDataLength(fileDesc);
	void* block=getTheRoot(fileDesc);//Now block points to the start of the root-block
	int pi=getThePi(fileDesc);//Max number of pointers in an internal block
	int pl=getThePl(fileDesc);//Max number of pointers in a leaf block
	ParentStackNodePointer parentstack;
	createEmptyParents(&parentstack);
	int numofparents=0;
	int isLeaf=isItALeaf(block);
	void* previousblock=block;//Όταν θα μπούμε στην επανάληψη,χρειαζόμαστε και την τιμή του προηγούμενου μπλοκ
	while(!isLeaf)
	{
		void* nextblock;
		findTheNextNode(previousblock,value1,keyLength,&nextblock);
		pushNewParent(&parentstack, previousblock);
		isLeaf=isItALeaf(nextblock);
		if(isLeaf)
		{	
			block=nextblock;
		}
		else
		{
			previousblock=nextblock;
		}
	}
	//Αφού φθάσουμε στο φύλλο,πρέπει να το διασχίσουμε για να βρούμε πού θα γράψουμε το νέο ζευγάρι <κλειδί,δείκτης>
	//Στη συνέχεια αν είναι γεμάτο,πρέπει να το σπάσουμε σε 2 άλλα φύλλα
	//Τέλος πρέπει να κάνουμε τις εγγραφές
	//Θα ξεκινήσουμε ανάποδα,με τις εγγραφές
	//printf("Going to write the data to the block\n");
	void* pointertotherecord=WriteRecord(value1, value2, fileDesc);
	void* leafblockstart=block;//Κρατάμε την αρχική τιμή του leafblock
	void* myBlock=leafblockstart;//Κρατάμε τον δείκτη στην αρχή του φύλλου
	int i;
	int place=-1;
	int pointersintheblok;//Ο αριθμός των δεικτών στο μπλοκ φύλλο
	block=(char*)block+sizeof(char);//Τωρα το leafblock δείχνει στον αριθμό των εγγραφών
	memmove(&pointersintheblok, block,sizeof(int));
	++pointersintheblok;
	memmove(block, &pointersintheblok ,sizeof(int));//Update στον αριθμό των δεικτώ
	--pointersintheblok;
	block=(char*)block+2*sizeof(int);//Τώρα δείχνει στην πρώτη εγγραφή
	void* key=malloc(sizeof(keyLength));
	for(i=0;i<pointersintheblok;++i)
	{
		memmove(key,block,keyLength);//Τώρα το key έχει την τιμή του κλειδιού που βρισκόμαστε
		if(memcmp(value1,key,keyLength)<0)
		{
			place=i;
			break;
		}
		block=(char*)block+keyLength+VOIDPOINTERSIZE;//Προχωράει στην επόμενη εγγραφή
	}
	free(key);
	if(place==-1)//Είτε είναι το μόνο μέχρι στιγμής είτε το μεγαλύτερο
	{
		memcpy(block,value1,keyLength);
		block=(char*)block+keyLength;
		memcpy(block,pointertotherecord,VOIDPOINTERSIZE);
		int blockorder;//Ποιο μπλοκ του αρχείου είναι
		leafblockstart=(char*)leafblockstart+sizeof(char)+sizeof(int);
		memmove(&blockorder,leafblockstart,sizeof(int));
		if (BF_WriteBlock(fileDesc,blockorder) < 0)
		{
			BF_PrintError("Error while writing to block!");
			AM_errno = AME_INEN;
			return AME_INEN;
		}
	}
	else
	{
		
		moveToWriteInLeaf(fileDesc,leafblockstart,value1,pointertotherecord,place);
	}
	//Τωρα μένει να δούμε αν πρέπει να σπάσουμε τον κόμβο φύλλο
	int isSplit=0;//boolean
	void* newnode;//Δείκτης στο καινούργιο φύλλο,και γενικά στον οποιονδήποτε καινούργιο κόμβο προκύψει από ενδεχόμενη διάδοση της διάσπασης
	void* theSplitBlock = myBlock;//Δείκτης στο/στα μπλοκ που ενδέχεται να διασπαστεί/διασπαστούν
	void* newKey=malloc(keyLength);//Δείκτης σε νέο κλειδί που ενδέχεται να χρειαστούμε σε πρίπτωση διάδοσης της διάσπασης
	++pointersintheblok;
	if(pointersintheblok<=pl)
	{
		;
	}
	else
	{
		isSplit=1;
		newnode=split(theSplitBlock,fileDesc,&newKey);
	}
	if(isSplit==0)
	{
		destroyTheStack(&parentstack);
		return AME_OK;
	}
	//Αν όντως κληθηκε η split,τότε πρέπει να εγγράψουμε τη διεύθυνση στο καινούργιο μπλοκ,η οποία βρίσκεται στη μεταβλητή newblock,στον πατρικό εσωτερικό κόμβο
	/*Εδώ υπάρχουν 2 περιπτώσεις
	-Είτε σπάσαμε τον μοναδικό κόμβο που ήταν και φύλλο,κι επομένως τώρα έχουμε 2 φύλλα και πρέπει να φτιαξουμε έναν κόμβο-ρίζα,που θα είναι εσωτερικός
	-Σπάσαμε έναν εσωτερικό κόμβο
	Και στις 2 περιπτώσεις η διάσπαση ενδέχεται να διαδοθεί
	Τη διεύθυνση στην αρχή του καινούργιου μπλοκ την έχει η μεταβλητή newblock,και τη διεύθυνση στην αρχή του μπλοκ που σπάσαμε την έχει η μεταβλητή block
	*/
	while(isSplit)
	{
		//*Έλεγξε αν η στοίβα των γονέων είναι άδεια.Αν είναι,χρειάζεσαι καινούργια ρίζα.
		//Αν χρειάζεσαι καινούργια ρίζα τότε ο τελευταίος κόμβος που έσπασες είτε ήταν φύλλο,είτε κάποιος εσωτερικός.*/
		if (isEmptyParents(parentstack))//Χρεαζόμαστε καινούργια ρίζα
		{
			if(newKey==NULL)//Μόλις έχουμε μπει στην επανάληψη,επομένως οι 2 νέοι κόμβοι έχουν προέλθει από διάσπαση φύλλου
			{
				newKey=getTheNewKey(newnode);
			}	
			void* newroot=makeNewRoot(theSplitBlock, newnode, fileDesc,newKey);//TYPOS
			changeTheRoot(fileDesc,newroot);
			return AME_OK;	
		}
		else//Η στοίβα με τους πατρικούς κόμβους δεν είναι άδεια.Πρέπει να εισάγουμε το νέο ζευγάρι <κλειδί,δείκτης> στον πατέρα
		{
			void* parent = popTheLastParent(&parentstack);//ΠΑΡΕ ΤΟΝ ΤΕΛΕΥΤΑΙΟ ΓΟΝΙΟ ΑΠ'ΤΗΝ ΟΥΡΑ
			if(newKey==NULL)//Βρισκόμαστε εδώ μετά από διάσπαση φύλλου
			{
				newKey=getTheNewKey(newnode);
			}
			void* startparent = parent;//Θυμόμαστε τη διεύθυνση του γονέα
			void* currentkey = parent;//Ο δείκτης αυτός θα μάς βοηθήσει να διατρέξουμε τα κλειδιά στο γονικό μπλοκ
			void* startnew = newnode;
			parent = (char*)parent + sizeof(char);//Τώρα δείχνει στον αριθμό των δεικτών
			int numofpointers = 0;
			int parentsorder;//Η μεταβλητή αυτή φυλάει την τάξη του parent για τη BF_WriteBlock()
			memmove(&numofpointers, parent, sizeof(int));
			++numofpointers;
			memmove(parent, &numofpointers, sizeof(int));//Ενημερωσε τον αριθμό των δεικτών+1
			--numofpointers;
			parent=(char*)parent+sizeof(int);
			//Τώρα δείχνει στην τάξη του μπλοκ
			memmove(&parentsorder,parent,sizeof(int));
			parent = (char*)parent + sizeof(int)+VOIDPOINTERSIZE;//Και τώρα στο πρώτο κλειδί
			currentkey=(char*)currentkey+sizeof(char)+2*sizeof(int)+VOIDPOINTERSIZE;
			//memmove(currentkey, parent, keyLength);
			int i;
			int place = -1;
			for (i = 0; i < (numofpointers-1); ++i)//Θα διατρέξουμε το μπλοκ για να βρούμε πού θα εισάγουμε το ζευγάρι <κλειδί,δείκτης>
			{
				if (memcmp(currentkey, newKey, keyLength) < 0)
				{
					currentkey = (char*)currentkey + keyLength + VOIDPOINTERSIZE;
				}
				else if (memcmp(currentkey, newKey, keyLength) >= 0)
				{
					place = i;
					break;
				}
			}
			if (place < 0)//Είναι το μεγαλύτερο κλειδί
			{
				memmove(currentkey, newKey, keyLength);
				currentkey = (char*)currentkey + keyLength;
				memmove(currentkey, &newnode, VOIDPOINTERSIZE);
				if(BF_WriteBlock(fileDesc,parentsorder)<0)
				{
					printf("Error writing on block\n");
					AM_errno = AME_INEN;
					return AME_INEN;
				}
			}
			else
			{
				moveToWriteInInternal(fileDesc,startparent,newKey,newnode,place);
			}	
			//Έχουμε ν'αποφασίσουμε αν θα σπάσουμε και αυτόν τον κόμβο ή όχι
			if ((numofpointers + 1) < pi)
			{
				destroyTheStack(&parentstack);
				return AME_OK;
			}
			else
			{
				isSplit = 1;
				newnode = split(parent, fileDesc,&newKey);
				theSplitBlock = parent;
			}
		}
	}
}



//*********************************************************************************************************************



int AM_OpenIndexScan(
  int fileDesc, /* αριθμός που αντιστοιχεί στο ανοιχτό αρχείο */
  int op, /* τελεστής σύγκρισης */
  void *value /* τιμή του πεδίου-κλειδιού προς σύγκριση */
)
{
	int succeeded = 0, pos, numOfElements;
    printf("I will open the file for scan\n");
    char fileName[30];
    strcpy(fileName,returntheName(openList,fileDesc));
    printf("Our filename is : %s\n", fileName);
  	addtoScanList(&scanList, fileName);
    printf("We're gonna search for the value\n");
    succeeded = BPTSearch(fileDesc, value, op);
    if (succeeded == 1)
    {
        printf("We found the value!!\n");
    }
    numOfElements = numOfScanElements(scanList);
    if (numOfElements > MAXSCANS)
    {
    	AM_errno = AME_OPINS;
    	return AME_OPINS;
    }

    return returnthePosition(scanList, fileName);
}



//*********************************************************************************************************************



void *AM_FindNextEntry(
  int scanDesc /* αριθμός που αντιστοιχεί στην ανοιχτή σάρωση */
)
{

}



//*********************************************************************************************************************



int AM_CloseIndexScan(
  int scanDesc /* αριθμός που αντιστοιχεί στην ανοιχτή σάρωση */
)
{
	printf("I will close the scan %d\n", scanDesc);
	if (deleteanodefromScanList(&scanList, scanDesc) == 1)
	{
		printf("Scan closed\n");
		return AME_OK;
	}
	AM_errno = AME_CLINS;
	return AME_CLINS;
}



//*********************************************************************************************************************



void AM_PrintError(
  char *errString /* κείμενο για εκτύπωση */
)
{
	printf("%s", errString);;
    
    switch (AM_errno)
    {
        case 0 :
            break;
        case -1 :
            break;
        case -2 :
            BF_PrintError("Error in AM_CreateIndex : Problem while creating the file!");
            break;
        case -3 :
            BF_PrintError("Error in AM_DestroyIndex : Problem while deleting the file!");
            break;
        case -4 :
            BF_PrintError("Error in AM_OpenIndex : Cannot open the file!");
            break;
        case -5 :
            BF_PrintError("Error in AM_CloseIndex : No such open file!");
            break;
        case -6 :
            BF_PrintError("Error in AM_InsertEntry : Problem while inserting values to file!");
            break;
        case -7 :
            BF_PrintError("Error in AM_OpenIndexScan : Scan array is full!");
            break;
        case -8 :
            BF_PrintError("Error in AM_FindNextEntry : Problem while finding next entry!");
            break;
        case -9 :
            BF_PrintError("Error in AM_CloseIndexScan : Problem while terminating/deleting scan entry!");
            break;
        case -10 :
            BF_PrintError("Error in AM_OpenIndex : The list is full!");
            break;
        case -11 :
            BF_PrintError("Error in AM_OpenIndex : No memory left!");
            break;
        default :
            break;
    }
}



#endif /* AM_H_ */