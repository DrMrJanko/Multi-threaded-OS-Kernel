/*
 * Queue.cpp
 *
 *  Created on: May 12, 2021
 *      Author: OS1
 */
#include "Queue.h"

#include "aglobal.h"
extern int syncPrintf(const char *format, ...);

void Queue::ins(PCB* a){
	lock
      elem* tmp=new elem(a);
      if(rear==0){
          rear=tmp;
          front=tmp;
          unlock
          return;
      }
      rear->next=tmp;
      tmp->pre=rear;
      rear=tmp;
     unlock
}

PCB* Queue::del(){
	lock
     elem* tmp=front;
     PCB* x=tmp->info;
     front->next->pre=0;
     front=front->next;
     if(front==0)rear=0;
     delete tmp;
     unlock
    return x;
}

int Queue::removePCB(PCB* a){
	lock
	if(rear==0){
		unlock
		return 0;
	}
	elem* tmp=rear,*prev=0;
	while(tmp!=0 && tmp->info!=a){
		prev=tmp;
		tmp=tmp->pre;
	}
	if(tmp==0){
		unlock
		return 0;
	}
	if(tmp==rear){
		if(front==rear){
			front=0;
			rear=0;
		}
		else {rear->pre->next=0;
			rear=rear->pre;
		}
	}else{
		if(tmp==front)front=prev;
		prev->pre=tmp->pre;
	}
	delete tmp;
	unlock
	return 1;
}




