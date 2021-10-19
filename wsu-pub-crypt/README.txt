Jakob Miner
CS 427
Project 2
16 Apr 2021

WSU-PUB-CRYPT
public key / private key encryption system

contents:
	wsu-pub-crypt.c
	keygen.c
	encrypt.c
	decrypt.c
	tools.c
	wpc-header.h
	Makefile
	README.txt

to build:
	$ make

to run:
	$ ./wsu-pub-crypt -genkey
		(Generate key)

	$ ./wsu-pub-crypt -e -k <PATH> -in <PATH> -out <PATH>
		(Encrypt)

	$ ./wsu-pub-crypt -d -k <PATH> -in <PATH> -out <PATH> 
		(Decrypt)

to clean up directory / generated files:
	$ make clean

notes:
	program will call out if any flag is missing. I didnt bother checking if provided
	flags or paths are valid, so make sure your arguments are correctly formated.