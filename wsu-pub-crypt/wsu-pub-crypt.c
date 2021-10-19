#include "wpc-header.h"

int main(int argc, char **argv){

	char ptext[50];
	char ctext[50];
	char dtext[50];

	char prikey[50];
	char pubkey[50];

	bool key = false, in = false, out = false;

	for(int i = 0; i < argc; i++){
		if(!strcmp(argv[i], "-genkey")){
			keygen();
			return 0;
		}
		else if(!strcmp(argv[i], "-e")){
			for(int j = 0; j < argc; j++){
				if(strcmp(argv[j], "-k") == 0) {
					strcpy(pubkey, argv[j+1]);
					key = true;
				}
				if(strcmp(argv[j], "-in") == 0) {
					strcpy(ptext, argv[j+1]);
					in = true;
				}
				if(strcmp(argv[j], "-out") == 0) {
					strcpy(ctext, argv[j+1]);
					out = true;
				}
			}
			if(key && in && out){
				encrypt(ptext, pubkey, ctext);
				return 0;
			}
			printf("invalid arguments. use -k <path> -in <path> -out <path>\n");
			return 0;
		}

		else if(!strcmp(argv[i], "-d")){
			for(int j = 0; j < argc; j++){
				if(strcmp(argv[j], "-k") == 0) {
					strcpy(prikey, argv[j+1]);
					key = true;
				}
				if(strcmp(argv[j], "-in") == 0) {
					strcpy(ctext, argv[j+1]);
					in = true;
				}
				if(strcmp(argv[j], "-out") == 0) {
					strcpy(dtext, argv[j+1]);
					out = true;
				}
			}
			if(key && in && out){
				decrypt(ctext, prikey, dtext);
				return 0;
			}
			printf("invalid arguments. use -k <path> -in <path> -out <path>\n");
			return 0;
		}
	}
	printf("invalid mode selected. use -genkey, -e, or -d\n");
	return 0;
}