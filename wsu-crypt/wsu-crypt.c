#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

int F (int r0, int r1, int round, const int ftable[256], unsigned long long int *key, int f[2], bool encrypt);
int G (int word, int round, int *i, const int ftable[256], int subkeys[12]);
int K(unsigned long long int *originalKey, int x, bool encrypt);

int main (int argc, char **argv){
	FILE *in, *ky, *out;
	bool encrypt;
	char infile[100], outfile[100], keyfile[100];

	//find encryption flag
	bool found = false;
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-e") == 0) {
			encrypt = true;
			found = true;
		}
		else if(strcmp(argv[i], "-d") == 0) {
			encrypt = false;
			found = true;
		}
	}
	if (found == false){
		printf("error: no mode selected. enter either -e to encrypt or -d for decrypt\n");
		return -1;
	}

	//find key flag
	found = false;
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-k") == 0){
			if(i+1 >= argc){
				printf("error: invalid argument. use -k <file>\n");
				return -1;
			}
			strcpy(keyfile, argv[i+1]);
			found = true;
		}
	}
	if (found == false){
		printf("error: no key identified. use -k <file>\n");
		return -1;
	}

	//find in flag
	found = false;
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-in") == 0){
			if(i+1 >= argc){
				printf("error: invalid argument. use -in <file>\n");
				return -1;
			}
			strcpy(infile, argv[i+1]);
			found = true;
		}
	}
	if (found == false){
		printf("error: no input identified. use -in <file>\n");
		return -1;
	}

	//find out flag
	found = false;
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-out") == 0){
			if(i+1 >= argc){
				printf("error: invalid argument. use -out <file>\n");
				return -1;
			}
			strcpy(outfile, argv[i+1]);
			found = true;
		}
	}
	if (found == false){
		printf("error: no output identified. use -out <file>\n");
		return -1;
	}

	int hex[2];
	if (pipe(hex) < 0) return -1;

	in = fopen(infile, "r");
	if (in == NULL) {
		printf("error: invalid input file\n");
		return -1;
	}
	ky = fopen(keyfile, "r");
	if (in == NULL) {
		printf("error: invalid key file\n");
		return -1;
	}
	out = fopen(outfile, "w");
	if (in == NULL) {
		printf("error: invalid output file\n");
		return -1;
	}

	static const int ftable[256] = { 
		0xa3,0xd7,0x09,0x83,0xf8,0x48,0xf6,0xf4,0xb3,0x21,0x15,0x78,0x99,0xb1,0xaf,0xf9,
		0xe7,0x2d,0x4d,0x8a,0xce,0x4c,0xca,0x2e,0x52,0x95,0xd9,0x1e,0x4e,0x38,0x44,0x28,
		0x0a,0xdf,0x02,0xa0,0x17,0xf1,0x60,0x68,0x12,0xb7,0x7a,0xc3,0xe9,0xfa,0x3d,0x53,
		0x96,0x84,0x6b,0xba,0xf2,0x63,0x9a,0x19,0x7c,0xae,0xe5,0xf5,0xf7,0x16,0x6a,0xa2,
		0x39,0xb6,0x7b,0x0f,0xc1,0x93,0x81,0x1b,0xee,0xb4,0x1a,0xea,0xd0,0x91,0x2f,0xb8,
		0x55,0xb9,0xda,0x85,0x3f,0x41,0xbf,0xe0,0x5a,0x58,0x80,0x5f,0x66,0x0b,0xd8,0x90,
		0x35,0xd5,0xc0,0xa7,0x33,0x06,0x65,0x69,0x45,0x00,0x94,0x56,0x6d,0x98,0x9b,0x76,
		0x97,0xfc,0xb2,0xc2,0xb0,0xfe,0xdb,0x20,0xe1,0xeb,0xd6,0xe4,0xdd,0x47,0x4a,0x1d,
		0x42,0xed,0x9e,0x6e,0x49,0x3c,0xcd,0x43,0x27,0xd2,0x07,0xd4,0xde,0xc7,0x67,0x18,
		0x89,0xcb,0x30,0x1f,0x8d,0xc6,0x8f,0xaa,0xc8,0x74,0xdc,0xc9,0x5d,0x5c,0x31,0xa4,
		0x70,0x88,0x61,0x2c,0x9f,0x0d,0x2b,0x87,0x50,0x82,0x54,0x64,0x26,0x7d,0x03,0x40,
		0x34,0x4b,0x1c,0x73,0xd1,0xc4,0xfd,0x3b,0xcc,0xfb,0x7f,0xab,0xe6,0x3e,0x5b,0xa5,
		0xad,0x04,0x23,0x9c,0x14,0x51,0x22,0xf0,0x29,0x79,0x71,0x7e,0xff,0x8c,0x0e,0xe2,
		0x0c,0xef,0xbc,0x72,0x75,0x6f,0x37,0xa1,0xec,0xd3,0x8e,0x62,0x8b,0x86,0x10,0xe8,
		0x08,0x77,0x11,0xbe,0x92,0x4f,0x24,0xc5,0x32,0x36,0x9d,0xcf,0xf3,0xa6,0xbb,0xac,
		0x5e,0x6c,0xa9,0x13,0x57,0x25,0xb5,0xe3,0xbd,0xa8,0x3a,0x01,0x05,0x59,0x2a,0x46
	};

	//read plaintext character by character, write converted hex to pipe
	if(encrypt){
		while(1){
			int num;
			char c[2];
			num = fgetc(in);
			if(feof(in)) break ;
			sprintf(c, "%01x", num);
			write(hex[1], &c, 2);
		}
		close(hex[1]);
	}
	
	//get 64bit key as unsigned long long int
	unsigned long long int key;
	char keytemp[20];
	fgets(keytemp, 17, ky);
	key = strtoull(keytemp, NULL, 16);
	fseek(ky, 0, SEEK_SET);

	//get k0 - k3 for whitening
	char ktemp[4][5];
	int k[4];
	for (int i = 0; i < 4; i++){
		fgets(ktemp[i], 5, ky);
		k[i] = strtol(ktemp[i], NULL, 16);
	}

	while(1){
		//get w0 - w3 for whitening
		char buffer[8];
		char wbuf[17];
		wbuf[16] = '\0';
		char wtemp[4][5];
		int w[4];
		size_t numread;

		//if encrypting, read from hexpipe
		numread = encrypt ? read(hex[0], wbuf, 16) : fread(&wbuf, 1, 16, in);
		if (numread == 0){
			close(hex[0]);
			fclose(in);
			fclose(ky);
			fclose(out);
			return 0;
		} 
		//fix partial block
		else if (numread < 16){
			while (numread < 17) wbuf[numread++] = '0';
		}

		//read in hex pipe
		int index = 0;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				wtemp[i][j] = wbuf[index++];
			}
			wtemp[i][4] = '\0';
			w[i] = strtol(wtemp[i], NULL, 16);
		}

		//initial whitening step, get r0 - r3
		int r[4];
		for (int i = 0; i < 4; i++){
			r[i] = w[i]^k[i];
		}

		//encryption rounds
		for (int rounds = 0; rounds < 16; rounds++){
			int f[2];
			int rnew[4];
			F(r[0], r[1], rounds, ftable, &key, f, encrypt);

			if(encrypt){
				rnew[0] = f[0]^r[2];
				rnew[0] = (rnew[0] >> 1) | (rnew[0] << 15) & 0x0000ffff;

				rnew[1] = (r[3] << 1) | (r[3] >> 15);
				rnew[1] = (rnew[1] ^ f[1]) & 0x0000ffff;
			} else {
				rnew[0] = (r[2] << 1 ) | (r[2] >> 15) & 0x0000ffff;
				rnew[0] = rnew[0]^f[0];

				rnew[1] = r[3]^f[1];
				rnew[1] = (rnew[1] >> 1) | (rnew[1] << 15) & 0x0000ffff;
			}

			rnew[2] = r[0];
			rnew[3] = r[1];
			for (int i = 0; i < 4; i++){
				r[i] = rnew[i]& 0x0000ffff;
			}
		}

		int y[4] = {r[2], r[3], r[0], r[1]};

		//output whitening
		int c[4] = {
			y[0]^k[0],
			y[1]^k[1],
			y[2]^k[2],
			y[3]^k[3]
		};

		char output[17];
		sprintf(output, "%04x%04x%04x%04x", c[0], c[1], c[2], c[3]);

		//convert hex to plaintext if decrypting
		if(!encrypt){
			for(int i = 0; i < 16; i+=2){
				char hexout[3] = {output[i], output[i+1], '\0'};
				int intout = strtol(hexout, NULL, 16);
				char charout = intout;
				fprintf(out, "%c", charout);
			}
		}
		else fprintf(out, "%s", output);
		
	}
}

