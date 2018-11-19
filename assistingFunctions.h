//This header file contains the prototypes and other important info about some functions that are used for various purposes within the program


int getThePl(int fileDesc);
//This function returns the number of pointers that can be stored within a leaf block

int getThePi(int fileDesc);
//This function returns the number of pointers that can be stored within an internal block

int getThenumberOfRecords(int fileDesc);
//This function returns the number of records that can be stored in a data block 

void* getTheRoot(int fileDesc);
//This functions returns the root of a B+ tree attached to the fileDesc file

int getTheKeyLength(int fileDesc);
//Returns the key length

int getTheDataLength(int fileDesc);
//Returns the data length

char getTheDataType(int fileDesc);
//Returns the type of variable that is the data

char getTheKeyType(int fileDesc);
//Returns the type of variable that is the key

void* UpdateRecords(void* root);
//This function returns the block we want, with updated number of records (+1)

void* split(void* theBlocktoSplit,int fileDesc,void** newkey);
//Splits a block and returns a pointer to the new one
//newkey will be freed we split a leaf.If we split an internal node,newkey will point to the key that must be stored to the parent

void* WriteRecord(void* value1,void* value2,int fileDesc);
//Writes a record on a data block

int isItALeaf(void* start);
//Returns 1 if start pointer is leaf, 0 otherwise

void findTheNextNode(void* theblock,void* value1,int keyLength,void** nextnode);
//The pointer nextnode gets to point at the new block

void searchANDprint(int fileDesc);
//Finds a data block and prints the data

int isItAData(void* start);
//Returns 1 if start pointer, points at data, 0 otherwise

int BPTSearch(int fileDesc, void* value, int op);
//Searches for a key in a specific data block

void* getTheNewKey(void* newnode);
//Returns a pointer to the first key of the newnode (the newnode is a leaf)

void* makeNewRoot(void* splitBlock, void* newnode, int fileDesc,void* newkey);
//Creates a new root for the tree,and returns a pointer to it

void changeTheRoot(int fileDesc, void* newroot);
//Changes the root of the B+ tree assosiated with the file fileDesc

int moveRecords(int fileDesc,void* oldBlock,void* newBlock,void** newkey);
//Moves half the records fron the old block to the new block
//Returns a negative number in cas of error;otherwise 1

int moveToWriteInLeaf(int fileDesc,void* leafStart,void* key,void* pointer,int place);
//Moves the records in a leaf in order to write the new pair <key,pointer> to the right position-place
//Returns a negative number in case of error;1 otherwise

int moveToWriteInInternal(int fileDesc,void* internalStart,void* key,void*pointer,int place);
//Moves the records in an internal node/block in order to write the new pair <key,pointer> to the right position-place
//Returns a negative number in case of error;1 otherwise

void printTheLeaf(void* leafStart);
//This function is for debbuging purposes.Prints the contents of a leaf block