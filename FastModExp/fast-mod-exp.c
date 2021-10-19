#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char **argv){
	long long int a, b, n;
	bool found;

	//find a
	found = false;
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-a") == 0){
			if(i+1 >= argc){
				printf("error: invalid argument use. use -a <int>\n");
				return -1;
			}
			a = strtoll(argv[i+1], NULL, 10);
			found = true;
		}
	}
	if (found == false){
		printf("error: no a identified. use -a <int>\n");
		return -1;
	}

	//find b
	found = false;
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-b") == 0){
			if(i+1 >= argc){
				printf("error: invalid argument use. use -b <int>\n");
				return -1;
			}
			b = strtoll(argv[i+1], NULL, 10);
			found = true;
		}
	}
	if (found == false){
		printf("error: no b identified. use -b <int>\n");
		return -1;
	}

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

	printf("%lld^%lld (mod %lld) = %lld\n", a, b, n, d);
	return 0;

}