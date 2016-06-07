#include "common.h"
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define BlockSize 64
#define bitofblock 6

#define maskofblock 0x3f
#define maskofzushu1 0x7f
#define maskofzushu2 0xfff

#define cache1size 65536
#define cache2size (4*1024*1024)

#define cache1way 8
#define cache2way 16

#define Zushu1 (cache1size/BlockSize/cache1way)  
#define Zushu2 (cache2size/BlockSize/cache2way)  

#define bitofzushu1 ((int)(round(log(Zushu1)/log(2))))
#define bitofzushu2 ((int) (round(log(Zushu2)/log(2))))
 
#define gettag1 (bitofblock+bitofzushu1)
#define gettag2 (bitofblock+bitofzushu2)

typedef struct 
{
	uint8_t offset[BlockSize];
	uint8_t valid;
	uint32_t tag;
}cache1block;

cache1block cache1[Zushu1][cache1way];

typedef struct 
{
	uint8_t offset[BlockSize];
	uint8_t valid;
	uint8_t dirty;
	uint32_t tag;
}cache2block;

cache2block cache2[Zushu2][cache2way];

uint32_t dram_read(hwaddr_t addr,size_t len);
uint32_t dram_write(hwaddr_t addr,size_t len,uint32_t data);
uint32_t hwaddr_read(hwaddr_t addr,size_t len);
uint32_t hwaddr_write(hwaddr_t addr,size_t len,uint32_t data);
void write_allocate(hwaddr_t addr,size_t len,uint32_t data);

void test_cache(uint32_t addr)
{
	uint32_t  tag1=addr>>gettag1;	
	uint32_t cacheno=(addr>>bitofblock)&maskofzushu1;
	int i;
	for ( i=0;i<cache1way;i++)
		if (cache1[cacheno][i].tag==tag1&&cache1[cacheno][i].valid)
		{
			printf("tag:%x",tag1);
			int j=0;
			for (j=0;j<64;j++)
			{
				if (j%8==0) printf("\n");
				printf("%2x",cache1[cacheno][i].offset[j]);
			}
			printf("\n");
			return ;
		}	
	printf("not int cache!\n");
	


}

void init_cache()
{
	int i,j;
	for (i=0;i<Zushu1;i++)
	  for (j=0;j<cache1way;j++)
		  cache1[i][j].valid=0;
	for (i=0;i<Zushu2;i++)
	  for (j=0;j<cache2way;j++)
 	  {  cache2[i][j].valid=0;
		  cache2[i][j].dirty=0;
	  }
}

bool checkcache1(hwaddr_t addr)
{
  uint32_t tag1;
  uint32_t cacheno;
  tag1=addr>>gettag1;
  cacheno=(addr>>bitofblock)&maskofzushu1;
  int i;
  for (i=0;i<cache1way;i++)
	 if (cache1[cacheno][i].tag==tag1&&cache1[cacheno][i].valid)
		 return 1;
  return 0;
}

bool checkcache2(hwaddr_t addr)
{
	uint32_t tag2;
	uint32_t cacheno;
	tag2=addr>>gettag2;
	cacheno=(addr>>bitofblock)&maskofzushu2;
	int i;
	for (i=0;i<cache2way;i++)
		if (cache2[cacheno][i].tag==tag2&&cache2[cacheno][i].valid)
			return 1;
	return 0;
}


void read_cache2tocache1(hwaddr_t addr)
{
	uint32_t tag1,tag2,cache1no,cache2no;
	tag1=addr>>gettag1;
	tag2=addr>>gettag2;
	cache1no=(addr>>bitofblock)&maskofzushu1;
	cache2no=(addr>>bitofblock)&maskofzushu2;
	int i;
	for (i=0;i<cache2way;i++)
		if (cache2[cache2no][i].valid==1&&cache2[cache2no][i].tag==tag2)
			break;
	int j;
	int cache1full=0;
	for (j=0;j<cache1way;j++)
	 cache1full+=cache1[cache1no][j].valid;
	int ran=0;
	if (cache1full==cache1way)
	{
		srand(time(0)+clock());	 
		ran=rand()%cache1way;
	}
	else 
	{
      for (j=0;j<cache1way;j++)
	  {
		  if (!cache1[cache1no][j].valid) 
		  {
			  ran=j;
			  break;
		  }
	  }
	}
	for (j=0;j<BlockSize;j++)
		cache1[cache1no][ran].offset[j]=cache2[cache2no][i].offset[j];
	cache1[cache1no][ran].valid=1;
	cache1[cache1no][ran].tag=tag1;
}

