#include <stdlib.h>
typedef struct BinaryST
{
    int data;
    struct BinaryST* Rnext;
    struct BinaryST* Lnext;
}NODE;

NODE* InsertionBST(int e,NODE*head){    //iterative implementation
    NODE* NewNode=(NODE*)malloc(sizeof(NODE));
    NewNode->data=e;
    NewNode->Rnext=NULL;
    NewNode->Lnext=NULL;
    if (head==NULL)
    return NewNode;

    NODE* current=head;
    NODE* prev=current;
    while (current!=NULL)
    {
        if (e>current->data)
        {
            prev=current;
            current=current->Rnext;
        }
        else if (e<current->data)
        {
            prev=current;
            current=current->Lnext;
        }

        else break;
        
    }
    if(e>prev->data)
    prev->Rnext=NewNode;
    else if(e<prev->data)
    prev->Lnext=NewNode;
    
    return head;
}

int Search(int e,NODE* head){
    if (head==NULL)
    return -1;

    if(head->data==e)
    return 1;

    if (e>head->data)
    return Search(e,head->Rnext);

    else if (e<head->data)
    return Search(e,head->Lnext);

}