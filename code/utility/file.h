#ifndef FILE_H
#define FILE_H
#define BUF_SIZE 128
void resize(char **&pts,int &size);
int savefile(const char* filename,char **lines);
char** loadfile(const char* filename);
#endif //FILE_H