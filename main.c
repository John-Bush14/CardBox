#include<stdio.h>
#include<stdbool.h>
#include<dirent.h>
#include"necessity.h"
#include<cjson/cJSON.h>

const char* cardBoxes = "./cardBoxes";

void todo() {printf("NOT DONE BRUV"); _Exit(0);}

char* fileList(char* folder, char* ext) {
   DIR *d; struct dirent *dir; d = opendir(folder); if (!d) {printf("help!");}
   int temp = 0; char* sub; char* files[50]; int fileIndex; char* file = malloc(101*sizeof(char)); if (file == NULL) {free(file); return NULL;}
   if (file == NULL) {printf("file malloc failed!"); closedir(d); return NULL;}

   while ((dir = readdir(d)) != NULL) { if (dir->d_name == NULL) {continue;}
   sub = substr(dir->d_name,-5, -1);
   
   if (sub != NULL && strcmp(sub, ext) == 0) { files[temp] = dir->d_name;
      printf("%i. %s\n", temp, dir->d_name); temp += 1;} 

   if (sub == NULL) {free(sub);}} closedir(d); 
   
   scanf("%i", &fileIndex); if (fileIndex >= temp) {free(file);}
   else {file = files[fileIndex];}
   if (file != NULL) {return file;} return NULL;
}

int main() { while (true) {
   printf("1. Import box, 2. Learn box or 3. See progress\n"); int action; scanf("%i", &action);

   switch (action) {
      case 1: 
         printf("Choose box to import:\n");
         char* file = fileList(cardBoxes, "html"); printf("\n"); if (file == NULL) {printf("fileList return null!"); return 0;}
         
         char command[110*sizeof(char)+sizeof(file)];
         snprintf(command, sizeof(command), "awk -F 'lang-(nl|fr)\">' '{ for (i=2; i<=NF; i++) {split($i, a, \"<\"); print(a[1])}}' ./cardBoxes/%s", file);
         
         char* nlWords[500]; char* frWords[500]; FILE* output; int temp = -2; int index = 0;
         char curWord[100]; char* words; output = popen(command, "r"); if (output==NULL) {printf("awk failed"); exit(0);}

         while (fgets(curWord, sizeof(curWord)-1, output)!=NULL) {if (temp>=0) { switch (abs(temp % 2)) {
            case 0: frWords[index] = (char*)malloc((strlen(curWord)+1)*sizeof(char)); strcpy(frWords[index], curWord); break;
            case 1: nlWords[index] = (char*)malloc((strlen(curWord)+1)*sizeof(char)); strcpy(nlWords[index], curWord); index++; break;
         }}temp++;} 
         

         break;
         
      case 2:
         todo(); break;

      case 3:
         todo(); break;
   
      default: _Exit(0);
   }
} return 0;}