int F (int r0, int r1, int round, const int ftable[256], unsigned long long int *key, int f[2], bool encrypt){
	int counter = 0;
	int t[2];
	int subkeys[12];
	if(encrypt){
		for (int i = 0; i < 12; i++){
			subkeys[i] = K(key, 4*round + i%4, encrypt);
		}
	} else {
		for (int i = 11; i > -1; i--){
			subkeys[i] = K(key, 4*round + (11-i)%4, encrypt);
		}
	}

	t[0] = G(r0, round, &counter, ftable, subkeys);
	t[1] = G(r1, round, &counter, ftable, subkeys);
	char k1[2][5], k2[2][3];
	sprintf(k1[0], "%02x", subkeys[8]);
	sprintf(k2[0], "%02x", subkeys[9]);
	sprintf(k1[1], "%02x", subkeys[10]);
	sprintf(k2[1], "%02x", subkeys[11]);
	strcat(k1[0], k2[0]);
	strcat(k1[1], k2[1]);
	int k[2];
	k[0] = strtol(k1[0], NULL, 16);
	k[1] = strtol(k1[1], NULL, 16);

	f[0] = (t[0] + 2*t[1] + k[0])%0x10000;
	f[1] = (2*t[0] + t[1] + k[1])%0x10000;
}

int G (int word, int round, int *counter, const int ftable[256], int subkeys[12]){
	int g[6];
	char wtemp[5];
	char wsplit[2][3];
	int w[2];
	sprintf(wtemp, "%04x", word);
	int j = 0;
	for(int i = 0; i < 2; i++){
		wsplit[i][0] = wtemp[j++];
		wsplit[i][1] = wtemp[j++];
		wsplit[i][2] = '\0';
		w[i] = strtol(wsplit[i], NULL, 16);
	}

	g[0] = w[0];
	g[1] = w[1];
	for (int i = 0; i < 4; i++){
		g[i+2] = (ftable[(g[i+1]^subkeys[*counter])])^g[i];
		*counter = *counter + 1;
	}

	char g5[17], g6[9];
	sprintf(g5, "%04x", g[4]);
	sprintf(g6, "%02x", g[5]);
	strcat(g5, g6);
	int output = strtol(g5, NULL, 16);

	return output;


}

int K(unsigned long long int *originalKey, int x, bool encrypt){
	char string[17];
	if (encrypt) *originalKey = (*originalKey << 1) | (*originalKey >> (63));
	sprintf(string, "%016llx", *originalKey);
	char substrings[8][3];
	int n = 0;
	if (encrypt){
		for (int i = 7; i > -1; i--){
			substrings[i][0] = string[n++];
			substrings[i][1] = string[n++];
			substrings[i][2] = '\0';
		}
	} else {
		for (int i = 0; i < 8; i++){
			substrings[i][0] = string[n++];
			substrings[i][1] = string[n++];
			substrings[i][2] = '\0';
		}
	}
	if(!encrypt) *originalKey = (*originalKey >> 1) | (*originalKey << (63));
	return strtol(substrings[x%8], NULL, 16);
}