/*
 * mainThr.h
 *
 *  Created on: May 25, 2021
 *      Author: OS1
 */

#ifndef PROJEKAT2_MAINTHR_H_
#define PROJEKAT2_MAINTHR_H_


class mainThread:public Thread{
public:
	mainThread():Thread(){}
	~mainThread(){waitToComplete();}
};


#endif /* PROJEKAT2_MAINTHR_H_ */
