#include<stdio.h>
#include<stdbool.h>

void todo() {printf("NOT DONE BRUV"); _Exit(0);}

int main() { while (true) {
   printf("1. Import box, 2. Learn box or 3. See progress\n"); char action; scanf("%c", &action);

   switch (action) {
      case '1': 
         todo();

      case '2':
         todo();

      case '3':
         todo();
   }
}}
