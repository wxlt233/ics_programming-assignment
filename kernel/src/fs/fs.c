#include "common.h"

typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
static const file_info file_table[] = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

typedef struct {
	bool opened;
	uint32_t offset;
}Fstate;

Fstate filestate[NR_FILES+3];


void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);

bool testpath(const char *s1,char *s2)
{
	while (*s1)
	{
		if (*s1!=*s2)
		return false;
		s1++;
		s2++;
	}
	if (*s2) return false;
	return true;
}


int fs_open(const char *pathname,int flags)
{
	int i=0;
	for (i=0;i<NR_FILES;i++)
	if (testpath(pathname,file_table[i].name))
	{
		filestate[i+3].opened=true;
		filestate[i+3].offset=0;
		return i+3;
	}
	assert(0);
	return -1;
}

int fs_read(int fd,void *buf,int len)
{
	if (filestate[fd].opened==false)
	{
		return -1;
	}	
	if (filestate[fd].offset+len>file_table[fd-3].size)
		len=file_table[fd-3].size-filestate[fd].offset;
	ide_read(buf,file_table[fd-3].disk_offset+filestate[fd].offset,len);
	filestate[fd].offset+=len;
	return len;
}

int fs_write(int fd,void *buf,int len)
{
	if (filestate[fd].opened==false)
	{
		return -1;
	}	
	if (filestate[fd].offset+len>file_table[fd-3].size)
		len=file_table[fd-3].size-filestate[fd].offset;
	ide_write(buf,file_table[fd-3].disk_offset+filestate[fd].offset,len);
	filestate[fd].offset+=len;
	return len;
}

int fs_lseek(int fd,int offset,int whence)
{
	if (!filestate[fd].opened)
		return -1;
	if (whence==SEEK_SET)
		filestate[fd].offset=offset;
	else if (whence==SEEK_CUR)
		filestate[fd].offset+=offset;
	else 
		filestate[fd].offset=file_table[fd-3].size+offset;
	
	return filestate[fd].offset;

}
int fs_close(int fd)
{
	if (!filestate[fd].opened)
		return -1;
	filestate[fd].opened=false;
	return 0;
}
/* TODO: implement a simplified file system here. */

