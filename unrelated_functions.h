#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<dirent.h>

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

char* scanfn() { // ends scan when user newlines
   static char input[100]; // Assuming input line won't exceed 100 characters

   fgets(input, sizeof(input), stdin);
      
//  if (input[0] == '\n') {fgets(input, sizeof(input), stdin);}


   // Remove trailing newline character if present
   input[strcspn(input, "\n")] = '\0';
   

   return input;
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

char* fileChoice(char* folder, char* filterExt) {
   DIR *dir = opendir(folder); if (!dir) {printf("help!");}

   int fileIndex = 0; char* files[50];

   char* file = malloc(101*sizeof(char));
   if (file == NULL) {printf("file malloc failed!"); closedir(dir); return NULL;}

   struct dirent *handle; char* ext;
   while ((handle = readdir(dir)) != NULL) {
      ext = substr(handle->d_name,-5, -1);
   
      if (ext != NULL && strcmp(ext, filterExt) == 0) { 
         files[fileIndex] = handle->d_name;
         printf("%i. %s\n", fileIndex, handle->d_name); fileIndex += 1;
      } 
   
      if (ext == NULL) {free(ext);}
   } closedir(dir); 

   int chosenFile; 
   scanf("%i", &chosenFile);

   if (chosenFile >= fileIndex && file != NULL) {free(file);}
   else {file = files[chosenFile];}
   if (file != NULL) {return file;} return NULL;
}
#endif
