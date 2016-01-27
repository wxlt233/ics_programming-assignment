#include<stdio.h>
void foo()
{
	int arr[4];
	int i;
	for (i=0;i<=4;i++)
	{	arr[i]-=4;
//	printf("%d\n",i);
	}
}
int main()
{
	foo();
	return 0;
}
