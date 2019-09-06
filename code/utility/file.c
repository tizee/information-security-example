#include "file.h"
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
