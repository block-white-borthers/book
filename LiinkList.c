#include "./lib/LinkList.h"
/* get struct node or LinkList */
LinkList* LinkListInit()
{
 LinkList *head;/* LinkList */
 head = (LinkList*)malloc(sizeof(LinkList));/* malloc memory to head */
 head->next = NULL;/* set head is NULL*/
 return head;/* return value */
}

elemtype makenode()
{
elemtype elem;/* var */
printf("please enter student id\n");/* tip  */
scanf("%s",elem.id);/* */
printf("please enter student name \n");/* tip */
scanf("%s",elem.name);/* */
printf("make node success \n");/* tip */
return elem;/* return LinkList data */
}


int CreateLinkList(LinkList *head)
{
 LinkList *p,*q;/* */
 int i;/* */
 q = head;/**/

 for(i= 0 ;/**/i<2;/**/ i++){
 p=(LinkList*)malloc(sizeof(LinkList));/* node malloc memory */
 p->data=makenode();/* set data to node */
 q->next = p;/* a,b, temp */
 q = p;/* */
 q->next = NULL;/**/
 }
 printf("create success \n");/* tip */
 return 1;/* */
}

int formatdata(elemtype e)
{

printf("id:%s'tname:%s\n",e.id,e.name);/* print format data  */
return 1;/**/
}

int display(LinkList *head)
{
LinkList *p;/* */
p=head->next;/* */
while (p) {
 formatdata(p->data);/* call formatdata fun */
 p=p->next;/* get next chain */
}
printf("display success \n");/* tip */
return 1;/* */
}

int LinkListExample()
{
 LinkList *head;/* */
 head = LinkListInit();/* malloc LinkList head */
 CreateLinkList(head);/**/
 display(head);/* */
 return 1;/* */
}
