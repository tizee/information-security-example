#ifndef FILEENCRYPTION_H
#define FILEENCRYPTION_H

#include<iostream>
#include<fstream>
#include<string>
// stl date&time C++11
#include<chrono>
// stl pseudo-random generator library C++11
#include<random>

using namespace std;

#include "cbc.h"
#include "cfb.h"

class DES_CFB_FileEncryption{
	public:
		DES_CFB_FileEncryption(ui64 key,ui64 IV);
		int encrypt(string input,string output);
		int decrypt(string input,string output);
		int cipher(string input,string output,bool mode);
	private:
		DESCFB des_cfb;
};

class DES_CBC_FileEncryption{
	public:
		DES_CBC_FileEncryption(ui64 key,ui64 IV);
		int encrypt(string input,string output);
		int decrypt(string input,string output);
		int cipher(string input,string output,bool mode);
	private:
		DESCBC des_cbc;
};

#endif //FILEENCRYPTION_H
