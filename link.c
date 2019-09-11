#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


typedef  struct lrc{
	int m_time;
	char lrc_buf[200];
	struct lrc *front;
	struct lrc *next;
}LRC;


void link_print(LRC *head)
{
	LRC *pb = head;
	if(head == NULL)
		return;
	
	// while(pb->next != head){
		// printf("%d\t%s\n",pb->m_time,pb->lrc_buf);
		// pb = pb->next;		
	// }
	if(pb != NULL){
			printf("%d\t%s\n",pb->m_time,pb->lrc_buf);
			}	
	// printf("%d\t%s\n",pb->m_time,pb->lrc_buf);	
}
LRC *link_find(LRC *head,int m_time)
{
	LRC *pb = head;
	if(head == NULL)
		return NULL;
	while(pb->next != head){
		if(pb->m_time == m_time)
			return pb;
		pb = pb->next;		
	}	
	if(pb->m_time == m_time)
		return pb;	
	else
		return NULL;
}
LRC *link_delete(LRC *head,int m_time)
{
	LRC *p = head,*pb;
	if(head == NULL)
		return NULL;
	while(p->next != head){
		if(m_time == p->m_time){//找出要删除的节点
			if(p == head)//被删除的是头节点
				head = head->next;		
			break;
		}
		pb = p;
		p = p->next;		
	}
	if(p->m_time == m_time){//头、中间、结尾都需要交换
		p->next->front = p->front;
		p->front->next = p->next;	
		free(p);
		if(p == head)//只有一个节点的情况
			return NULL;
		else
			return head;
	}
	return head;
}
//带创建的插入接口
#if 0
LRC *link_insert(LRC *head,LRC *pnew)
{
	if(head == NULL){
		head = pnew;
		head->next = head;
		head->front = head;
	}else{
		head->next->front = pnew;
		pnew->next = head->next;
		head->next = pnew;
		pnew->front = head;
	}
	return head;
}
#else
//以学号升序插入
LRC *link_insert(LRC *head,LRC *pnew)
{
	LRC *pb = head;
	if(head == NULL){
		head = pnew;
		head->next = head;
		head->front = head;
	}else{
		while(pb->next != head){
			if(pnew->m_time < pb->m_time)
				break;
			pb = pb->next;		
		}
		if(pnew->m_time >= pb->m_time)
			pb = pb->next;
		pnew->next = pb;
		pnew->front = pb->front;
		pb->front->next = pnew;
		pb->front = pnew;
		if(pnew->m_time < head->m_time)
			head = pnew;
	}
	return head;
}
#endif
LRC *link_order(LRC *head)
{
	LRC temp,*pf=head,*pb;
	if(head == NULL)
		return head;
	while(pf->next != head){
		pb = pf->next;
		while(pb != head){
			if(pf->m_time > pb->m_time){
				//交换
				temp = *pf;
				*pf = *pb;
				*pb = temp;
				//temp.next = pb->next;
				pb->next = pf->next;
				pf->next = temp.next;
				//temp.front = pb->front;
				pb->front = pf->front;
				pf->front = temp.front;				
			}
			pb=pb->next;
		}
		pf = pf->next;
	}
	return head;
}




