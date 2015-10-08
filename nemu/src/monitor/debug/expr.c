#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ,NUM

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"-",'-'},                      // minus
	{"\\*",'*'},                    //multiple
	{"/",'/'},                      //div 
	{"\\(",'('},	                //left-bracket
    {"\\)",')'},	                //right-bracket
	{"[0-9]+",NUM},                 	//number
	{"==", EQ}						// equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

            

				switch(rules[i].token_type) {
					case NOTYPE:
						nr_token--;
						break;
					case '+':
				    case '-':
					case '*':
				    case '/':
				    case '(':
					case ')':
					case EQ :	
					   	tokens[nr_token].type=rules[i].token_type;
					   	break;
					case NUM:
                        tokens[nr_token].type=rules[i].token_type;
						int i1;
						if (pmatch.rm_eo-pmatch.rm_so>30) assert(0);
						for (i1=pmatch.rm_so;i1<=pmatch.rm_eo;i1++)
                        tokens[nr_token].str[i-pmatch.rm_so]=
						e[position-substr_len+i1];
                        break;
					default: panic("please implement me");
				}
                nr_token++;
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	return true; 
}



int check_parentheses(int p,int q)
{
	int head=p,rear=q;
	int n1=0,n2=0;
	while (head<=rear)
	{
		if (tokens[head].type=='(') n1++;
		if (tokens[head].type==')') n2++;
		if  (n2>n1)  return -1;
		head++;
	}
	head--;
	if (n1!=n2) return -1;
    if (tokens[p].type=='('&&tokens[q].type==')')  return 1;
	    else  return 0; 	
}


int finddop(int p,int q)
{   printf("jaja");
	int head=p,rear=q;
	int statusprior=3;
	int status=0;
	while (head<=rear)
	{
		if (tokens[head].type==NUM) {
			head++;
		}
		else if (tokens[head].type=='(') 
		{    
			int n1,n2=0;
			n1=1;
			while (n1!=n2)
			{
				if (tokens[head].type=='(') n1++;
				if (tokens[head].type==')') n2++;
				if (n2>n1)   assert(0); 
                head++;
		    }
		}
		else if (tokens[head].type=='-'||tokens[head].type=='+')
		{      
			statusprior=1;
			status=head;
			head++;
		}
		else if (tokens[head].type=='*'||tokens[head].type=='/')
		{   
			if (statusprior>=2)
			{
				statusprior=2;
				status=head;
				head++;
			}
		}
	}
	return status;
}


uint32_t  eval(int p,int q)
{   printf("jjj");
	if (p>q) 
		assert(0);
	else if (p==q) {
		printf("haha");
	   int i1,num=0;
       for (i1=0;tokens[p].str[i1]>='0'&&tokens[p].str[i1]<='9';i1++)
		   num=num*10+(tokens[p].str[i1]-'0');
	    return num;	
	}
	else if (check_parentheses(p,q)==1)
		return eval(p+1,q-1);
	else {
		int op=finddop(p,q);
		int val1,val2;
		val1=eval(p,op-1);
		val2=eval(op+1,q);
        switch (tokens[op].type){
			case '+':return val1+val2;
			case '-':return val1-val2;
			case '*':return val1*val2;
			case '/':return val1/val2;
			case EQ :return val1==val2;
			default:assert(0);
		}
	}
}


uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	if (check_parentheses(0,nr_token-1)==-1) 
	{
		*success=false;
		return 0;
	}
	int i1;
	for ( i1=0;i1<nr_token;i1++)
		printf("%d\n",tokens[i1].type);
	*success=true;
		//	return 100000;
	return  eval(0,nr_token-1);


	/* TODO: Insert codes to evaluate the expression. */
	panic("please implement me");
	return 0;
}

