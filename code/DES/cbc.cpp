#include "cbc.h"

DESCBC::DESCBC(ui64 key,ui64 IV):
	des(key),
	IV(IV),
	first_block(IV)
{
}

ui64 DESCBC::encrypt(ui64 block){
	first_block= des.encrypt(block^first_block);
	return first_block;
}

ui64 DESCBC::decrypt(ui64 block){
	ui64 res=des.decrypt(block)^first_block; 
	first_block = block;
	return res;
}

void DESCBC::reset(){
	first_block = IV;
}
