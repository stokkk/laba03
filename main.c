#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<openssl/evp.h>
#include<openssl/md5.h>


typedef unsigned char byte;

#define BUFF_SIZE 1024
#define error(message)\
	printf("ERROR: to %s\n", message);\
	exit(0);

byte * digest_message(char *filename) {
	unsigned int digest_length = MD5_DIGEST_LENGTH;
	byte *digest = (byte *)OPENSSL_malloc(digest_length);
	if(digest == NULL) {
		error("allocate memory for digest variable \'digest\'");	
	}
	MD5_CTX mdctx;
	if (1 != MD5_Init(&mdctx)) {
		error("create message digest context \'mdctx\' and set up digest context\
\'mdctx\' to use a digest \'type\'");
	}
	
	FILE* fp = NULL;
	if ( (fp = fopen(filename,"rb")) == NULL) {
		error("can\'t open the file \'filename\'");
	}
	byte buff[1024];

	while(1) {
		int size = fread(&buff,sizeof(byte),BUFF_SIZE,fp);
		if (size <= 0) { break; }
		if (1 != MD5_Update(&mdctx, buff, size)) {
			error("hash \'message_length\' bytes of data at \'message\' into the 				digest context \'mdctx\'");
		}
	}

	fclose(fp);

	if (1 != MD5_Final(digest, &mdctx)) {
		error("retrieve the digest value from \'mdctx\' and place it in \'digest\'");
	}
	return digest;
}

int main(int argc, char* argv[]) {
	if (argc < 2) { printf("Usage: ./main [file_with_info] [file_to_digest]\n"); exit(0); }
	printf("%s\n", "Hello, This is 3 practical lesson!");
	byte* filename = argv[1];
	byte* digest = digest_message(filename);
	if (argc > 2) {
		FILE* fp = fopen(argv[2],"wb");
		if(fp == NULL) { error("can\'t open the file \'argv[2]\' to write digest"); }
		fwrite(digest,sizeof(byte),strlen(digest),fp);
	} else BIO_dump_fp(stdout, digest, MD5_DIGEST_LENGTH);
	return 0;
}
