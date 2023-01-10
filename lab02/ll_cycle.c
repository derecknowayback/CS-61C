#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
   node *tortoise = head;
   node *hare = head;                                                                 
   while (hare != NULL) {
       tortoise = tortoise->next;
       if(hare->next)
        hare = hare->next->next;
       else
        return 0; 
       if (tortoise == hare) {
           return 1;
       }
   }
   return 0;
}