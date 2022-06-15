/*
 * PCB.cpp
 *
 *  Created on: Apr 28, 2021
 *      Author: OS1
 */

#include "PCB.h"
#include <dos.h>
#include "PCBlist.h"
#include <iostream.h>
//
#include <stdio.h>
//
#include "aglobal.h"
unsigned PCB::IDD=0;
PCB* PCB::running=0;
PCB* PCB::tmpPCB=0;
extern void dispatch();


PCB::PCB(Thread*const thr,StackSize sts ,Time t){
	lock
	stack=0;
	if(sts>=65536)stacksize=65535;
	else if(sts<1024)stacksize=1024;
	else stacksize=sts;
	stsInElem=stacksize/sizeof(unsigned int);
	quant=t;
	state=DEFAULT;
	myThread=thr;
	id=++IDD;
	infTime=(quant==0?1:0);
	signalized=0;
	waitingForMe=new PCBList();
	children=new PCBList();
	Global::allPCBs->add(this);
	waitingOn=0;
	sleepingTime=0;
	parentPCB=0;
	numOfChilds=0;
	waitingCount=0;
	waitingForChildren=0;
	unlock
}
void PCB::createPCB(){
	lock
	unsigned sts=stacksize/sizeof(unsigned int);
    	stack=new unsigned[sts];
    #ifndef BCC_BLOCK_IGNORE
      stack[sts-1] = 0x200;
      stack[sts-2] =FP_SEG(PCB::wrapper);
      stack[sts-3] = FP_OFF(PCB::wrapper);

    	sp = FP_OFF(&stack[sts-12]);
    	ss = FP_SEG(&stack[sts-12]);
    	bp = sp;
     stack[sts-12]=0;
    #endif
 unlock
}

 void PCB::wrapper(){
	 running->myThread->run();
	 while(running->waitingForMe->empty()!=1){
     PCB* a=running->waitingForMe->removeFirst();
     if(a==running->parentPCB){
    	 if(a->waitingForChildren==1){
    		 --(a->waitingCount);
    		 if(a->waitingCount==0){
    			 a->waitingForChildren=0;
    			 a->state=READY;
    			 Scheduler::put(a);
    		 }
    	 }else{
    		 a->state=READY;
    		 Scheduler::put(a);
    	 }
     }else{
     a->state=READY;
     Scheduler::put(a);}
	}
	 running->state=PCB::FINISHED;
     dispatch();

 }

 void PCB::waitToComplete(){
	 lock
	 if (running == this || this==Global::idle->getMyPCB() || this==Global::mainThr->getMyPCB()){
		 unlock
		 return;
	 }
	 if (this->state == PCB::FINISHED || this->state==PCB::DEFAULT){
		 unlock
		 return;
	 }
	 running->state = PCB::BLOCKED;
	 this->waitingForMe->add(running);
	 unlock
	 dispatch();
 }

 void PCB::start(){
	 lock
	 if(this->state == PCB::DEFAULT){
		createPCB();
	 	state = PCB::READY;
	 	Scheduler::put(this);
	 }
	 unlock
 }
 ID PCB::getRunningId(){
	 return PCB::running->id;
 }
 Thread* PCB::getThreadById(ID id){
	 lock
	 PCBList::elem* a = Global::allPCBs->prvi;
	 while (a != 0) {
	 	if (a->info->id == id){
	 		unlock
	 		return a->info->myThread;
	 	}
	 	a = a->next;
	 }
	 unlock
	 return 0;
 }

 ID PCB::getId(){return id;}

 PCB::~PCB(){
	 lock
	 if(children!=0)delete children;
	 if(waitingForMe!=0)delete waitingForMe;
	 if(stack!=0)delete stack;
	 unlock
 }


 StackSize offset;
 StackSize newOffset;
 unsigned condition;
 StackSize depth;
 unsigned* tmp;
 StackSize i;
 StackSize j;


void interrupt PCB::copyStack(){

tmpPCB->parentPCB=PCB::running;
PCB::running->children->add(tmpPCB);
PCB::running->numOfChilds++;

tmp=(unsigned*)MK_FP(_SS,_BP);//adresa stek pointera

i=1;
while(1){
tmpPCB->stack[tmpPCB->stsInElem-i]=running->stack[running->stsInElem-i];
if(&running->stack[running->stsInElem-i]==tmp)break;
i++;
}
depth=i;
//printf("%d\n\n\n",depth);

tmpPCB->sp=FP_OFF(&tmpPCB->stack[tmpPCB->stsInElem-depth]);
tmpPCB->bp=FP_OFF(&tmpPCB->stack[tmpPCB->stsInElem-depth]);
tmpPCB->ss=FP_SEG(&tmpPCB->stack[tmpPCB->stsInElem-depth]);

//for (j = 1 ; j<=70; j++) printf("%p ; %4x		%4p ; %4x\n", &this->stack[stsInElem - j],  this->stack[stsInElem - j], &running->stack[running->stsInElem - j],  running->stack[stsInElem - j]);
//printf("base pointer oldPCB: %d\n", _BP);
//printf("stack pointer oldPCB: %d\n", _SP);
//printf("stack segment oldPCB: %d\n", _SS);
//cout<<"base pointer newPCB "<<FP_OFF(this->bp)<<endl<<flush;
//cout<<"stack pointer newPCB:"<<FP_OFF(this->sp)<<endl<<flush;
//cout<<"stack segment newPCB:"<< FP_OFF(this->ss)<<endl<<flush;

//while(1);


offset=depth;

while((condition=tmpPCB->stack[tmpPCB->stsInElem-offset])!=0){
newOffset=(StackSize)((running->stack-(unsigned*)MK_FP(_SS,condition)+running->stsInElem));
tmpPCB->stack[tmpPCB->stsInElem-offset]=FP_OFF(tmpPCB->stack+tmpPCB->stsInElem-newOffset);
offset=newOffset;
}
//for (j = 1 ; j<=70; j++) printf("%p ; %4x		%4p ; %4x\n", &tmpPCB->stack[tmpPCB->stsInElem - j],  tmpPCB->stack[tmpPCB->stsInElem - j], &running->stack[running->stsInElem - j],  running->stack[running->stsInElem - j]);
//while(1);
}
