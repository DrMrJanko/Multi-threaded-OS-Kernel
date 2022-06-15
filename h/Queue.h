/*
 * Queue.h
 *
 *  Created on: May 12, 2021
 *      Author: OS1
 */

#ifndef PROJEKAT2_QUEUE_H_
#define PROJEKAT2_QUEUE_H_

class PCB;

class Queue{
    struct elem{
        struct elem* next,*pre;
        PCB* info;
        elem(PCB* a):info(a),next(0),pre(0){}
    };
    struct elem *rear,*front;
public:
    Queue(){
        front=0;
        rear=0;
    }
    void ins(PCB* a);
    PCB* del();
    int removePCB(PCB* a);
};



#endif /* PROJEKAT2_QUEUE_H_ */
