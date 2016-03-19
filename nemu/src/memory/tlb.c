#include "common.h"
#include<time.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct 
{
	uint32_t tag;
	uint32_t pagestartaddr;
	uint8_t valid;
}tlbblock;

tlbblock tlb[64];

void refreshtlb()
{
	int i=0;
	for (i=0;i<64;i++)
		tlb[i].valid=0;
}

int  hittlb(lnaddr_t addr)
{
	uint32_t tag=addr>>12;
	int i;
	for (i=0;i<64;i++)
		if (tlb[i].tag==tag&&tlb[i].valid)
			return 1;
	return 0;
}

hwaddr_t getpagestart(lnaddr_t addr)
{
	uint32_t tag=addr>>12;
	int i;
	for (i=0;i<64;i++)
		if (tlb[i].tag==tag&&tlb[i].valid)
			return tlb[i].pagestartaddr;
	return 0;
}

void updatetlb(lnaddr_t addr,hwaddr_t pagestart)
{
	int i;
	uint32_t tag=addr>>12;
	for (i=0;i<64;i++)
		if (tlb[i].valid==0)
		{
			tlb[i].valid=1;
			tlb[i].pagestartaddr=pagestart;
			tlb[i].tag=tag;
			return ;
		}
	srand(time(0)+clock());
	int ran=rand()%64;
	tlb[ran].valid=1;
	tlb[ran].pagestartaddr=pagestart;
	tlb[ran].tag=tag;
	return;
}
