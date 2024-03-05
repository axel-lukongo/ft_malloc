// #include "uapi_mm.h"
#include "MemoryManager.h"

int main(){

  char *test1 = malloc(4000);
  void *test2 = malloc(1990);
  void *test3 = malloc(4000);
  void *test4 = malloc(4000);
  void *test5 = malloc(4008);
  void *test6 = malloc(400);
  void *test7 = malloc(408);
  void *test8 = malloc(984);
  void *test9 = malloc(84);
  char *emp= malloc(5);
  show_alloc_mem();
  ft_printf("-\n-\n");
  for(int i = 0; i < 5; i++){
    emp[i] = i + '0';
  }
  ft_printf("emp: %s\n", emp);
  ft_printf("-\n-\n");
  ft_free(emp);
  ft_free(test2);
  ft_free(test1);
  ft_free(test3);
  show_alloc_mem();
  ft_printf("-\n-\n");
  ft_free(test4);
  ft_free(test5);
  show_alloc_mem();
  ft_printf("-\n-\n");
  ft_free(test6);
  ft_free(test7);
  ft_free(test8);
  ft_free(test9);
  show_alloc_mem();
  return 0;

}