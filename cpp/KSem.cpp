/*
 * KSem.cpp
 *
 *  Created on: May 17, 2021
 *      Author: OS1
 */
#include "KSem.h"

#include "aglobal.h"
#include "Queue.h"
#include "PCB.h"

extern syncPrintf(const char* fromat,...);
extern void dispatch();
KernelSem::KernelSem(int a){
	lock
	value=a;
	myQueue=new Queue();
	unlock
}

int KernelSem::wait(Time t){
	lock
	PCB::running->signalized=0;
	value--;
	if(value<0){
		PCB::running->state=PCB::BLOCKED;
		PCB::running->waitingOn=this;
		myQueue->ins(PCB::running);

		if(t>0){
			PCB::running->sleepingTime=t;
			Global::blockedPCBs->add(PCB::running);
		}
		unlock
		dispatch();
		return PCB::running->signalized;
	}else{
		unlock
		return 1;
	}

}


void KernelSem::signal(){
	lock
	value++;
		if(value<=0){
			PCB* pcb=myQueue->del();
			pcb->state=PCB::READY;
			pcb->waitingOn=0;
			pcb->sleepingTime=0;
			pcb->signalized=1;
			int a=Global::blockedPCBs->removePCB(pcb);
			Scheduler::put(pcb);
		}
	unlock
}

int KernelSem::getValue()const{return value;}

KernelSem::~KernelSem(){
 lock
 delete myQueue;
 unlock
}

void KernelSem::delPCB(PCB* a){
lock
int x=myQueue->removePCB(a);
if(x==1)value++;
unlock
}
