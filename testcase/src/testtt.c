#include <trap.h>
int func(int n)
{
	if (n<=2) return 1;
	else return (func(n-1)+func(n-2));
}
int main()
{
	int a=0;
	a=func(5);
	nemu_assert(a== 5);

	HIT_GOOD_TRAP;
	return 0;	
}
