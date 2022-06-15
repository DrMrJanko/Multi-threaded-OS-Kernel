/*
 * PCBlist.cpp
 *
 *  Created on: May 10, 2021
 *      Author: OS1
 */
#include "PCBlist.h"

#include "aglobal.h"
#include "KSem.h"
extern int syncPrintf(const char *format, ...);

int PCBList::empty(){
	 if(prvi==0) return 1;
	 return 0;
}

PCB* PCBList::removeFirst(){
lock
  if(prvi==0){
	  unlock
	  return 0;
  }
  PCB* a= prvi->info;
  elem* x=prvi;
  prvi->next->pre=0;
  prvi=prvi->next;
  if(prvi==0)posl=0;
  delete x;
  unlock
return a;
}

void PCBList::add(PCB* a){
	elem* x=new elem(a);
	if(posl==0){
     prvi=x;
     posl=x;
     return;
	}
	posl->next=x;
	x->pre=posl;
	posl=x;
}

PCBList::~PCBList(){
 while(prvi){
	 elem* tmp= prvi;
	 prvi=prvi->next;
	 delete tmp;
 }
}

int PCBList::removePCB(PCB* a){
	lock
	if(prvi==0){
		unlock
		return 0;
	}
	elem* tmp=posl,*prev=0;
	while(tmp!=0 && tmp->info->getId()!=a->getId()){
		prev=tmp;
		tmp=tmp->pre;
	}
	if(tmp==0){
		unlock
		return 0;
	}
	if(tmp==posl){
		if(prvi==posl){
			prvi=0;
			posl=0;
		}
		else {
			posl->pre->next=0;
			posl=posl->pre;
		}
	}else{
		if(tmp==prvi)prvi=prev;
		prev->pre=tmp->pre;
	}
	delete tmp;
	unlock
	return 1;
}

void PCBList::notify(){
lock
elem* tmp=0;
for(tmp=prvi;tmp!=0;tmp=tmp->next)tmp->info->sleepingTime--;
	for(tmp=posl;tmp!=0;){
		elem* prev=tmp->pre;
		if(tmp->info->sleepingTime<=0){
				tmp->info->state=PCB::READY;
				tmp->info->sleepingTime=0;
				tmp->info->waitingOn=0;
				tmp->info->signalized=0;
				if(tmp->info->waitingOn != 0) tmp->info->waitingOn->delPCB(tmp->info);
				Scheduler::put(tmp->info);
				this->removePCB(tmp->info);
			}
		tmp=prev;
	}
unlock
}






