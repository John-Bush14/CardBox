#include<stdio.h>
#include<stdbool.h>
#include<dirent.h>
#include"necessity.h"

void todo() {printf("NOT DONE BRUV"); _Exit(0);}

int main() { while (true) {
   printf("1. Import box, 2. Learn box or 3. See progress\n"); int action; scanf("%i", &action);

   switch (action) {
      case 1: 
         DIR *d; struct dirent *dir; d = opendir("./cardBoxes"); if (!d) {printf("help!");} 

         printf("Choose box to import:\n"); int temp = 0;
         while ((dir = readdir(d)) != NULL) { if (substr(dir->d_name,-5, -1) == "html") {
            printf("%i. %s\n", temp, dir->d_name); temp += 1;}} closedir(d);
         
      case 2:
         todo();

      case 3:
         todo();
   }
}}
