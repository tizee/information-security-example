// This file contains the 'main' function.
// Program execution begins and ends there.

#include <iostream>
#include "RSA.h"
using namespace std;

void test_rsa_encrypt() {
	RSA test;
	string s = "123456ABCDEF";
	mpz_class t(s, 16);
	cout << t.get_str(16)<<endl;
	string en_msg;

	cout << endl;
	test.encrypt_str(s, en_msg);
	cout << "encrypted msg:"<<en_msg << endl;
	string msg;
	test.decrypt_str(en_msg, msg);
	cout << "decrypted msg:"<<msg << endl;
}

void test_rsa_sign() {
	RSA test;
	string input = "test.in";
	string output = "test.sign";
	test.sign_file(input,output);
	int state = test.verify_file(input,output);
	if (state>=0) {
		cout << "ok" << endl;
	}
	else {
		cout << "failed" << endl;
	}

	// change msg file a little bit
	fstream f(input, ios_base::binary | ios_base::in | ios_base::out| ios_base::ate|ios_base::app);
	if (!f.is_open()) {
		cout << "open failed" << endl;
		exit(1);
	}
	string change = "a little bit change";
	f << change;
	f.close();
	state= test.verify_file(input,output);
	if (state>=0) {
		cout << "ok" << endl;
	}
	else {
		cout << "failed" << endl;
	}
}

int main()
{
	test_rsa_sign();
	return 0;
}
