/*
 * UMThr.h
 *
 *  Created on: Jun 22, 2021
 *      Author: OS1
 */

#ifndef UMTHR_H_
#define UMTHR_H_


#include "thread.h"
#include "aglobal.h"
extern int userMain(int,char**);

class uMainThr:public Thread{
	int args;
	char** argv;
public:
	int res;
	uMainThr(int ars,char** arv):Thread(){
		args=ars;
		argv=arv;
		res=0;
	}
	 virtual void run(){
		 res=userMain(args,argv);
	}
	~uMainThr(){waitToComplete();}
};


#endif /* UMTHR_H_ */
