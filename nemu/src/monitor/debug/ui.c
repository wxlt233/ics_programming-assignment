#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"


#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args)
{   
	char *arg = strtok(NULL, " ");
	if(arg == NULL)  cpu_exec(1);
	else { int n=0;
		char *s;
		s=arg;
		while (*s)
		{   n=n*10+(*s-'0');
			s++;
		}
		 cpu_exec(n);
	    }
	
	return 0;
}

static int cmd_x(char *args)
{ char *arg1=strtok(NULL," ");
//  char *arg3=strtok(NULL," ");
  char *arg2=arg1+strlen(arg1)+1;
   size_t n=0;
   char *s=arg1;
    while (*s)
    {   n=n*10+(*s-'0');
			s++;
	}
   uint32_t add=0;
   s=arg2;
   bool pan=true;
   printf("%s:       ",arg2);
   add=expr(arg2,&pan);
   if (!pan) 
   {
	   printf("wrong input!\n");return 0;
   }
   printf("%u           ",add);
   int i1;
   for (i1=0;i1<n;i1++)
   printf("0x%08x ",hwaddr_read(add+4*i1,4));   
   printf("\n"); 
   return 0;
}

static int cmd_info(char *args)
{   if (args[0]=='r')
	  {   int i;
		  for (i=0;i<4;i++)
		  printf("%s=0x%08x  %s=0x%04x  %s=0x%02x %s=0x%02x\n",
				  regsl[i],cpu.gpr[i]._32,
				  regsw[i],cpu.gpr[i]._16,
				  regsb[i],cpu.gpr[i]._8[0],
				  regsb[i+4],cpu.gpr[i]._8[1]);

		  for (i=4;i<8;i++)
		  printf("%s=0x%08x  %s=0x%04x\n",
				  regsl[i],cpu.gpr[i]._32,
				  regsw[i],cpu.gpr[i]._16);
		  printf("EIP=0x%x\n",cpu.eip);
	 }	    
	 
	 return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
    {"si","Execute instruction for n times",cmd_si},
	{"info","print the state of program",cmd_info},
	{"x","scan the memory",cmd_x}
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
