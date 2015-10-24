#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */
WP * new_wp()
{
	WP *t;
	if (free_==NULL)
		assert(0);
	else 
	{
		t=free_;
	    free_=free_->next;
		t->next=NULL;	
	}
	return t;
}

void free_wp(WP *wp)
{
	WP *te=free_;
	while (te->next!=NULL)
	{
		te=te->next;
	}
	te->next=wp;
	wp->next=NULL;
}

void printwatchpoint()
{
	WP *t=head;
	while (t!=NULL)
	{
		printf("watchpoint %d : %s  value %u\n",t->NO,t->wexpr,t->oldvalue);
		t=t->next;
	}
	return;
}

void deletewatchpoint(int n)
{
	WP *t=head,*t1=head;
	int pan=0;
	while (t!=NULL)
	{
		if (t->NO==n)
		{
			if (t==head)
				head=head->next;
			else 
			{
				t1->next=t->next;
			}
			free_wp(t);
			pan=1;
			break;
		}
		t1=t;
		t=t->next;
	}
	if (!pan) printf("there is no watchpoint %d\n",n);
	else printf("watchpoint %d deleted\n",n);
	return ;
}

void setwatchpoint(char *args)
{
	WP *tt=new_wp();
	int i=0;
	while (*args)
		tt->wexprc[i++]=*args++;
	tt->wexpr=tt->wexprc;
	bool su=1;
	tt->oldvalue=expr(tt->wexpr,&su);
	tt->newvalue=tt->oldvalue;
	if (head==NULL) head=tt;
	else 
	{
		WP *te1=head;
		while (te1->next!=NULL)
			te1=te1->next;
		te1->next=tt;
	}	
	return;
}

bool calwatchpoint()
{
	WP *t=head;
	bool succ=1;
	bool change=0;
	while (t!=NULL)
	{
		t->oldvalue=t->newvalue;
		t->newvalue=expr(t->wexpr,&succ);
		if (t->oldvalue!=t->newvalue) 
		{
			change=1;
		}
		t=t->next;
	}
	return change;
}


void printchangedwp()
{
	WP *t=head;
	while (t!=NULL)
	{
		if (t->newvalue!=t->oldvalue)
		{
			printf("watchpoint %d changed ! old value :%u new value :%u\n",t->NO,t->oldvalue,t->newvalue);
			t=t->next;
		}
	}
	return;
}
