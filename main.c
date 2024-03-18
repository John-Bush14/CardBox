#include<stdio.h>
#include<stdbool.h>
#include<dirent.h>
#include"necessity.h"
#include"cJSON/cJSON.h"

const char* cardBoxes = "./cardBoxes";

void TODO() {printf("NOT DONE BRUV"); _Exit(0);}

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
      case 1: // importing box (searchpoint)
         // get files to import
         printf("Choose box to import:\n");
         char* UIFilePath = fileList(cardBoxes, "html"); printf("\n"); if (UIFilePath == NULL) {printf("fileList return null!"); return 0;}
         
         // seperate file into words
         char command[200*sizeof(char)]; 
         snprintf(command, sizeof(command), "awk -F 'lang-(nl|fr)\">' '{ for (i=2; i<=NF; i++) {split($i, a, \"<\"); print(a[1])}}' ./cardBoxes/%s", UIFilePath);
         
         FILE* output; char curWord[50*sizeof(char)]; cJSON* words = cJSON_CreateArray(); int wordslen = 0;
         output = popen(command, "r"); if (output==NULL) {printf("awk failed"); exit(0);}

         for (int i = 0; i<2; i++) {fgets(curWord, sizeof(curWord), output);} // skip first double cardset

         while (fgets(curWord, sizeof(curWord), output)) {
            cJSON* frNlArr = cJSON_CreateArray();
            if (frNlArr==NULL) {break;}
            
            for (int i = 0; i<2; i++) {
               printf(curWord);
               char *newline = strchr(curWord, '\n');
               if (newline != NULL) *newline = '\0';
               cJSON_AddItemToArray(frNlArr, cJSON_CreateString(curWord)); if (i == 1) {continue;}
               if (fgets(curWord, sizeof(curWord), output)==NULL) {break;} wordslen += 1;
            }

            cJSON_AddItemToArray(words, frNlArr);
         } pclose(output);

         // generate boilerplate
         cJSON* box = cJSON_CreateObject();

         cJSON_AddItemToObject(box, "todo", words);
         cJSON_AddItemToObject(box, "done", cJSON_CreateArray()); 
         cJSON_AddItemToObject(box, "completions", cJSON_CreateArray());
         cJSON_AddItemToObject(box, "previous", cJSON_CreateNumber(wordslen));
         cJSON_AddItemToObject(box, "next", cJSON_CreateNumber(0));
         cJSON_AddItemToObject(box, "offset", cJSON_CreateNumber(0));

         // write to savefile
         char name[50*sizeof(char)]; printf("\n\nname? "); scanf("%s", name); char svFilePath[100*sizeof(char)];
         snprintf(svFilePath, sizeof(svFilePath), "cardBoxes/%s.json", name);
         FILE* svFile = fopen(svFilePath, "w");  
         if (svFile == NULL) {printf("help"); cJSON_Delete(box); return 0;}

         fprintf(svFile, cJSON_Print(box));

         cJSON_Delete(box); fclose(svFile); if (UIFilePath!=NULL) {free(UIFilePath);}
         break;
         
      case 2: // learning box (searchpoint)
         // read savefile
         char* fileName = fileList(cardBoxes, "json"); if (fileName==NULL) {free(fileName); return 0;}      
         char filePath[200*sizeof(char)]; snprintf(filePath, sizeof(filePath), "cardBoxes/%s", fileName);

         FILE* svFilePtr = fopen(filePath, "r");

         fseek(svFilePtr, 0, SEEK_END);
         long fileSize = ftell(svFilePtr);
         fseek(svFilePtr, 0, SEEK_SET);

         char *svFileStr = (char *)malloc(fileSize + 1);
         fread(svFileStr, sizeof(char), fileSize, svFilePtr);
         svFileStr[fileSize] = '\0';

         fclose(svFilePtr);

         // parse file
         cJSON* cardbox = cJSON_Parse(svFileStr);



      case 3: // showing box (searchpoint)
         TODO(); break;
   
      default: _Exit(0);
}
} return 0;}
