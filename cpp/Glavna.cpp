/*
 * Glavna.cpp
 *
 *  Created on: Apr 23, 2021
 *      Author: OS1
 */
#include <IOSTREAM.H>

#include "aglobal.h"
#include "PCBlist.h"
#include "PCB.h"
#include "thread.h"
#include "SCHEDULE.H"
#include "Queue.h"
extern void inic();
extern void restore();
extern unsigned counter;
extern int userMain(int,char**);


int main(int argc,char** argv){
	Global::inic();
	Global::userMainThread=new uMainThr(argc,argv);
	Global::userMainThread->start();
	Global::userMainThread->waitToComplete();
	Global::restore();
return Global::retVal;

}
