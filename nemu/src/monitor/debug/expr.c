#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ,NUM,AND,OR,NUM1,NE,N,REG,DEREF,NEG,VAL

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {
 
	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	{"0x[0-9A-Fa-f]+",NUM1},              //hex
	{"[0-9]+",NUM},                 	//number
	{"[0-9a-zA-z_]+",VAL},          //variable
	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"-",'-'},                      // minus
	{"\\*",'*'},                    //multiple
	{"/",'/'},                      //div 
	{"\\(",'('},	                //left-bracket
    {"\\)",')'},	                //right-bracket
	{"==", EQ},						// equal
	{"&&",AND},                     //and
	{"\\|\\|",OR},                       //or
	{"!=",NE},                         //not equal
	{"!",N},                          //not
	{"\\$\\w{2,3}",REG},                  //register
	{"*",DEREF},                       //pointer
	{"-",NEG}        //NEG

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
		//		char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

			//	Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
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
					case AND:
					case OR:
					case NE:	
					case N:
					   	tokens[nr_token].type=rules[i].token_type;
					   	break;
					case NUM:
					case NUM1:
					case REG:
					case VAL:
                        tokens[nr_token].type=rules[i].token_type;
						int i1;
						if (pmatch.rm_eo-pmatch.rm_so>30) assert(0);
						for (i1=pmatch.rm_so;i1<pmatch.rm_eo;i1++)
                        tokens[nr_token].str[i1-pmatch.rm_so]=
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
	// printf("check %d %d\n",p,q);
	int head=p,rear=q;
	int pan1=0;
	int n1=0,n2=0;
	while (head<=rear)
	{
		if (tokens[head].type=='(') n1++;
		if (tokens[head].type==')') n2++;
		if  (n2>n1)  return -1;
		if (n2==n1&&pan1==0) pan1=head;
		head++;
	}
	head--;
	if (n1!=n2) return -1;
    if (tokens[p].type=='('&&tokens[q].type==')'&&pan1==q)  return 1;
	    else  return 0; 	
}


int finddop(int p,int q)
{   
//	printf("finddop %d %d\n",p,q);
	int head=p,rear=q;
	int statusprior=22;
	int status=0;
	while (head<=rear)
	{
		if (tokens[head].type==VAL||tokens[head].type==NUM||tokens[head].type==NUM1||tokens[head].type==N||tokens[head].type==DEREF||tokens[head].type==REG||tokens[head].type==NEG) {
			head++;
		}
		else if (tokens[head].type=='(') 
		{    
			int n1,n2=0;
			n1=1;
			head++;
			while (n1!=n2&&head<=q)
			{
				if (tokens[head].type=='(') n1++;
				if (tokens[head].type==')') n2++;
				if (n2>n1)   assert(0); 
                head++;
		    }
			if (head>q) return status;
		}
		else if (tokens[head].type=='-'||tokens[head].type=='+')
		{	
			if (statusprior>=18)
			{	      
				statusprior=18;
				status=head;
			}
			head++;
		}
		else if (tokens[head].type=='*'||tokens[head].type=='/')
		{   
			if (statusprior>=20)
			{
				statusprior=20;
				status=head;
			}
			head++;
		}
		else if (tokens[head].type==EQ||tokens[head].type==NE)
		{
			if (statusprior>=16)
			{
				statusprior=16;
				status=head;
			}
			head++;
			
		}
		else if (tokens[head].type==AND)
		{
			if (statusprior>=14)
			{
				statusprior=14;
				status=head;
			}
			head++;
		}
		else if (tokens[head].type==OR)
		{
			if (statusprior>=12)
			{
				statusprior=12;
				status=head;
			}
			head++;
		}
	}
	return status;
}


