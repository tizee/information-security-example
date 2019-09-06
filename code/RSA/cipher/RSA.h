#ifndef RSA_H
#define RSA_H
#include<stdio.h> // MPIR FILE* function
#include<string>
#include<ctime>
#include<mpir.h>
#include<mpirxx.h>
#include<iostream>
#include<fstream>
#include "md5.h"

typedef uint64_t u64;
typedef uint32_t u32;

using namespace std;
class RSA
{
private:
	string keystr;
	mpz_t p,q,n,phi_n,e,d;
	void gen_prime(mpz_t prime);
	int md5_hash(string& input, string& hash);
public:
	RSA();
	int encrypt_str(string& input, string& output);
	int decrypt_str(string& input, string& output);
	int sign_file(string& input,string& output);
	int verify_file(string& msg_file,string& sign_file);
	void print();
};

#endif // RSA_H
