#include "common.h"
#include <stdlib.h>
#include <elf.h>

char *exec_file = NULL;

static char *strtab = NULL;
static Elf32_Sym *symtab = NULL;
static int nr_symtab_entry;

uint32_t findadd(char *destvar)
{
	int ii;
	char c[30];
	char *c1=c;
	for (ii=0;ii<nr_symtab_entry;ii++)
	{
		int jj=0;
		while (*(jj+strtab+symtab[ii].st_name))
		{
//			printf("%c",*(jj+strtab+symtab[ii].st_name));
			c[jj]=*(jj+strtab+symtab[ii].st_name);
		    jj++;
		}
		c[jj]='\0';
		if (ELF32_ST_TYPE(symtab[ii].st_info)==STT_OBJECT&&strcmp(destvar,c1)==0) return (symtab[ii].st_value);
	}
	printf("no such var\n");
//	asseert("no value");
    return 233;
}

void findfunc(uint32_t  addrt)
{
    int ii=0;
	for (ii=0;ii<nr_symtab_entry;ii++)
	{
		if ((ELF32_ST_TYPE(symtab[ii].st_info)==STT_FUNC)&&symtab[ii].st_value<=addrt&&symtab[ii].st_value+symtab[ii].st_size>=addrt)
		{
			char c[30];
			char *c1=c;
			int jj=0;
			while (*(jj+strtab+symtab[ii].st_name))
			{	
//				printf("%c",*(jj+strtab+symtab[ii].st_name));
				c[jj]=*(jj+strtab+symtab[ii].st_name);
				jj++;
			}
			c[jj]='\0';
			printf("%s\n",c1);
			break;
		}
	}
}

void load_elf_tables(int argc, char *argv[]) {
	int ret;
	Assert(argc == 2, "run NEMU with format 'nemu [program]'");
	exec_file = argv[1];

	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	uint8_t buf[4096];
	/* Read the first 4096 bytes from the exec_file.
	 * They should contain the ELF header and program headers. */
	ret = fread(buf, 4096, 1, fp);
	assert(ret == 1);

	/* The first several bytes contain the ELF header. */
	Elf32_Ehdr *elf = (void *)buf;
	char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	/* Check ELF header */
	assert(memcmp(elf->e_ident, magic, 4) == 0);		// magic number
	assert(elf->e_ident[EI_CLASS] == ELFCLASS32);		// 32-bit architecture
	assert(elf->e_ident[EI_DATA] == ELFDATA2LSB);		// littel-endian
	assert(elf->e_ident[EI_VERSION] == EV_CURRENT);		// current version
	assert(elf->e_ident[EI_OSABI] == ELFOSABI_SYSV || 	// UNIX System V ABI
			elf->e_ident[EI_OSABI] == ELFOSABI_LINUX); 	// UNIX - GNU
	assert(elf->e_ident[EI_ABIVERSION] == 0);			// should be 0
	assert(elf->e_type == ET_EXEC);						// executable file
	assert(elf->e_machine == EM_386);					// Intel 80386 architecture
	assert(elf->e_version == EV_CURRENT);				// current version


	/* Load symbol table and string table for future use */

	/* Load section header table */
	uint32_t sh_size = elf->e_shentsize * elf->e_shnum;
	Elf32_Shdr *sh = malloc(sh_size);
	fseek(fp, elf->e_shoff, SEEK_SET);               //the offset of section header :elf->e_shoff
	ret = fread(sh, sh_size, 1, fp);           //sh section header
	assert(ret == 1);

	/* Load section header string table */
	char *shstrtab = malloc(sh[elf->e_shstrndx].sh_size);
	fseek(fp, sh[elf->e_shstrndx].sh_offset, SEEK_SET);
	ret = fread(shstrtab, sh[elf->e_shstrndx].sh_size, 1, fp);
	assert(ret == 1);
//	printf("%x\n",sh[elf->e_shstrndx].sh_offset);

	int i;
	for(i = 0; i < elf->e_shnum; i ++) {
		if(sh[i].sh_type == SHT_SYMTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".symtab") == 0) {
			/* Load symbol table from exec_file */                         //find which of the section if symtab and strtab
			symtab = malloc(sh[i].sh_size);
//			printf("%x\n",sh[i].sh_offset);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(symtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
			nr_symtab_entry = sh[i].sh_size / sizeof(symtab[0]);
		}
		else if(sh[i].sh_type == SHT_STRTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".strtab") == 0) {
			/* Load string table from exec_file */
			strtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(strtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
		}
	}
/*printf("%x\n",(unsigned)strtab);
	int ii;
	for (ii=0;ii<nr_symtab_entry;ii++)
	{
		int jj=0;
		//printf("%x\n",symtab[ii].st_name);
		while (*(jj+strtab+symtab[ii].st_name))
		{
			printf("%c",*(jj+strtab+symtab[ii].st_name));
		    jj++;	
		}
		printf("info: %c",symtab[ii].st_info);
		printf("\n");
	}
*/
	free(sh);
	free(shstrtab);

	assert(strtab != NULL && symtab != NULL);

	fclose(fp);
}

