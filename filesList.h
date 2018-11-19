#define MAXOPENFILES 20
#define MAXSCANS 20


typedef struct fileNode{
	//FileInfo thefile;
	char fileName[30];
	int keyLength;//4,0-255
	int dataLength;//4,0-255
	char keyType;// "i","f","c"
	char dataType;// "i","f","c"
	void* root;
	struct fileNode* next;
}fileNode;

typedef fileNode* fileNodePointer;
//^^Created files


typedef struct openNode{
	int id;//fileDesc
	char fileName[30];
	int position;//1-20
	struct openNode* next;
}openNode;

typedef openNode* openNodePointer;
//^^Open files


typedef struct scanNode{
	char fileName[30];
	int position;//1-20
	struct scanNode* next;
}scanNode;

typedef scanNode* scanNodePointer;
//^^Files under scan



//Global declaration of the roots of the three lists
extern fileNodePointer filesList;
extern openNodePointer openList;
extern scanNodePointer scanList;


//Functions' prototypes

int emptyList(fileNodePointer root);
//This function checks whether the list is empty or not
//If it is empty,it returns 1;otherwise it returns 0

void createList(fileNodePointer* root);
//Creates an empty new list of files

void addtoList(fileNodePointer* root,char* fileName,int keyLength,int dataLength,char keyType,char dataType,void* block);
//Adds a new node to the list

int searchforfileintheList(fileNodePointer currentnode,char* fileName);
//Searches for a file named fileName in the list.
//If this file exists,it returns 1;otherwise it returns 0	

void destroytheList(fileNodePointer* root);
//Destroys the list

int deleteanodefromList(fileNodePointer *root,char* fileName);
//Deletes the node for the file named fileName from the list

//int numOfElements(openNodePointer root);
//This function counts how many elements the list contains

int getkeyLength(fileNodePointer root,char* fileName);
//Returns the keyLegth of the file named fileName

int getdataLength(fileNodePointer root,char* fileName);
//Returns the dataLength of the file named fileName

char getkeyType(fileNodePointer root,char* fileName);
//Returns the type of variable that is the key

char getdataType(fileNodePointer root,char* fileName);
//Returns the type of variable that is the data

void* gettheRoot(fileNodePointer root,char* fileName);
//Returns the root of the B+ tree attached to the file named fileName

int emptyopens(openNodePointer root);
//This function checks whether the list is empty or not
//If it is empty,it returns 1;otherwise it returns 0

void createOpenList(openNodePointer* root);
//Creates an empty new list of files

int addtoOpenList(openNodePointer* root,int fileDesc,char* fileName);
//Adds a new node to the list
//Returns 1 if it adds the node to the list;otherwise a negative number

int searchforfileintheOpenList(openNodePointer currentnode,char* fileName);
//Searches for a file named fileName in the list.
//If this file exists,it returns 1;otherwise it returns 0	

void destroytheOpenList(openNodePointer* root);
//Destroys the list

int deleteanodefromOpenList(openNodePointer *root,int fileDesc);
//Deletes the node for the file named fileName from the list
 
int numOfOPenElements(openNodePointer root);
//Counts the elemnets in the list

char* returntheName(openNodePointer root,int fileDesc);
//Returns the name of the file with fileDesc,if it is in the list;otherwise NULL

int returnthefileDesc(openNodePointer root,char* fileName);
//Returns the fileDesc of a file named fileName

int emptyscans(scanNodePointer root);
//This function checks whether the list is empty or not
//If it is empty,it returns 1;otherwise it returns 0

void createScanList(scanNodePointer* root);
//Creates an empty new list of files

int addtoScanList(scanNodePointer* root,char* fileName);
//Adds a new node to the list
//Returns 1 if it adds the file in the list;otherwise a negative number

int searchforfileintheScanList(scanNodePointer currentnode,char* fileName);
//Searches for a file named fileName in the list.
//If this file exists,it returns 1;otherwise it returns 0	

void destroytheScanList(scanNodePointer* root);
//Destroys the list

int deleteanodefromScanList(scanNodePointer *root,int scanDesc);
//Deletes the node for the file named fileName from the list

int numOfScanElements(scanNodePointer root);
//Counts the elements in the list

char* returnthescanName(scanNodePointer root,int scanDesc);
//Returns the name of the file with scanDesc,if it is in the list;otherwise NULL

int returnthePosition(scanNodePointer root,char* fileName);
//Returns the position of a file named fileName