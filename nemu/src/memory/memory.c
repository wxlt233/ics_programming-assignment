#include "common.h"
#include "nemu.h"
 
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
int hittlb(lnaddr_t addr);
void updatetlb(lnaddr_t addr,hwaddr_t pagestart);
hwaddr_t getpagestart(lnaddr_t addr);
int is_mmio(hwaddr_t addr);
uint32_t mmio_read(hwaddr_t addr,size_t len,int map_NO);
void mmio_write(hwaddr_t addr,size_t len,uint32_t data,int map_NO);
/* Memory accessing interfaces */
 
uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	int map_no=is_mmio(addr);
	if (map_no!=-1) return mmio_read(addr,len,map_no);
	else 
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}
/*uint32_t hwaddr_read(hwaddr_t addr,size_t len)
{
	int map_no=is_mmio(addr);
	if (map_no!=-1) return mmio_read(addr,len,map_no);
	else {
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
}*/


void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int map_no=is_mmio(addr);
	if (map_no!=-1) mmio_write(addr,len,data,map_no);
else 
	dram_write(addr, len, data);
}

/*void hwaddr_write(hwaddr_t addr,size_t len,uint32_t data)
{
	int map_no=is_mmio(addr);
	if (map_no!=-1) mmio_write(addr,len,data,map_no);
	else
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
}*/
hwaddr_t  page_translate(lnaddr_t addr)
{
	if (cpu.cr0.protect_enable==1&&cpu.cr0.paging==1)
	{
		//if (cpu.eip==0x8054655) printf("%x\n",cpu.eax);
		if (hittlb(addr))
		{
			uint16_t offset=addr&0xfff;
			hwaddr_t pagestart=getpagestart(addr);
			return (pagestart<<12)+offset;
		}
		else{
			uint16_t offset=addr&0xfff;
			uint16_t dir=(addr>>22)&0x3ff;
			uint16_t page=(addr>>12)&0x3ff;

			PDE aa;
			aa.val=hwaddr_read((cpu.cr3.page_directory_base<<12)+4*dir,4);
			if (aa.present==0)
			{
				assert(aa.present);
			}
	//		uint32_t pagetableaddr=hwaddr_read(((cpu.cr3.page_directory_base<<12)+4*dir),4)>>12;
	//		uint32_t pagestartaddr=(hwaddr_read(((pagetableaddr<<12)+4*page),4)>>12)<<12;
			PTE a;
			a.val=hwaddr_read((aa.page_frame<<12)+4*page,4);
			
	//		a.val=hwaddr_read((pagetableaddr<<12)+4*page,4);
			assert(a.present);
	//		updatetlb(addr,pagestartaddr>>12);
	//			return pagestartaddr+offset;
			updatetlb(addr,a.page_frame);
			return (a.page_frame<<12)+offset;
		}
	}
	else return (hwaddr_t) addr;
}
/*uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}*/

uint32_t lnaddr_read(lnaddr_t addr,size_t len){
	if ((addr&0xfff)+len>4096)
	{
	//	assert(0);
		int newlen=(addr&0xfff)+len-4096;
		uint32_t newdata=lnaddr_read((((addr+4096)>>12)<<12),newlen);
		uint32_t olddata=lnaddr_read(addr,len-newlen);
		return (newdata<<8*(len-newlen))+olddata;
	}
	else 
 	{ 
		hwaddr_t hwaddr=page_translate(addr);
		return hwaddr_read(hwaddr,len);
	}
}
void lnaddr_write(lnaddr_t addr,size_t len ,uint32_t data){
	if ((addr&0xfff)+len>4096)
 	{
		int newlen=(addr&0xfff)+len-4096;
		uint32_t  olddata=data&((1<<(8*(len-newlen)))-1);
		uint32_t newdata=data>>(8*(len-newlen));
		lnaddr_write((((addr+4096)>>12)<<12),newlen,newdata);
		lnaddr_write(addr,len-newlen,olddata);
	}
	else 
	{
		hwaddr_t hwaddr=page_translate(addr);
	//	return 
		hwaddr_write(hwaddr,len,data);
	 }

}


lnaddr_t seg_translate(swaddr_t addr,size_t len,uint8_t sreg)
{
	if (cpu.cr0.protect_enable==0) return addr;
	else 
	{
//		lnaddr_t lnaddr=cpu.DESC[sreg].base_15_0+(cpu.DESC[sreg].base_23_16<<16)+(cpu.DESC[sreg].base_31_24<<24)+addr;
	//	return lnaddr;
		return addr;
	}
}
/*uint32_t swaddr_read(swaddr_t addr, size_t len) {
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
*/
uint32_t swaddr_read(swaddr_t addr, size_t len,uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr=seg_translate(addr,len,sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data,uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr=seg_translate(addr,len,sreg);
	lnaddr_write(lnaddr, len, data);
}

