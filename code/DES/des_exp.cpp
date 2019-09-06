#include<iostream>
using namespace std;

#include "DES.cpp"

string encryptBlock(char key[8],char str[8]){
  // store encrypted text
  char cipherBlock[8];
  // bytes to bits
  char bits_key[64];
  Char8ToBit64(key,bits_key);
  // generate subkey
  char subKeys[16][48];
  DES_MakeSubKeys(bits_key,subKeys);
  // encrypt
  DES_EncryptBlock(str,subKeys,cipherBlock);
  return string(cipherBlock);
}

string decryptBlock(char key[8],char en_str[8]){
  char plainBlock[8];
  // bytes to bits
  char bits_key[64];
  Char8ToBit64(key,bits_key);
  // generate key
  char subKeys[16][48];
  DES_MakeSubKeys(bits_key,subKeys);
  DES_DecryptBlock(en_str,subKeys,plainBlock);
  return string(plainBlock);
}

int main(int argc,char** argv){
  // input key
  char key[8];
  cout<<"input a key string:"<<endl;	
  cin>>key;
  cout<<"the key is:"<<key<<endl;
  cout<<"charr array length:"<<strlen(key)<<endl;
  if(strlen(key)!=8){
    cout<<"key is too short or too long\nkey should be 8-length"<<endl;	
    exit(0);
  }
  // input string to encrypted
  char str[8];
  cout<<"input a plain block:"<<endl;	
  cin>>str;
  if(strlen(str)<8){
    // zero-padding
    for(int i=strlen(str);i<8;i++){
      str[i] = '\0';
    }
    cout<<"use \\0 for padding, the plain block is:";
    for(int i = 0;i<strlen(str);i++){
      cout<<str[i];
    }
    cout<<endl;
  }else{
    cout<<"the plain block is:"<<str<<endl;
  }

  string en_block = encryptBlock(key,str);
  cout<<"encrypted block is:"<<"'"<<en_block<<"'"<<endl;
  cout<<"length of en_block is:"<<en_block.size()<<endl;
  char* en_char = new char[8];
  memset(en_char,0,8);
  std::copy(en_block.begin(),en_block.end(),en_char);
  /* en_char[en_block.size()]='\0'; */
  string plain_block = decryptBlock(key,en_char);
  cout<<"decrypted block is:"<<"'"<<plain_block<<"'"<<endl;
  cout<<"length of plain block is:"<<plain_block.size()<<endl;
  delete []en_char;

  
  return 0;
}
