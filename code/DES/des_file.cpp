#include<bits/stdc++.h>
#include "DES.cpp"

using namespace std;

#define BUF_SIZE 128

/*
 *reference:https://youtu.be/5wzmEKjNqiU
 * - Don't know size ahead of time
 * - Conserve RAM
 * - Dynamically-sized
 * - Each line a sperate string
 * - Random access to each string
 **/

void resize(char** &pts, int &size);
char **loadfile(const char *&filename);
int savefile(const char* &filename, char ** &lines);
int loadKeyFile(const string &file_path, char subKeys[16][48]);
int saveKeyFile(const string &file_path, char subKeys[16][48]);
void encrypt8bytes(char *str, int start, int end, char chunk[8],char subKeys[16][48]);
void decrypt8bytes(char *str, int start, int end, char chunk[8],char subKeys[16][48]);
char **do_encrypt(char** &plainText, char subKeys[16][48]);
char **do_decrypt(char** &enText, char subKeys[16][48]);


void resize(char ** &pts, int &size) {
    char **new_pts = (char **)realloc(pts, size * sizeof(char *));

    if (!new_pts) {
        fprintf(stderr, "Realloc failed!\n");
        exit(1);
    }

    pts = new_pts;
}

int savefile(const char *filename, char **lines) {
    FILE *fp = fopen(filename, "wb+");

    if (!fp) {
        cerr << "Couldn't write, Failed!" << endl;
        return -1;
    }

    int i = 0;

    while (lines[i] != NULL) {
        fprintf(fp, "%s\n", lines[i]);
        i++;
    }

    fclose(fp);
    return 0;
}

char **loadfile(const char *filename) {
    FILE *f = fopen(filename, "rb");

    if (!f) {
        fprintf(stderr, "file %s not found!\n", filename);
        return NULL;
    }

    char **str_pts = NULL;
    int size = 0;
    int count = 0;
    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);

    // read by line
    while (fgets(buf, BUF_SIZE, f)) {
        // resize the str_pts if full
        if (count % BUF_SIZE == 0) {
            size = size + BUF_SIZE;
            resize(str_pts, size);
        }

        // trim off escape character \n
        buf[strlen(buf) - 1] = '\0';
        int blen = strlen(buf);
        // get rid of \0
        str_pts[count] = (char *)malloc(blen * sizeof(char));
        // could also use strcpy here
        memcpy((char *)str_pts[count], (char *) buf, blen);
        count++;
    }

    fclose(f);

    if (count == size) {
        size++;
        resize(str_pts, size);
    }

    str_pts[count] = NULL;

    return str_pts;
}



int saveKeyFile(const string &file_path, char subKeys[16][48]) {
    FILE *key_file = fopen(file_path.c_str(), "wb+");

    for (int i = 0; i < 16; i++) {
      for(int j =0;j<48;j++){
      fprintf(key_file, "%c", subKeys[i][j]);
      }
      fprintf(key_file,"\n");
    }

    fclose(key_file);
    return 0;
}

// subKey file
int loadKeyFile(const string &file_path, char subKeys[16][48]) {
    memset(subKeys,0,sizeof(subKeys));
    FILE *key_file = fopen(file_path.c_str(), "rb");

    if (key_file == NULL) {
        cerr << file_path << " not found!" << endl;
        return -1;
    }

    char *line = NULL;
    size_t len = 0;
    int row = 0;

    while ((getline(&line, &len, key_file)) != -1) {
        memcpy((char *)subKeys[row], (char *)line, 48);
        row += 1;
    }

    if (line) {
        free(line);
    }

    return 0;
}

void encrypt8bytes(char *str, int start, int end, char chunk[8],
                   char subKeys[16][48]) {
    char plainStr[8];
    // use \0 for padding
    memset(plainStr, 0, 8);
    memcpy(plainStr, str + start, end - start);
    DES_EncryptBlock(plainStr, subKeys, chunk);
}

void decrypt8bytes(char *str, int start, int end, char chunk[8],
                   char subKeys[16][48]) {
    char enStr[8];
    // use \0 for padding
    memset(enStr, 0, 8);
    memcpy(enStr, str + start, end - start);
    DES_DecryptBlock(enStr, subKeys, chunk);
}

