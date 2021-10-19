#include "wpc-header.h"

int decrypt(char *in, char *key, char *out){
	FILE *ctext = fopen(in, "r");
	FILE *prikey = fopen(key, "r");
	FILE *dtext = fopen(out, "w");

	unsigned long long int c1, c2, p, g, d, m;
	char str[50];
	char * token;

	fgets(str, 50, prikey);
	token = strtok(str, " ");

	p = strtoull(token, NULL, 10);
	token = strtok(NULL, " ");

	g = strtoull(token, NULL, 10);
	token = strtok(NULL, " ");

	d = strtoull(token, NULL, 10);
	token = strtok(NULL, " ");

	char mOut[5];
	mOut[4] = '\0';
	while(fgets(str, 50, ctext)!=NULL){
		token = strtok(str, " ");

		c1 = strtoull(token, NULL, 10);
		token = strtok(NULL, " ");

		c2 = strtoull(token, NULL, 10);
		token = strtok(NULL, " ");

		m = (fast_mod_exp(c1, (p-1-d), p)*(c2%p))%p;

		mOut[0] = m & 0x000000FF;
		mOut[1] = (m & 0x0000FF00) >> 8;
		mOut[2] = (m & 0x00FF0000) >> 16;
		mOut[3] = (m & 0xFF000000) >> 24;
		fprintf(dtext, "%s", mOut);
		printf("%s", mOut);
	}

	fclose(ctext);
	fclose(prikey);
	fclose(dtext);
	return 0;
}