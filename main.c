#include<stdio.h>
#include<stdbool.h>
#include<dirent.h>
#include"unrelated_functions.h"
#include"cJSON/cJSON.h"
#include<time.h>
#include<pthread.h>
#include<plplot/plplot.h>

const char* cardBoxes = "./cardBoxes";
const int SET_SIZE = 20;
const int RANDOMNESS = 10;

void TODO() {printf("NOT DONE BRUV"); _Exit(0);}

char* fileList(char* folder, char* filterExt) {
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

void plotProgress(cJSON* cJSON_Save) {     
   plsdev("qtwidget");
   plinit();
   
   // End PLplot
   plend();

   free(cJSON_Save); pthread_exit(NULL);
}

int main() { while (true) {
   printf("1. Import box, 2. Learn box or 3. See progress\n"); int action; scanf("%i", &action);
   pthread_t plotThread;

   switch (action) {
      case 1: // importing box (searchpoint)

         // get files to import
         printf("Choose box to import:\n");
         char* UIFilePath = fileList(cardBoxes, "html"); printf("\n"); if (UIFilePath == NULL) {printf("fileList return null!"); return 0;}
                  
         // seperate file into words
         char splitCommand[200*sizeof(char)]; 
         snprintf(splitCommand, sizeof(splitCommand), "awk -F 'lang-(nl|fr)\">' '{ for (i=2; i<=NF; i++) {split($i, a, \"<\"); print(a[1])}}' ./cardBoxes/%s", UIFilePath);
         

         FILE* output; output = popen(splitCommand, "r"); if (output==NULL) {printf("awk failed"); exit(0);}

         cJSON* words = cJSON_CreateArray(); int wordCount = 0;


         char word[50*sizeof(char)]; 

         for (int i = 0; i<2; i++) {fgets(word, sizeof(word), output);} // skip first double cardset

         while (fgets(word, sizeof(word), output)) {
            cJSON* frNlArr = cJSON_CreateArray();
            if (frNlArr==NULL) {break;}
            
            for (int i = 0; i<2; i++) {
               printf(word);
               char *newline = strchr(word, '\n');
               if (newline != NULL) *newline = '\0';
               cJSON_AddItemToArray(frNlArr, cJSON_CreateString(word)); if (i == 1) {continue;}
               if (fgets(word, sizeof(word), output)==NULL) {break;} wordCount += 1;
            }

            cJSON_AddItemToArray(words, frNlArr);
         } pclose(output);

         // generate boilerplate
         cJSON* box = cJSON_CreateObject();

         cJSON_AddItemToObject(box, "todo", words);
         cJSON_AddItemToObject(box, "done", cJSON_CreateArray()); 
         cJSON_AddItemToObject(box, "completions", cJSON_CreateArray());
         cJSON_AddItemToObject(box, "previous", cJSON_CreateNumber(wordCount));
         cJSON_AddItemToObject(box, "next", cJSON_CreateNumber(0));
         cJSON_AddItemToObject(box, "offset", cJSON_CreateNumber(0));

         // write to savefile
         char name[50*sizeof(char)]; printf("\n\nname? "); scanf("%s", name); char svFilePath[100*sizeof(char)];
         snprintf(svFilePath, sizeof(svFilePath), "cardBoxes/%s.json", name);
         FILE* svFile = fopen(svFilePath, "w");  
         if (svFile == NULL) {printf("help"); cJSON_Delete(box); return 0;}

         fprintf(svFile, cJSON_Print(box));

         cJSON_Delete(box); fclose(svFile); 
         break;
         
      case 2: // learning box (searchpoint)
         char* SVFile = fileList(cardBoxes, "json"); if (SVFile==NULL) {free(SVFile); return 0;}      
         char SVFilePath[200*sizeof(char)]; snprintf(SVFilePath, sizeof(SVFilePath), "cardBoxes/%s", SVFile);

         char* SVFileStr = read(SVFilePath);
         // parse file
         cJSON* cardbox = cJSON_Parse(SVFileStr); 

         cJSON* todo = cJSON_GetObjectItemCaseSensitive(cardbox, "todo");
         cJSON* done = cJSON_GetObjectItemCaseSensitive(cardbox, "done");
         cJSON* next = cJSON_GetObjectItemCaseSensitive(cardbox, "next");
         cJSON* previous = cJSON_GetObjectItemCaseSensitive(cardbox, "previous");
         cJSON* offset = cJSON_GetObjectItemCaseSensitive(cardbox, "offset");
         cJSON* completions = cJSON_GetObjectItemCaseSensitive(cardbox, "completions");

         // checking for completion
         if (previous->valueint == 0) {
            previous->valueint = next->valueint; next->valueint = 0;
            offset->valueint = offset->valueint + 1;
            
            while (cJSON_GetArraySize(done) > 0) {
               cJSON_AddItemToArray(todo, cJSON_DetachItemFromArray(done, 0));
            }
         } 
         
         // learning file
         int sizeTarget = max(cJSON_GetArraySize(todo)-SET_SIZE, 0);
         double correct = 0; double wrong = 0;

         while (cJSON_GetArraySize(todo) > sizeTarget) {
            srand(time(NULL));
            cJSON* set = cJSON_GetArrayItem(todo, rand()%min(cJSON_GetArraySize(todo), RANDOMNESS));
            cJSON* fr = cJSON_GetArrayItem(set, 0);
            cJSON* nl = cJSON_GetArrayItem(set, 1);

            printf("\033[H\033[J \n%s\n\n\n", cJSON_Print(nl));
            char* answer; answer = scanfn();

            if (strcmp(answer, fr->valuestring) != 0) {
               printf("wrong : %s\n\n", fr->valuestring);
               
               char* delay = scanfn();
               if (strlen(delay) == 0) {
                  cJSON_AddItemToArray(todo, cJSON_DetachItemFromArray(todo, 0));
                  
                  if (wrong+correct < SET_SIZE) {wrong += 1;}

                  continue;
               }
            }
            if (wrong+correct < SET_SIZE) {correct += 1;}
            
            printf("\ncorrect: %s\n\n", fr->valuestring); scanfn();
            cJSON_AddItemToArray(done, cJSON_DetachItemFromArray(todo, 0));

            previous->valueint = previous->valueint-1;
            next->valueint = next->valueint+1;
            
            previous->valuedouble = previous->valueint; next->valuedouble = next->valueint; offset->valuedouble = offset->valueint;
         }
         cJSON* completion = cJSON_CreateArray();  
         cJSON_AddItemToArray(completion, cJSON_CreateNumber(correct/SET_SIZE));   
         
         time_t now = time(NULL) ;
         struct tm curTime ;
         localtime_r(&now, &curTime) ;
         char ftime[100] ;
         strftime(ftime, sizeof(ftime), "%Y%m%d%H%M%S", &curTime) ;
         cJSON_AddItemToArray(completion, cJSON_CreateString(ftime));

         cJSON_AddItemToArray(completions, completion);

         FILE* svFilePtr = fopen(SVFilePath, "w");
         fprintf(svFilePtr, cJSON_Print(cardbox));
         fclose(svFilePtr);

         cJSON_Delete(cardbox); break;

      case 3: // showing progress (searchpoint)
         char* SVFileName = fileList(cardBoxes, "json"); if (SVFileName==NULL) {free(SVFileName); return 0;}      
         char FilePath[200*sizeof(char)]; snprintf(FilePath, sizeof(FilePath), "cardBoxes/%s", SVFileName);
         cJSON* cJSON_Save = malloc(sizeof(cJSON)); if (cJSON_Save == NULL) {return 0;} 
         cJSON_Save = cJSON_Parse(read(FilePath));
         pthread_create(&plotThread, NULL, plotProgress, cJSON_Save);

         break;
   
      default: pthread_join(plotThread, NULL); _Exit(0); 
   }
} return 0;}
