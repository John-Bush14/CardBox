#include<string.h>

#ifndef NECESSITY_H
#define NECESSITY_H

int negArrIndex(int len, int i) {if (i < 0) {return len+i;} return i;}

char* substr(char* str, int x, int y) {
   int slen = strlen(str); x = negArrIndex(slen, x); y = negArrIndex(slen, y);
   int substrLen = (y-x)+1; char substr[substrLen]; 
   
   if (substrLen-1 > slen) {printf("Substr longer then Str %s", str); return str;}
   memcpy(substr, &str[x], substrLen-1);
   
   substr[substrLen-1] = '\0'; printf("%s", substr);
   return substr;
}

#endif
