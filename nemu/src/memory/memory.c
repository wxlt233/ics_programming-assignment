#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

bool checkcache1(hwaddr_t addr);
bool checkcache2(hwaddr_t addr);
void read_cache2tocache1(hwaddr_t addr);
void read_dramtocache2(hwaddr_t addr);
uint32_t read_cache1_hit(hwaddr_t addr,size_t len);
uint32_t read_cache2_hit(hwaddr_t addr,size_t len);
void write_cache1_hit(hwaddr_t addr,size_t len,uint32_t data);
void write_cache2_hit(hwaddr_t addr,size_t len,uint32_t data);
void write_allocate(hwaddr_t addr,size_t len,uint32_t data);

/* Memory accessing interfaces */

/*uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}*/

uint32_t hwaddr_read(hwaddr_t addr,size_t len)
{
	if (checkcache1(addr))
	{
		return read_cache1_hit(addr,len);
	}
	else if (checkcache2(addr))
	{  
		read_cache2tocache1(addr);
		return read_cache2_hit(addr,len);
	}
	else 
	{
		read_dramtocache2(addr);
		read_cache2tocache1(addr);
		return read_cache1_hit(addr,len);		
	}
}


/*void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	dram_write(addr, len, data);
}*/

void hwaddr_write(hwaddr_t addr,size_t len,uint32_t data)
{
	if (checkcache1(addr))
	{
		write_cache1_hit(addr,len,data);
	}
	else if (checkcache2(addr))
	{
		write_cache2_hit(addr,len,data);
	}
	else write_allocate(addr,len,data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

