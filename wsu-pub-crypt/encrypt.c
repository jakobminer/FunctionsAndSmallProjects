#include "wpc-header.h"

int encrypt (char *in, char *key, char *out){
	FILE *ptext = fopen(in, "r");
	FILE *pubkey = fopen(key, "r");
	FILE *ctext = fopen(out, "w");

	unsigned long long int p, g, e2;
	char str[50];
	char * token;

	fgets(str, 50, pubkey);
	token = strtok(str, " ");

	p = strtoull(token, NULL, 10);
	token = strtok(NULL, " ");

	g = strtoull(token, NULL, 10);
	token = strtok(NULL, " ");

	e2 = strtoull(token, NULL, 10);
	token = strtok(NULL, " ");

	char wtemp[5];
	unsigned long long int m, k, c1, c2, numread;
	srand(time(0));

	do{
		m = 0;
		numread = fread(&m, 1, 4, ptext);

		k = rand() % p;

		c1 = fast_mod_exp(g, k, p);
		c2 = (fast_mod_exp(e2, k, p)*(m%p))%p;

		fprintf(ctext, "%llu %llu\n", c1, c2);
		printf("%llu %llu\n", c1, c2);
	} while(numread>0);

	fclose(ptext);
	fclose(pubkey);
	fclose(ctext);
	return 0;
}
