#include "cfb.h"

DESCFB::DESCFB(ui64 key,ui64 IV):
	des(key),
	IV(IV),
	first_block(IV)
{
}
/*
 * CFB mode encrypt:
 * Ci = Ek(C_{i_1}) XOR Pi
 * C0 = IV
 */

ui64 DESCFB::encrypt(ui64 block){
	first_block = des.encrypt(first_block)^block;
	return first_block;
}

/*
 * CFB mode encrypt:
 * Pi = Ek(C_{i_1}) XOR Ci
 * C0 = IV
 */

ui64 DESCFB::decrypt(ui64 block){
	ui64 res = des.encrypt(first_block)^block;
	first_block = block;
	return res;
}

void DESCFB::reset(){
	first_block=IV;
}
