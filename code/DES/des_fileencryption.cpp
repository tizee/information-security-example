#include "des_fileencryption.h"
DES_CFB_FileEncryption::DES_CFB_FileEncryption(ui64 key,ui64 IV):
des_cfb(key,IV)
{
}

int DES_CFB_FileEncryption::encrypt(string input, string output) {
    return cipher(input, output, true);
}
int DES_CFB_FileEncryption::decrypt(string input, string output) {
    return cipher(input, output, false);
}

int DES_CFB_FileEncryption::cipher(string input, string output, bool mode) {
    ifstream ifile;
    ofstream ofile;
    // 8bytes buffer
    ui64 buffer;

    if (input.size() < 1) {
        // get input from stdin
        input = "/dev/stdin";
    }

    if (output.size() < 1) {
        // set output to stdout
        output = "/dev/stdout";
    }

    ifile.open(input, ios_base::in | ios_base::binary | ios_base::ate);
    ofile.open(output, ios_base::out | ios_base::binary);
    // use ate mode to get file end
    ui64 size = ifile.tellg();
    ifile.seekg(0, ios_base::beg);

    ui64 block_num = size / 8;


    if (!mode) {
    // decryption
        block_num--;
    }

#ifdef DEBUG
cout<<"debug: fileencryption:"<<endl;
cout<<"debug: size:"<<int(size)<<endl;
cout<<"debug: block_num:"<<int(block_num)<<endl;
#endif

    for (ui64 i = 0; i < block_num; i++) {
        ifile.read((char *)&buffer, 8);
#ifdef DEBUG
cout<<"debug: buffer:"<<(char*)&buffer<<endl;
#endif

        if (mode) {
            buffer = des_cfb.encrypt(buffer);
        } else {
            buffer = des_cfb.decrypt(buffer);
        }
#ifdef DEBUG
cout<<"debug: after operation buffer:"<<(char*)&buffer<<endl;
#endif
        // 8 bytes
        ofile.write((char *)&buffer, 8);
    }

    if (mode) {
        // encryption
// get amount of padding size
        ui8 padding = 8 - (size % 8);

        if (padding == 0) {
            padding = 8;
        }

#ifdef DEBUG
cout<<"debug: padding:"<<padding<<endl;
#endif

	buffer = (ui64)0;
// continue reading remaining file
        ifile.read((char *)&buffer, 8 - padding);
// byte padding : 1 following by 0s
        ui8 shift= padding * 8;

#ifdef DEBUG
cout<<"debug: shift:"<<shift<<endl;
#endif

        buffer <<= shift;
        buffer |= (ui64)0x0000000000000001 << (shift - 1);
        buffer = des_cfb.encrypt(buffer);
        ofile.write((char *)&buffer, 8);
    } else {
        // decryption
// continue reading remaining file
        ifile.read((char *)&buffer, 8);
        buffer = des_cfb.decrypt(buffer);
// get padding amount
        ui8 padding = 0;

// check for and recording padding on the end
        while (!(buffer & 0x00000000000000ff)) {
            buffer >>= 8;
            padding++;
        }
        buffer >>= 8;
        padding++;

#ifdef DEBUG
cout<<"debug: padding:"<<padding<<endl;
#endif
        if (padding != 8) {
            ofile.write((char *)&buffer, 8 - padding);
        }
    }

    ifile.close();
    ofile.close();
    return 0;
}


DES_CBC_FileEncryption::DES_CBC_FileEncryption(ui64 key,ui64 IV):
des_cbc(key,IV)
{
}

int DES_CBC_FileEncryption::encrypt(string input, string output) {
    return cipher(input, output, true);
}
int DES_CBC_FileEncryption::decrypt(string input, string output) {
    return cipher(input, output, false);
}
int DES_CBC_FileEncryption::cipher(string input, string output, bool mode) {
    ifstream ifile;
    ofstream ofile;
    // 8bytes buffer
    ui64 buffer;

    if (input.size() < 1) {
        // get input from stdin
        input = "/dev/stdin";
    }

    if (output.size() < 1) {
        // set output to stdout
        output = "/dev/stdout";
    }

    ifile.open(input, ios_base::in | ios_base::binary | ios_base::ate);
    ofile.open(output, ios_base::out | ios_base::binary);
    // use ate mode to get file end
    ui64 size = ifile.tellg();
    ifile.seekg(0, ios_base::beg);

    ui64 block_num = size / 8;


    if (!mode) {
    // decryption
        block_num--;
    }

#ifdef DEBUG
cout<<"debug: fileencryption:"<<endl;
cout<<"debug: size:"<<int(size)<<endl;
cout<<"debug: block_num:"<<int(block_num)<<endl;
#endif

    for (ui64 i = 0; i < block_num; i++) {
        ifile.read((char *)&buffer, 8);
#ifdef DEBUG
cout<<"debug: buffer:"<<(char*)&buffer<<endl;
#endif

        if (mode) {
            buffer = des_cbc.encrypt(buffer);
        } else {
            buffer = des_cbc.decrypt(buffer);
        }
#ifdef DEBUG
cout<<"debug: after operation buffer:"<<(char*)&buffer<<endl;
#endif
        // 8 bytes
        ofile.write((char *)&buffer, 8);
    }

    if (mode) {
        // encryption
// get amount of padding size
        ui8 padding = 8 - (size % 8);

        if (padding == 0) {
            padding = 8;
        }

#ifdef DEBUG
cout<<"debug: padding:"<<padding<<endl;
#endif

	buffer = (ui64)0;
// continue reading remaining file
        ifile.read((char *)&buffer, 8 - padding);
// byte padding : 1 following by 0s
        ui8 shift= padding * 8;

#ifdef DEBUG
cout<<"debug: shift:"<<shift<<endl;
#endif

        buffer <<= shift;
        buffer |= (ui64)0x0000000000000001 << (shift - 1);
        buffer = des_cbc.encrypt(buffer);
        ofile.write((char *)&buffer, 8);
    } else {
        // decryption
// continue reading remaining file
        ifile.read((char *)&buffer, 8);
        buffer = des_cbc.decrypt(buffer);
// get padding amount
        ui8 padding = 0;

// check for and recording padding on the end
        while (!(buffer & 0x00000000000000ff)) {
            buffer >>= 8;
            padding++;
        }
        buffer >>= 8;
        padding++;

#ifdef DEBUG
cout<<"debug: padding:"<<padding<<endl;
#endif
        if (padding != 8) {
            ofile.write((char *)&buffer, 8 - padding);
        }
    }

    ifile.close();
    ofile.close();
    return 0;
}

