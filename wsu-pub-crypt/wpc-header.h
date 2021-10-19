#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

unsigned long long int fast_mod_exp(unsigned long long int a, unsigned long long int b, unsigned long long int n);
int miller_rabin(long long int n, long long int s);
bool witness(long long int a, long long int n, bool output);
int keygen(void);
int encrypt (char *in, char *key, char *out);
int decrypt(char *in, char *key, char *out);