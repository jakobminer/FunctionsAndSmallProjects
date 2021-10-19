#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int miller_rabin(long long int n, long long int s);
bool witness(long long int a, long long int n, bool output);
int fast_mod_exp(long long int a, long long int b, long long int n);

int main (int argc, char **argv){
	long long int n, a, s;
	bool found;

	//find n
	found = false;
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-n") == 0){
			if(i+1 >= argc){
				printf("error: invalid argument use. use -n <int>\n");
				return -1;
			}
			n = strtoll(argv[i+1], NULL, 10);
			found = true;
		}
	}
	if (found == false){
		printf("error: no n identified. use -n <int>\n");
		return -1;
	}

	//find a
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-a") == 0){
			if(i+1 >= argc){
				printf("error: invalid argument use. use -a <int>\n");
				return -1;
			}
			a = strtoll(argv[i+1], NULL, 10);
			witness(a, n, true);
			return 0;
		}
	}

	//find s
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-s") == 0){
			if(i+1 >= argc){
				printf("error: invalid argument use. use -s <int>\n");
				return -1;
			}
			s = strtoll(argv[i+1], NULL, 10);
			miller_rabin(n, s);
			return 0;
		}
	}

	printf("error: missing argument. use -a or -s <int> <int>\n");
	return -1;
}

int miller_rabin(long long int n, long long int s){
	int a;
	time_t t;
	srand((unsigned) time(&t));

	for (int j = 0; j < s; j++){
		long long int a = 1 + rand() % (n-2);
		if(witness(a, n, false)){
			printf("composite\n");
			return 0;
		}
	}
	printf("prime\n");
	return 0;
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

int fast_mod_exp(long long int a, long long int b, long long int n){
	long long int c = 0, d = 1;
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
	



