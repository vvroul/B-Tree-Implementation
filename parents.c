#include <stdio.h>
#include <stdlib.h>
#include "parents.h"


void createEmptyParents(ParentStackNodePointer* root)
//Creates an empty stack of parents
{
	(*root) = NULL;
}




int isEmptyParents(ParentStackNodePointer root)
//Returns 1 if the stack is empty;0 otherwise
{
	if (root == NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}




int pushNewParent(ParentStackNodePointer* root, void* parent)
//Adds a new parent node at the "top" of the stack
{
	ParentStackNodePointer newparent = malloc(sizeof(ParentStackNodePointer));
	if (newparent != NULL)
	{
		newparent->parent = parent;
		newparent->next = (*root);
		(*root) = newparent;
		return 1;
	}
	return 0;
}




void* popTheLastParent(ParentStackNodePointer* root)
//Pops a parent out of the stack
{
	void* toreturn;
	if ((*root != NULL))
	{
		toreturn = (*root)->parent;
		(*root) = (*root)->next;
		return toreturn;
	}
	return NULL;
}




void destroyTheStack(ParentStackNodePointer* root)
//Destroys the stack
{
	if ((*root) != NULL)
	{
		ParentStackNodePointer assistant = (*root);
		ParentStackNodePointer temp;
		while (assistant != NULL)
		{
			temp = assistant->next;
			free(assistant);
			assistant=temp;
		}
	}
}