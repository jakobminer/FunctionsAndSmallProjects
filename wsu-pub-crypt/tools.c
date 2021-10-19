#include "wpc-header.h"

int miller_rabin(long long int n, long long int s){
	int a;

	for (int j = 0; j < s; j++){
		long long int a = 1 + rand() % (n-2);
		if(witness(a, n, false)){
			return 0;
		}
	}
	return 1;
}

bool witness(long long int a, long long int n, bool output){
	long long int t, u, x, x2;
	t=0;
	u = n-1;
	if(u%2 != 0){
		if (output) printf("n is even\n");
		return true;
	}
	while(u%2==0){
		t++;
		u = u / 2;
	}
	x = fast_mod_exp(a,u, n);
	for(int i = 0; i < t; i++){
		x2 = (x*x)%n;
		if ((x2 == 1) && (x != 1) && (x!=(n-1))) {
			if(output)printf("true\n");
			return true;
		}
		x = x2;
	}
	if(x2 != 1) {
		if(output)printf("true\n");
		return true;
	}
	if(output)printf("false\n");
	return false;
}

unsigned long long int fast_mod_exp(unsigned long long int a, unsigned long long int b, unsigned long long int n){
	unsigned long long int c = 0, d = 1;
	int k = -1;
	for (int i = 63; i > -1; i--){
		if(b & (1 << i)){
			k = i - 32;
			break;
		}
	}

	for(int i = k; i > -1; i--){
		c = 2*c;
		d = (d * d)%n;
		if (b & (1<<i)){
			c++;
			d = (d*a)%n;
		}
	}
	return d;
}