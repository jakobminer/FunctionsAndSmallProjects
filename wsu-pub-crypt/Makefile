wsu-pub-crypt: wsu-pub-crypt.o encrypt.o decrypt.o keygen.o tools.o
	gcc -o wsu-pub-crypt wsu-pub-crypt.o encrypt.o decrypt.o keygen.o tools.o -lm

wsu-pub-crypt.o: wsu-pub-crypt.c wpc-header.h
	gcc -c wsu-pub-crypt.c wpc-header.h -lm

encrypt: encrypt.c wpc-header.h
	gcc -c encrypt.c wpc-header.h -lm

decrypt.o: decrypt.c wpc-header.h
	gcc -c decrypt.c wpc-header.h -lm

keygen.o: keygen.c wpc-header.h
	gcc -c keygen.c wpc-header.h -lm

tools.o: tools.c wpc-header.h
	gcc -c tools.c wpc-header.h -lm

clean:
	rm wsu-pub-crypt wsu-pub-crypt.o encrypt.o decrypt.o keygen.o tools.o ctext.txt dtext.txt prikey.txt pubkey.txt wpc-header.h.gch
