#include "ArchiverAndDearchiver.h"

void tree(char* basePath, const int root) {
    int i;
    char path[1000];
    struct dirent* dp;
    DIR* dir = opendir(basePath);
    if (!dir) return;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            strcpy_s(path, sizeof(path), basePath);
            strcat_s(path, sizeof(path), "\\");
            strcat_s(path, sizeof(path), dp->d_name);
            printf("%s\n", path);
            //гдеяэ лнфмн гюосяйюрэ йндхпнбюмхе тюикю
            tree(path, root + 2);
        }
    }
    closedir(dir);
}