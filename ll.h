#include<stdlib.h>

typedef struct Structure
{
    int data;
    struct Structure* next;
}NODE;

NODE* Insertion(int e,NODE*head){
    NODE* NewNode=(NODE*)malloc(sizeof(NODE));
    NewNode->data=e;
    NewNode->next=NULL;
    if (head==NULL)
    return NewNode;
    NODE* current=head;
    while(current->next!=NULL){
        current=current->next;
    }
    current->next=NewNode;
    return head;

}


NODE* Delete(NODE* head,int e){
    NODE*current=head;
    if (head->data==e)
    {   head=current->next;
        free(current);
        return head;}
    
    while(current->next!=NULL){
      if (current->next->data==e)
      { 
        NODE*temp=current->next;
        current->next=current->next->next;
        free(temp);
        break;
      }
      current=current->next;
    }
    if(current->data==e){
        free(current);
    }
    return head;
}
