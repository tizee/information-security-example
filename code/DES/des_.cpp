#include "des_.h"
#include "DES.cpp"

DES::DES(ui64 key) {
    generate_subkey(key);
}

void DES::generate_subkey(ui64 key) {
    char key_bits[64];
    memset(key_bits, 0, 64 * sizeof(char));
    Char8ToBit64((char *)&key, key_bits);
    DES_MakeSubKeys(key_bits, sub_key);
}

ui64 DES::des(ui64 block, bool mode) {
    ui64 res = 0;

    if (mode) {
// encrypt
        DES_EncryptBlock((char *)&block, sub_key, (char *)&res);
    } else {
// decrypte
        DES_DecryptBlock((char *)&block, sub_key, (char *)&res);
    }

    return res;
}

ui64 DES::encrypt(ui64 block) {
    return des(block, true);
}

ui64 DES::decrypt(ui64 block) {
    return des(block, false);
}

ui64 DES::encrypt(ui64 block, ui64 key) {
    DES des(key);
    return des.des(block, true);
}

ui64 DES::decrypt(ui64 block, ui64 key) {
    DES des(key);
    return des.des(block, false);
}
