/*
 * KEve.cpp
 *
 *  Created on: May 25, 2021
 *      Author: OS1
 */

#include "KEve.h"
#include "event.h"
#include <dos.h>

#include "aglobal.h"
extern void dispatch();

KernelEv::KernelEv(IVTNo n){
lock
owner=PCB::running;
entry=n;
blocked=0;
Global::entries[n]->myKEvent=this;
unlock
}

KernelEv::~KernelEv(){
	lock
	Global::entries[entry]->myKEvent=0;
	unlock
}

void KernelEv::wait() {
	lock
	if (PCB::running == owner) {
		owner->state = PCB::BLOCKED;
		blocked = 1;
		unlock
		dispatch();
		return;
	} else {
		unlock
	}
}

void KernelEv::signal() {
	lock
	if (blocked == 1) {
		owner->state = PCB::READY;
		blocked = 0;
		Scheduler::put(owner);
		unlock
		dispatch();
		return;
	}
	unlock
}

IVTEntry::IVTEntry(IVTNo id,void interrupt (*a)(...) ){
lock
this->old=getvect(id);
setvect(id,a);
this->myKEvent=0;
Global::entries[id]=this;
unlock
}


void IVTEntry::callOld(){
	lock
	if(old!=0)this->old();
	unlock
}

void IVTEntry::signal(){
if(this->myKEvent!=0)this->myKEvent->signal();
}
