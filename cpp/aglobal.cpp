/*
 * global.cpp
 *
 *  Created on: May 1, 2021
 *      Author: OS1
 */
#include "aglobal.h"

#include "thread.h"
#include <dos.h>
volatile int Global::contextOnDemand=0;
volatile unsigned int Global::lockFlag=1;
volatile Time Global::counter=defaultTimeSlice;
PCBList* Global::allPCBs=0;
Idle* Global::idle=0;
mainThread* Global::mainThr=0;
PCBList* Global::blockedPCBs=0;
IVTEntry* Global::entries[256]={0};
uMainThr* Global::userMainThread=0;
int Global::retVal=0;

void Global::dispatch(){ // sinhrona promena konteksta
	asm pushf;
	asm cli;
	Global::contextOnDemand = 1;
	timer();
	asm popf;
}

unsigned oldTimerSEG=0,oldTimerOFF=0;

void Global::inic(){


	allPCBs=new PCBList();
	blockedPCBs=new PCBList();
	idle =new Idle();
	idle->start();
	mainThr=new mainThread();


	PCB::running=mainThr->myPCB;
	PCB::running->state=PCB::READY;


	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}




int tbp;
int tsp;
int tss;


extern void tick();
void interrupt timer(){	// prekidna rutina
	if(!Global::contextOnDemand) {
		tick();
		asm cli;
	    asm int 60h;
	    asm sti;
	    if(Global::blockedPCBs->empty()==0) Global::blockedPCBs->notify();
	}
	if (!Global::contextOnDemand && PCB::running->infTime==0) Global::counter--;
	if ((Global::counter == 0 && PCB::running->infTime==0) || (Global::contextOnDemand==1)) {
	if(Global::lockFlag==1){
			Global::contextOnDemand=0;

#ifndef BCC_BLOCK_IGNORE
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp,bp
		}
#endif
		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp=tbp;

        if(PCB::running->state==PCB::READY && PCB::running!=Global::idle->getMyPCB())Scheduler::put(PCB::running);

        PCB::running=Scheduler::get();
        if(PCB::running==0) PCB::running=Global::idle->getMyPCB();

        tsp =PCB::running->sp;
		tss =PCB::running->ss;
        tbp =PCB::running->bp;

		Global::counter = PCB::running->quant;
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp,tbp
		}
#endif

	}else Global::contextOnDemand=1;}


}


void Global::restore(){

	if(PCB::running!=mainThr->myPCB) return;
	delete allPCBs;
	delete blockedPCBs;

	retVal=userMainThread->res;

	for(int i=0;i<256;i++){
		if(entries[i]!=0){
			setvect(i,entries[i]->old);
			entries[i]=0;
		}
	}

	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
	delete userMainThread;
	delete idle;
	delete mainThr;
}

