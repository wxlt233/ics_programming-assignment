#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	long long t=((long long)a* (long long )b)/(1<<16);
	return t;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
//	long long at=a<<16;
	long long t=a/b;
	return t;
}

FLOAT f2F(float a) {
	int aint=*((int*)&a);
	int exp=((aint>>23)&0xff)-127;
	int sign=(aint>>31)&1;
	int f2=(aint&0x7fffff)|0x800000;
	int zhengshu=((f2>>(23-exp))<<16)&0x7fff0000;
	int xiaoshu=(f2<<(9+exp)>>16)&0x0000ffff;
	int F=zhengshu+xiaoshu;	
	if (sign==1) return -F;
	else return F;
}

FLOAT Fabs(FLOAT a) {
	if (a>=0) return a;
	else a=~a+1;
	return a;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

