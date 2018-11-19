typedef struct ParentStackNode
{
	void* parent;
	struct ParentStackNode* next;
}ParentStackNode;

typedef ParentStackNode* ParentStackNodePointer;


//While going down a tree we need to keep the previous nodes in a stack,in case of split
//This stack is implemented as a list


void createEmptyParents(ParentStackNodePointer* root);
//Creates an empty stack of parents

int isEmptyParents(ParentStackNodePointer root);
//Returns 1 if the stack is empty;0 otherwise

int pushNewParent(ParentStackNodePointer* root,void* parent);
//Pushes a new parent node at the "top" of the stack

void* popTheLastParent(ParentStackNodePointer* root);
//Pops the last in parent

void destroyTheStack(ParentStackNodePointer* root);
//Destroys the stack