void read_dramtocache2(hwaddr_t addr)
{
	uint32_t tag2=addr>>(gettag2);
	uint32_t cache2no=(addr>>bitofblock)&maskofzushu2;
	int cache2full=0;
	int j,ran=0;
	for (j=0;j<cache2way;j++)
	{
		cache2full+=cache2[cache2no][j].valid;
	}
	if (cache2full==cache2way)
	{
		srand(time(0)+clock());
	    ran=rand()%cache2way;
	}
	else 
	{
		for (j=0;j<cache2way;j++)
		{
			if (!cache2[cache2no][j].valid)
			{
				ran=j;
				break;
			}
		}
	}
	int i;
	if (cache2[cache2no][ran].valid&&cache2[cache2no][ran].dirty)
 	{
		uint32_t writebackaddr=(cache2[cache2no][ran].tag<<gettag2)+(cache2no<<bitofblock);
		for (i=0;i<BlockSize;i++)
			dram_write(writebackaddr+i,1,cache2[cache2no][ran].offset[i]);
	}
	uint32_t beginaddr=(addr>>bitofblock)<<bitofblock;
	cache2[cache2no][ran].dirty=0;
	cache2[cache2no][ran].valid=1;
	cache2[cache2no][ran].tag=tag2;
	for (i=0;i<BlockSize;i++)
		cache2[cache2no][ran].offset[i]=dram_read(beginaddr+i,1);

}
uint32_t read_cache1_hit(hwaddr_t addr,size_t len)
{
	uint32_t tag1=addr>>gettag1;
	uint32_t cache1no=(addr>>bitofblock)&maskofzushu1;
	int i;
	for (i=0;i<cache1way;i++)
 	{
		if (cache1[cache1no][i].valid&&cache1[cache1no][i].tag==tag1) break;
	}
	uint32_t offset=addr&maskofblock;
	uint32_t data=0;
	if (offset+len-1<BlockSize)
	{
		int j;
		for (j=0;j<len;j++)
			data+=(cache1[cache1no][i].offset[offset+j])<<(8*j);
		return data;
	}
	else 
	{
		int j;
		for (j=offset;j<BlockSize;j++)
			data+=(cache1[cache1no][i].offset[j])<<(8*(j-offset));
		uint32_t nextblockaddr=((addr+BlockSize)>>bitofblock)<<bitofblock;
		uint32_t t=hwaddr_read(nextblockaddr,len-(BlockSize-offset));
		data+=t<<(8*(BlockSize-offset));
		return data;
	}
}

uint32_t read_cache2_hit(hwaddr_t addr,size_t len)
{
	uint32_t tag2=addr>>gettag2;
	uint32_t cache2no=(addr>>bitofblock)&maskofzushu2;
	int i;
	for (i=0;i<cache2way;i++)
	{
		if (cache2[cache2no][i].valid&&cache2[cache2no][i].tag==tag2)
			break;
	}
	uint32_t offset=addr&maskofblock;
	uint32_t data=0;
	if (offset+len-1<BlockSize)
	{
		int j;
		for (j=0;j<len;j++)
			data+=(cache2[cache2no][i].offset[offset+j])<<(8*j);
		return data;
	}
	else 
	{
		int j;
		for (j=offset;j<BlockSize;j++)
			data+=(cache2[cache2no][i].offset[j]<<(8*(j-offset)));
		uint32_t nextblockaddr=((addr+BlockSize)>>bitofblock)<<bitofblock;
		uint32_t t=hwaddr_read(nextblockaddr,len-(BlockSize-offset));
		data+=t<<(8*(BlockSize-offset));
		return data;
	}
}

