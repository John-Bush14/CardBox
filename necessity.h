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

#endif
