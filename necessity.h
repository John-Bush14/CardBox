#include<string.h>
#include<stdlib.h>

#ifndef NECESSITY_H
#define NECESSITY_H

int negArrIndex(int len, int i) {if (i < 0) {return len+i;} return i;}

// TEMPLATES/EXAMPLES
// char* name = (char*)malloc(size+1); if (name == NULL) {return NULL;}

char* substr(char* str, int x, int y) {
   int slen = strlen(str); x = negArrIndex(slen+1, x); y = negArrIndex(slen+1, y);
   int substrLen = (y-x); 

   char* substr = (char*)malloc((substrLen+1)*sizeof(char)); if (substr == NULL) {return NULL;}
   
   if (substrLen > slen) {free(substr); return NULL;}
   memcpy(substr, &str[x], substrLen);
   
   substr[substrLen] = '\0';
   return substr;
}

void printRaw(const char *str) {
    while (*str) {
        if (*str == '\n') {
            printf("\\n");
        } else if (*str == '\t') {
            printf("\\t");
        } else {
            printf("%c", *str);
        }
        str++;
    }
    printf("\n");
}

int max(int num1, int num2) {
    if (num1 > num2) {
        return num1;
    } else {
        return num2;
    }
}
int min(int num1, int num2) {
    if (num1 < num2) {
        return num1;
    } else {
        return num2;
    }
}

char* read(char* filePath) {
   FILE* filePtr = fopen(filePath, "r");

   fseek(filePtr, 0, SEEK_END);
   long fileSize = ftell(filePtr);
   fseek(filePtr, 0, SEEK_SET);

   char *fileStr = (char *)malloc(fileSize + 1);
   fread(fileStr, sizeof(char), fileSize, filePtr);
   fileStr[fileSize] = '\0';

   fclose(filePtr); return fileStr;
}
#endif
