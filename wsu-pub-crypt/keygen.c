#include "wpc-header.h"

int keygen(void){
	unsigned int q, p, r, d, e2;

	srand(time(0));

	while(1){
		while(1){
			q = rand();
			q = q | 0x40000001;
			if(miller_rabin(q, 10) && q%12==5) break;
		}
		p = q*2+1;
		if(miller_rabin(p, 10)){
			break;
		}
	}
	d = (rand() % (p-2)) + 1;
	e2 = fast_mod_exp(2, d, p);

	FILE *fp = fopen("pubkey.txt", "w");
	fprintf(fp, "%u 2 %u\n", p, e2);
	printf("pubkey = %u 2 %u\n", p, e2);
	fclose(fp);

	fp = fopen("prikey.txt", "w");
	fprintf(fp, "%u 2 %u\n", p, d);
	printf("prikey = %u 2 %u\n", p, d);
	fclose(fp);
	return 0;
}