void write_cache1_hit(hwaddr_t addr,size_t len ,uint32_t data)
{  
	uint32_t tag1=addr>>gettag1;
	uint32_t cache1no=(addr>>bitofblock)&maskofzushu1;
	int i;
	uint32_t datat=data;
	int cache1zunei=0;
	for (i=0;i<cache1way;i++)
		if (cache1[cache1no][i].valid&&cache1[cache1no][i].tag==tag1)
		{
			cache1zunei=i;
			break;
		}
	uint32_t offset=addr&maskofblock;
	int j;
	
	if (offset+len-1<BlockSize)
	{
		for (j=0;j<len;j++)
			cache1[cache1no][cache1zunei].offset[offset+j]=(data>>(8*j))&0xff;
	}
	else 
	{
		for (j=offset;j<BlockSize;j++)
		{
			cache1[cache1no][cache1zunei].offset[j]=data&0xff;
			data=data>>8;
		}
		hwaddr_write(((addr+BlockSize)>>bitofblock)<<bitofblock,len-(BlockSize-offset),data);
	}

	uint32_t tag2=addr>>gettag2;
	uint32_t cache2no=(addr>>bitofblock)&maskofzushu2;
	uint32_t cache2zunei=cache2way;
	for (i=0;i<cache2way;i++)
		if (cache2[cache2no][i].valid&&cache2[cache2no][i].tag==tag2)
		{ 
			cache2zunei=i;
		   	break;
 		}
	if (cache2zunei<cache2way)
	{
		cache2[cache2no][cache2zunei].dirty=1;
		if (offset+len-1<BlockSize)
		{
			for (j=0;j<len;j++)
			cache2[cache2no][cache2zunei].offset[offset+j]=cache1[cache1no][cache1zunei].offset[offset+j];
		}
		else 
		{
			for (j=offset;j<BlockSize;j++)
			cache2[cache2no][cache2zunei].offset[j]=cache1[cache1no][cache1zunei].offset[j];
		}
	}
	else 
	{
		write_allocate(addr,len,datat);
	}
}


void write_cache2_hit(hwaddr_t addr,size_t len,uint32_t data)
{
	uint32_t tag2=addr>>gettag2;
	uint32_t cache2no=(addr>>bitofblock)&maskofzushu2;
	int i;
	for (i=0;i<cache2way;i++)
		if (cache2[cache2no][i].valid&&cache2[cache2no][i].tag==tag2)
			break;
	cache2[cache2no][i].dirty=1;
	uint32_t offset=addr&maskofblock;
	int j;
	if (offset+len-1<BlockSize)
	{
		for (j=0;j<len;j++)
		{
			cache2[cache2no][i].offset[j+offset]=data&0xff;
			data=data>>8;
		}
	}
	else 
	{
		for (j=offset;j<BlockSize;j++)
		{
			cache2[cache2no][i].offset[j]=data&0xff;
			data=data>>8;
		}
		hwaddr_write(((addr+BlockSize)>>bitofblock)<<bitofblock,len-(BlockSize-offset),data);
	}
}

void write_allocate(hwaddr_t addr,size_t len,uint32_t data)
{
	uint32_t tag2=addr>>gettag2;
	uint32_t cache2no=(addr>>bitofblock)&maskofzushu2;
	dram_write(addr,len,data);
	int cache2full=0;
	int i=0,j;
	for (j=0;j<cache2way;j++)
	{
		cache2full+=cache2[cache2no][j].valid;
	}
	if (cache2full==cache2way)
	{
		srand(time(0)+clock());
		i=rand()%cache2way;
	}
	else 
	{
		for (j=0;j<cache2way;j++)
		{
			if (!cache2[cache2no][j].valid)
			{
				i=j;
				break;
			}
		}
	}
	if (cache2[cache2no][i].valid&&cache2[cache2no][i].dirty)
	{
		uint32_t writebackaddr=(cache2[cache2no][i].tag<<gettag2)+(cache2no<<bitofblock);
		for (j=0;j<BlockSize;j++)
			dram_write(writebackaddr+j,1,cache2[cache2no][i].offset[j]);
	}
	cache2[cache2no][i].valid=1;
	cache2[cache2no][i].dirty=0;
	cache2[cache2no][i].tag=tag2;
	uint32_t readbeginaddr=(addr>>bitofblock)<<bitofblock;
	for (j=0;j<BlockSize;j++)
		cache2[cache2no][i].offset[j]=dram_read(readbeginaddr+j,1);
}