uint32_t  eval(int p,int q)
{   
//	printf("eval %d %d\n",p,q);
	if (p>q) 
		assert(0);
	else if (p==q) {
//		printf("p=q");
	   int i1;
	   uint32_t num=0;
	   if (tokens[p].type==NUM)
	   {
		   for (i1=0;tokens[p].str[i1]>='0'&&tokens[p].str[i1]<='9';i1++)
		   num=num*10+(tokens[p].str[i1]-'0');
	       return num;	
	   }
	   else if (tokens[p].type==NUM1) 
	   {
	       for (i1=2;(tokens[p].str[i1]>='0'&&tokens[p].str[i1]<='9')||(tokens[p].str[i1]>='A'&&tokens[p].str[i1]<='F')||(tokens[p].str[i1]>='a'&&tokens[p].str[i1]<='f');i1++)
		   if (tokens[p].str[i1]<='9')
		       num=num*16+(tokens[p].str[i1]-'0');
		   else if (tokens[p].str[i1]>='a')
		       num=num*16+(tokens[p].str[i1]-'a'+10);
		   else 
		       num=num*16+(tokens[p].str[i1]-'A'+10);
	       return num; 	   
	   }
       else if (tokens[p].type==REG)
	   {
		   int i;
		   uint32_t num=0;
           for (i=0;i<8;i++)
		   {
			   if (tokens[p].str[1]==regsl[i][0] &&tokens[p].str[2]==
			   regsl[i][1]&&tokens[p].str[3]==regsl[i][2])
			   {
				   num=reg_l(i);
				   return num;
			   }
			   else if (tokens[p].str[1]==regsw[i][0] &&tokens[p].str
				  [2]==regsw[i][1])
			   {
				   num=reg_w(i);
				   return num;
			   }
               else if (tokens[p].str[1]==regsb[i][0]&&tokens[p].str[2]==
					   regsb[i][1])
			   {
				   num=reg_b(i);
			       return num;
			   }
			   else if (tokens[p].str[1]=='e'&&tokens[p].str[2]=='i'&&tokens[p].str[3]=='p')
			   {
			       num=cpu.eip;
				   return num;
			   }
	       }
		   return 0;
	   }
		else 
		{
			for (i1=0;tokens[p].str[i1];i1++ )
			{
				printf("%c",tokens[p].str[i1]);	 
			}
			printf("\n");
            return 233;
			
		}	   
	}
	else if (p+1==q&&tokens[p].type==N)
		return !eval(p+1,q);
	else if (p+1==q&&tokens[p].type==DEREF)
		return swaddr_read(eval(p+1,q),4);
	else if (p+1==q&&tokens[p].type==NEG)
		return -eval(p+1,q);
	else if (tokens[p].type==N&&check_parentheses(p+1,q)==1)
		return !eval(p+1,q);
	else if (tokens[p].type==DEREF&&check_parentheses(p+1,q)==1)
		return swaddr_read(eval(p+1,q),4);
	else if (tokens[p].type==NEG&&check_parentheses(p+1,q)==1)
		return -eval(p+1,q);
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
			case AND:return val1&&val2;
			case OR: return val1||val2;
			case NE: return val1!=val2; 		 
			default:assert(0);
		}
	}
}


uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		printf("bug!!!");
		*success = false;
		return 0;
	}
	if (check_parentheses(0,nr_token-1)==-1) 
	{
		*success=false;
		return 0;
	}
	int i1;
	for (i1=0;i1<nr_token;i1++)
	{
		if (tokens[i1].type=='*'&&(i1==0||tokens[i1-1].type=='+'||tokens[i1-1].type=='/'||tokens[i1-1].type=='-'||tokens[i1-1].type=='*'||tokens[i1-1].type==AND||tokens[i1-1].type==OR||tokens[i1-1].type==EQ||tokens[i1-1].type==NE||tokens[i1-1].type=='('))
        tokens[i1].type=DEREF;  
	}
	for (i1=0;i1<nr_token;i1++)
	{
		if (tokens[i1].type=='-'&&(i1==0||tokens[i1-1].type=='+'||tokens[i1-1].type=='/'||tokens[i1-1].type=='-'||tokens[i1-1].type=='*'||tokens[i1-1].type==AND||tokens[i1-1].type==OR||tokens[i1-1].type==EQ||tokens[i1-1].type==NE||tokens[i1-1].type=='('))
		tokens[i1].type=NEG;
	}
//			printf("%d\n",nr_token);
//	for ( i1=0;i1<nr_token;i1++)
//	{	printf("%d:%d\n",i1,tokens[i1].type);
 //   }
	 uint32_t x;
     x=eval(0,nr_token-1);
	 int ik;
	 for (ik=0;ik<nr_token;ik++)
	 {	
		 int jk;
		 for (jk=0;jk<32;jk++)
		 tokens[ik].str[jk]='\0';
	 }
	 return x;


	/* TODO: Insert codes to evaluate the expression. */
	panic("please implement me");
	return 0;
}

