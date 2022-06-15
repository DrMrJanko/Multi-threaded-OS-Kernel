/*
 * global.h
 *
 *  Created on: Apr 28, 2021
 *      Author: OS1
 */

#ifndef PROJEKAT2_AGLOBAL_H_
#define PROJEKAT2_AGLOBAL_H_

#define lock Global::lockFlag--;

#define unlock Global::lockFlag++;\
        if (Global::contextOnDemand==1 && Global::lockFlag==1) {\
                dispatch();\
}

#include "PCBlist.h"
#include "Idle.h"
#include "mainThr.h"
#include "event.h"
#include "UMThr.h"
extern void tick();
class Global{
public:
	friend class PCB;
	friend class Thread;
	friend class Idle;
	friend class PCBList;
	friend class KernelSem;


	static int retVal;
	static uMainThr* userMainThread;
	static IVTEntry* entries[];
	static PCBList* blockedPCBs;
	static mainThread* mainThr;
	static Idle* idle;
	static PCBList* allPCBs;
	static volatile unsigned int lockFlag;
	static volatile int contextOnDemand;
	static volatile Time counter;
	static void inic();
	static void restore();
	static void dispatch();

};

void interrupt timer();

#endif /* PROJEKAT2_AGLOBAL_H_ */
