#include "thread.h"

#include "aglobal.h"
#include "PCB.h"
#include "PCBlist.h"
//
#include <stdio.h>
//
void dispatch(){
	Global::dispatch();
}

void Thread::start() {
myPCB->start();
}

Thread::Thread(StackSize ss, Time ts) {
	lock
    myPCB=new PCB(this,ss,ts);
	unlock
}


Thread* Thread::getThreadById(ID id) {
return PCB::getThreadById(id);
}

ID Thread::getRunningId() {
return PCB::running->id;
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

ID Thread::getId() {
return myPCB->getId();
}

PCB* Thread::getMyPCB() {
return myPCB;

}

Thread::~Thread() {
lock
	delete myPCB;
unlock
}

Thread* Thread::clone()const{
Thread* a=new Thread(this->myPCB->stacksize,this->myPCB->quant);
a->myPCB->createPCB();
return a;

}


ID Thread::fork(){
lock
Thread * a=PCB::running->myThread->clone();
if(a==0){
	unlock
	return -1;
}
if(a->myPCB==0){
	unlock
	return -1;
}
//for (int j = 1 ; j<=70; j++) printf("%4p ; %4x\n", &PCB::running->stack[PCB::running->stsInElem - j],  PCB::running->stack[PCB::running->stsInElem - j]);

PCB::tmpPCB=a->myPCB;
PCB::copyStack();
PCB::tmpPCB=0;
if(PCB::running->id!=a->myPCB->parentPCB->id){
	return 0;
}

a->myPCB->state=PCB::READY;
Scheduler::put(a->myPCB);

unlock
return a->myPCB->id;
}


void Thread::waitForForkChildren(){
lock
if(PCB::running->children->empty()){
	unlock
	return;
 }
PCB::running->waitingForChildren=1;
PCB::running->state=PCB::BLOCKED;
for(PCBList::elem* tmp=PCB::running->children->prvi;tmp!=0;tmp=tmp->next){
tmp->info->waitingForMe->add(PCB::running);
PCB::running->waitingCount++;
}
unlock
dispatch();
}

void Thread::exit(){
lock
if(PCB::running->parentPCB!=0){
	if(PCB::running->parentPCB->waitingForChildren==1){
		PCB::running->parentPCB->waitingCount--;
		PCB::running->waitingForMe->removePCB(PCB::running->parentPCB);
		if(PCB::running->parentPCB->waitingCount==0){
			PCB::running->parentPCB->state=PCB::READY;
			Scheduler::put(PCB::running->parentPCB);
		}
	}
}
while(PCB::running->waitingForMe->empty()==0){
PCB* tmp=PCB::running->waitingForMe->removeFirst();
tmp->state=PCB::READY;
Scheduler::put(tmp);
}
PCB::running->state=PCB::FINISHED;
unlock
dispatch();
}