char **do_encrypt(char** &plainText, char subKeys[16][48]) {
    char **enText = NULL;
    char* buf = (char*)malloc(5*BUF_SIZE*sizeof(char));
    char* chunk= (char*)malloc(8*sizeof(char));
    int size = 0;
    // encrypt each line
    int i = 0;

    while (plainText[i] != NULL) {
        int lsize = strlen(plainText[i]);
        // encrypt each 8 bytes in a line
        int count = 0;

        for (int start = 0; start < lsize; start = start + 8) {
            int end = start + 8 >= lsize ? lsize: start + 8;
	    memset(chunk,0,sizeof(chunk));
            encrypt8bytes(plainText[i], start, end, chunk, subKeys);
            // concatenate excluding the null-terminator
	    memcpy(buf+start,chunk,sizeof(chunk));
            count += 1;
        }

        // allocate space for encrypted line
        char *enLine = (char *)malloc(count * 8 * sizeof(char));
        memcpy(enLine, buf, count * 8);

        // resize enText
        if (i % BUF_SIZE == 0) {
            size = size + BUF_SIZE;
            resize(enText, size);
        }

        // assgin
        enText[i] = enLine;
        i++;
    }

    if (i == size) {
        size++;
        resize(enText, size);
    }

    enText[i] = NULL;
    free(buf);
    free(chunk);

    return enText;
}

char **do_decrypt(char** &enText, char subKeys[16][48]) {
    char **plainText = NULL;
    char* buf = (char*)malloc(5*BUF_SIZE*sizeof(char));
    char* chunk= (char*)malloc(8*sizeof(char));
    int size = 0;
    // encrypt each line
    int i = 0;

    while (enText[i] != NULL) {
        int lsize = strlen(enText[i]);
        // encrypt each 8 bytes in a line
        int count = 0;

        for (int start = 0; start < lsize; start = start + 8) {
	  memset(chunk,0,8);
            int end = start + 8 >= lsize ? lsize: start + 8;
            decrypt8bytes(enText[i], start, end, chunk, subKeys);
            // concatenate excluding the null-terminator
            memcpy(buf + start, chunk, sizeof(chunk));
            count += 1;
        }

        // allocate space for encrypted line
        char *plainLine = (char *)malloc(count * 8 * sizeof(char));
        memcpy(plainLine, buf, count * 8);

        // resize enText
        if (i % BUF_SIZE == 0) {
            size = size + BUF_SIZE;
            resize(plainText, size);
        }

        // assgin
        plainText[i] = plainLine;
        i++;
    }

    if (i == size) {
        size++;
        resize(plainText, size);
    }

    plainText[i] = NULL;

    return plainText;
}

void des_encrypt() {
    string plain_filepath;
    cout << "input file path:" << endl;
    cin >> plain_filepath;
    char **lines = loadfile(plain_filepath.c_str());

    while (!lines) {
        cout << "file doesn't exist!" << endl;
        cout << "input file path:" << endl;
        cin >> plain_filepath;
        lines = loadfile(plain_filepath.c_str());
    }

    int num;
    cout << "Select one method:" << endl;
    cout << "1) input key file path" << endl;
    cout << "2) input new key" << endl;
    cout << "input a number:" << endl;
    cin >> num;

    while (num < 1 || num > 2) {
        cout << "wrong number!" << endl;;
        cout << "input a number:" << endl;
        cin >> num;
        num = (int)num;
    }

    if (num == 1) {
        string key_path;
        char subKeys[16][48];
        cout << "input the key file path:" << endl;
        cin >> key_path;

        // check existence
        while (loadKeyFile(key_path, subKeys) < 0) {
            cout << "input the key file path" << endl;
            cin >> key_path;
        }

        // encrypt
        char** enLines = do_encrypt(lines, subKeys);
	// save
	string en_filepath ="en-"+plain_filepath;
	savefile(en_filepath.c_str(),enLines);
	free(lines);
	free(enLines);
    } else {
        // input new key
        string key;
        cout << "input a key:" << endl;
        cin >> key;

        while (key.size() > 8) {
            cout << "key length is longer than 8!" << endl;
            cin >> key;
        }

        if (key.size() < 8) {
            cout << "use \\0 as padding for key" << endl;
            key.append(8 - key.size(), '\0');
        }

        cout << "the key is:" << "'" << key << "'" << endl;
        // bytes to bits
        char bits_key[64];
        char key_str[8];
        copy(key.begin(), key.end(), key_str);
        Char8ToBit64(key_str, bits_key);
        // generate subkey
        char subKeys[16][48];
        DES_MakeSubKeys(bits_key, subKeys);
	// save subkey
	string keyFile = plain_filepath+".key";
	saveKeyFile(keyFile.c_str(),subKeys);
        // encrypt
        char** enLines = do_encrypt(lines, subKeys);
	// save
	string en_filepath ="en-"+plain_filepath;
	savefile(en_filepath.c_str(),enLines);
	free(enLines);
	free(lines);

    }
        cout << "\nencrypt done" << endl;
}

