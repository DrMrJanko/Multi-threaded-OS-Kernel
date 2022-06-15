#ifndef PROJEKAT2_PCBLIST_H_
#define PROJEKAT2_PCBLIST_H_

#include "PCB.h"
#include "iostream.h"

class PCBList{
public:
	struct elem{
		struct elem* next,*pre;
		PCB* info;
		elem(PCB* a){
			next=0;
			pre=0;
			info=a;
		}
	};
struct elem* prvi,*posl;

PCBList(){
	prvi=posl=0;
}

void add(PCB* a);
PCB* removeFirst();
int removePCB(PCB* a);
void notify();
int empty();
~PCBList();

};
#endif /* PROJEKAT2_PCBLIST_H_ */
