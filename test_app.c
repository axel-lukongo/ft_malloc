// #include "uapi_mm.h"
#include "MemoryManager.h"

/**
 * il faudra que je verifie demain dans quelle vm_page le block 4 se trouve
 * car lorsque je le free le realloc segfault donc cela signifie que cette page
 * a ete retourne au kernel mais que le pointeur n'a pas ete mis a null
 * donc pour le realloc lorsque je cherche le plus grand free, cela me fais
 * depasser dans ma boucle et donc je segfault
 * 
 * @return int 
 */


int main(){

  char *test1 = malloc(70);
  void *test2 = malloc(1990);
  void *test3 = malloc(4000);
  void *test4 = malloc(4000);
  void *test5 = malloc(4007);
  void *test6 = malloc(400);
  void *test7 = malloc(408);
  void *test8 = malloc(984);
  char *test9;
  for(int i = 0; i < 5; i++){
    test9 = malloc(84);
  }
  // test9[0] = 'A';
  // char *emp= malloc(5);
  show_alloc_mem();
  // ft_printf("-\n-\n");
  // for(int i = 0; i < 40; i++){
  //   test9[i] = 'A';
  // }
  // ft_printf("test9: %s\n", test9);
  ft_printf("-\n-\n");
  // free(emp);
  free(test2);
  free(test1);
  // ft_printf("test1: %s\n", test1);

  free(test3);
  show_alloc_mem();
  ft_printf("-\n-\n");
  free(test4);
  // free(test5);
  test6 = realloc(test6, 300);
  // show_alloc_mem();
  // ft_printf("-\n-\n");
  // free(test6);
  // free(test7);
  // free(test8);
  // free(test9);
  // show_alloc_mem();
  return 0;
}




