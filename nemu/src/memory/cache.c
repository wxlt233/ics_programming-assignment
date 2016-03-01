#include "common.h"
#include<time.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct 
{
	uint8_t offset[64];
	uint8_t valid;
	uint32_t tag;
}cache1block;

cache1block cache1[128][8];

typedef struct 
{
	uint8_t offset[64];
	uint8_t valid;
	uint8_t dirty;
	uint32_t tag;
}cache2block;
cache2block cache2[4096][16];

uint32_t dram_read(hwaddr_t addr,size_t len);
uint32_t dram_write(hwaddr_t addr,size_t len,uint32_t data);
uint32_t hwaddr_read(hwaddr_t addr,size_t len);

void initcache()
{
	int i,j;
	for (i=0;i<128;i++)
	  for (j=0;j<8;j++)
		  cache1[i][j].valid=0;
	for (i=0;i<4096;i++)
	  for (j=0;j<16;j++)
		  cache2[i][j].valid=0;
}

bool checkcache1(hwaddr_t addr)
{
  uint32_t tag1;
  uint32_t cacheno;
  tag1=addr>>13;
  cacheno=(addr>>6)&0x7f;
  int i;
  for (i=0;i<8;i++)
	 if (cache1[cacheno][i].tag==tag1&&cache1[cacheno][i].valid)
		 return 1;
  return 0;
}

bool checkcache2(hwaddr_t addr)
{
	uint32_t tag2;
	uint32_t cacheno;
	tag2=addr>>18;
	cacheno=(addr>>6)&0xfff;
	int i;
	for (i=0;i<16;i++)
		if (cache2[cacheno][i].tag==tag2&&cache2[cacheno][i].valid)
			return 1;
	return 0;
}


void read_cache2tocache1(hwaddr_t addr)
{
	uint32_t tag1,tag2,cache1no,cache2no;
	tag1=addr>>13;
	tag2=addr>>18;
	cache1no=(addr>>6)&0x7f;
	cache2no=(addr>>6)&0xfff;
	int i;
	for (i=0;i<16;i++)
		if (cache2[cache2no][i].valid==1&&cache2[cache2no][i].tag==tag2)
			break;
	int j;
	srand(time(NULL));
	int ran=rand()%8;
	for (j=0;j<64;j++)
		cache1[cache1no][ran].offset[j]=cache2[cache2no][i].offset[j];
	cache1[cache1no][ran].valid=1;
	cache1[cache1no][ran].tag=tag1;
}

void read_dramtocache2(hwaddr_t addr)
{
	uint32_t tag2=addr>>18;
	uint32_t cache2no=(addr>>6)&0xfff;
	srand(time(NULL));
	int ran=rand()%16;
	int i;
	if (cache2[cache2no][ran].valid&&cache2[cache2no][ran].dirty)
	{
		uint32_t writebackaddr=(cache2[cache2no][ran].tag<<18)+(cache2no<<6);
		for (i=0;i<64;i++)
			dram_write(writebackaddr+i,1,cache2[cache2no][ran].offset[i]);
	}
	uint32_t beginaddr=addr&(~(0x3f));
	cache2[cache2no][ran].dirty=0;
	cache2[cache2no][ran].valid=1;
	cache2[cache2no][ran].tag=tag2;
	for (i=0;i<64;i++)
		cache2[cache2no][ran].offset[i]=dram_read(beginaddr+i,1);

}
uint32_t read_cache1_hit(hwaddr_t addr,size_t len)
{
	uint32_t tag1=addr>>13;
	uint32_t cache1no=(addr>>6)&0x7f;
	int i;
	for (i=0;i<8;i++)
	{
		if (cache1[cache1no][i].valid&&cache1[cache1no][i].tag==tag1) break;
	}
	uint32_t offset=addr&0x3f;
	uint32_t data=0;
	if (offset+len-1<64)
	{
		int j;
		for (j=0;j<len;j++)
			data+=(cache1[cache1no][i].offset[offset+j])<<(8*j);
		return data;
	}
	else 
	{
		int j;
		for (j=offset;j<64;j++)
			data+=(cache1[cache1no][i].offset[j])<<(8*(j-offset));
		uint32_t nextblockaddr=((addr+64)>>6)<<6;
		uint32_t t=hwaddr_read(nextblockaddr,len-(64-offset));
		data+=t<<(8*(64-offset));
		return data;
	}
}

uint32_t read_cache2_hit(hwaddr_t addr,size_t len)
{
	uint32_t tag2=addr>>18;
	uint32_t cache2no=(addr>>6)&0xfff;
	int i;
	for (i=0;i<16;i++)
	{
		if (cache2[cache2no][i].valid&&cache2[cache2no][i].tag==tag2)
			break;
	}
	uint32_t offset=addr&0x3f;
	uint32_t data=0;
	if (offset+len-1<64)
	{
		int j;
		for (j=0;j<len;j++)
			data+=(cache2[cache2no][i].offset[offset+j]<<(8*j));
		return data;
	}
	else 
	{
		int j;
		for (j=offset;j<64;j++)
			data+=(cache2[cache2no][i].offset[j]<<(8*(j-offset)));
		uint32_t nextblockaddr=((addr+64)>>6)<<6;
		uint32_t t=hwaddr_read(nextblockaddr,len-(64-offset));
		data+=t<<(8*(64-offset));
		return data;
	}
}




