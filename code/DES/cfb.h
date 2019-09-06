#ifndef CFB_H
#define CFB_H
#include<iostream>
#include<string>

using namespace std;

#include "des_.h"
#define BUF_SIZE 128
/* #define DEBUG */

class DESCFB{
	public:
		DESCFB(ui64 key,ui64 IV);
		ui64 encrypt(ui64 block);
		ui64 decrypt(ui64 block);
		void reset();
	private:
		DES des;
		ui64 IV;
		// prepend the IV at the first block of the cipher text 
		ui64 first_block;
};

#endif // CFB_H