void des_decrypt() {
    string en_filepath;
    cout << "input file path:" << endl;
    cin >> en_filepath;
    char **lines = loadfile(en_filepath.c_str());

    while (!lines) {
        cout << "file doesn't exist!" << endl;
        cout << "input file path:" << endl;
        cin >> en_filepath;
        lines = loadfile(en_filepath.c_str());
    }

    int num;
    cout << "Select one method:" << endl;
    cout << "1) input key file path" << endl;
    cout << "2) input the key" << endl;
    cout << "input a number:" << endl;
    cin >> num;

    while (num < 1 || num > 2) {
        cout << "wrong number!" << endl;;
        cout << "input a number:" << endl;
        cin >> num;
        num = (int)num;
    }

    if (num == 1) {
        string key_path;
        char subKeys[16][48];
        cout << "input the key file path:" << endl;
        cin >> key_path;

        // check existence
        while (loadKeyFile(key_path, subKeys) < 0) {
            cout << "input the key file path" << endl;
            cin >> key_path;
        }
        // decrypt
        char** plainLines = do_decrypt(lines, subKeys);
	string plain_filepath = "plain-"+en_filepath;
	savefile(plain_filepath.c_str(),plainLines);
	free(lines);
	free(plainLines);
    } else {
        // input new key
        string key;
        cout << "input key:" << endl;
        cin >> key;

        while (key.size() > 8) {
            cout << "key length is longer than 8!" << endl;
            cin >> key;
        }

        if (key.size() < 8) {
            cout << "use \\0 as padding for key" << endl;
            cout << "input key:" << endl;
            key.append(8 - key.size(), '\0');
        }

        // bytes to bits
        char bits_key[64];
        char key_str[8];
        copy(key.begin(), key.end(), key_str);
        Char8ToBit64(key_str, bits_key);
        // generate subkey
        char subKeys[16][48];
        DES_MakeSubKeys(bits_key, subKeys);
        // decrypt
        char** plainLines = do_decrypt(lines, subKeys);
	string plain_filepath = "plain-"+en_filepath;
	savefile(plain_filepath.c_str(),plainLines);
	free(lines);
	free(plainLines);
    }
        cout << "\ndecrypt done" << endl;
}


int main(int argc, char **argv) {
    if (argc > 1) {
        char **lines = loadfile(argv[1]);

        if (!lines) {
            cerr << "empty file!" << endl;
            exit(1);
        }

        // print first 20 lines
        int i = 0;

        while (i < 20 && lines[i] != NULL) {
            cout << lines[i] << endl;
            i++;
        }

        free(lines);
    } else {
        while (1) {
            int num;
            cout << "Select:" << endl;
            cout << "1) encrypt one file" << endl;
            cout << "2) decrypt one encrypted file" << endl;
            cout << "input a number:" << endl;
            cin >> num;

            // check valid
            while (num < 1 || num > 2) {
                cout << "wrong number!" << endl;
                cout << "input a number:" << endl;
                cin >> num;
            }

            if (num == 1) {
                des_encrypt();
            } else {
                des_decrypt();
            }

        }
    }

    return 0;
}

