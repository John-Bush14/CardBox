#include<stdio.h>
#include<stdbool.h>
#include<dirent.h>
#include"necessity.h"
#include"cJSON/cJSON.h"

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
   
   scanf("%i", &fileIndex); if (fileIndex >= temp && file != NULL) {free(file);}
   else {file = files[fileIndex];}
   if (file != NULL) {return file;} return NULL;
}

int main() { while (true) {
   printf("1. Import box, 2. Learn box or 3. See progress\n"); int action; scanf("%i", &action);

   switch (action) {
      case 1: 
         printf("Choose box to import:\n");
         char* file = fileList(cardBoxes, "html"); printf("\n"); if (file == NULL) {printf("fileList return null!"); return 0;}
         printf("szz");
         char command[200*sizeof(char)];
         snprintf(command, sizeof(command), "awk -F 'lang-(nl|fr)\">' '{ for (i=2; i<=NF; i++) {split($i, a, \"<\"); print(a[1])}}' ./cardBoxes/%s", file);
         printf("ddd");
         FILE* output; char curWord[30*sizeof(char)]; cJSON* words = cJSON_CreateArray();
         output = popen(command, "r"); if (output==NULL) {printf("awk failed"); exit(0);}
         printf("plsgh");
         while (fgets(curWord, sizeof(curWord), output)) {
            cJSON* frNlArr = cJSON_CreateArray();
            if (frNlArr==NULL) {break;}
            
            for (int i = 0; i<2; i++) {
               char *newline = strchr(curWord, '\n');
               if (newline != NULL) *newline = '\0';
               cJSON_AddItemToArray(frNlArr, cJSON_CreateString(curWord)); 
               if (fgets(curWord, sizeof(curWord), output)==NULL) {break;}
            }

            cJSON_AddItemToArray(words, frNlArr);
         } printf("help"); printf("%s", cJSON_Print(words)); cJSON_Delete(words);
 
         break;
         
      case 2:
         todo(); break;

      case 3:
         todo(); break;
   
      default: _Exit(0);
}
} return 0;}
