#ifndef DES_CLASS_H
#define DES_CLASS_H

#include<iostream>
using namespace std;

#define ui64 uint64_t
#define ui8 uint64_t


class DES{
	public:
		DES(ui64 key);
		ui64 des(ui64 block,bool mode);
		ui64 encrypt(ui64 block);
		ui64 decrypt(ui64 block);
		static ui64 encrypt(ui64 block,ui64 key);
		static ui64 decrypt(ui64 block,ui64 key);
	protected:
		void generate_subkey(ui64 key);
	private:
		// subkey [16][48]
		// 16*64bits=16* 8bytes
		// char[16][48]
		// 16*48(8)=16* (48*1) bytes
		char sub_key[16][48];
};

#endif // DES_CLASS_H
