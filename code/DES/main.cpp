#include<iostream>
#include<string>
using namespace std;

#include "des_fileencryption.h"
void generate_IV(ui64 &IV);

static void helper(string name) {
    std::cerr << "Usage:" << name << endl
              << "encrypt: -e -cfb/-cbc -key {key} [input-file] [output-file]" << endl
              << "encrypt: -e -cfb/-cbc -keyfile {keyfile} [input-file] [output-file]" <<endl
              << "decrypt: -d -cfb/-cbc -key {key} [input-file] [output-file]" << endl
            << "decrypt: -d -cfb/-cbc -keyfile {key} [input-file] [output-file]" << endl;
}

int main(int argc, const char **argv) {
    if (argc < 7) {
        helper(argv[0]);
        return 1;
    }

    string en_flag = argv[1];
    string mode = argv[2];
    string key_flag = argv[3];

    if (en_flag.compare("-e") != 0 && en_flag.compare("-d")!=0) {
        helper(argv[0]);
        return 2;
    }

    if (mode.compare("-cfb") != 0 && mode.compare("-cbc")!=0) {
        helper(argv[0]);
        return 2;
    }

    if (key_flag.compare("-key") != 0 && key_flag.compare("-keyfile")!=0) {
        helper(argv[0]);
        return 2;
    }

    string input;
    string output;

    if (argc > 5) {
        input = argv[5];
    }

    if (argc > 6) {
        output = argv[6];
    }

    string key_str;

    if (key_flag.compare("-key") == 0) {
        key_str = argv[4];
    } else {
        // to-do:load key from file
    }

    ui64 key = strtoull(key_str.c_str(), nullptr, 16);

// encryption
    if (en_flag.compare("-e") == 0) {
        if (mode.compare("-cbc") == 0) {
		ui64 IV= (ui64)0;
            DES_CBC_FileEncryption f(key,IV);
            f.encrypt(input,output);
        } else {
	ui64 IV= (ui64)0;
            DES_CFB_FileEncryption f(key,IV);
            f.encrypt(input,output);
        }
    }

// decryption
    if (en_flag.compare("-d") == 0) {
        if (mode.compare("-cbc") == 0) {
	ui64 IV= (ui64)0;
            DES_CBC_FileEncryption f(key,IV);
            f.decrypt(input,output);
        } else {
		// to-do: append IV to the last block
	    /* ui64 IV = generate_IV(); */ 
	ui64 IV= (ui64)0;
            DES_CFB_FileEncryption f(key,IV);
            f.decrypt(input,output);
        }
    }

#ifdef DEBUG
    cout << "debug:\n"
	    <<"argc:"<<argc<<endl
         << "en_flag:" << en_flag << endl
         << "mode:" << mode << endl
         << "key_flag:" << key_flag<< endl
         << "key:" << key << endl
         << "input:" << input << endl
         << "output:" << output << endl;
#endif

    return 0;
}
// generate initialization vector with pseudo-random
// IV size is the same as block size

// cryptographically secure/strong pseudo-random number generator (CSPRNG), specially desgined for cryptography, e.g Mersenne Twister

//This random IV ensures that each message encrypts differently, such that seeing multiple messages encrypted with the same key doesn't give the attacker any more information than just seeing a single long message.
// https://crypto.stackexchange.com/questions/3965/what-is-the-main-difference-between-a-key-an-iv-and-a-nonce/3970?newreg=43468066cc51482a86a663c2d25f9566

// an IV is a nonce with an additional requirement: it must be selected in a nonpredictable way. it should not be reused or be predictable which means the IV should be generated randomly(pseudo-random)
// reference from :https://www.oreilly.com/library/view/secure-programming-cookbook/0596003943/ch04s09.html

ui64 generate_IV() {
    // 64bits = 8bytes
    // time-based seed
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // used for bias
    std::random_device rd;
    seed += rd();
#ifdef DEBUG
    cout << "debug: seed:" << seed << endl;
#endif
    // use Mersenne Twister engine, uint_fast64_t
    mt19937_64 engine(seed);
    uint_fast64_t rn = engine();
#ifdef DEBUG
    cout << "debug: sizeof(uint_fast64_t):" << sizeof(rn) << endl;
    cout << "debug: random number:" << rn << endl;
#endif
    // convert uint_fast64_t to char bit representation
    return rn;
}
