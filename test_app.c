// #include "uapi_mm.h"
#include "MemoryManager.h"

int main(){

  char *test1 = ft_malloc(4000);
  void *test2 = ft_malloc(1990);
  void *test3 = ft_malloc(4000);
  void *test4 = ft_malloc(4000);
  void *test5 = ft_malloc(4008);
  char *emp= ft_malloc(5);
  show_alloc_mem();
  ft_printf("\n-\n-\n");
  for(int i = 0; i < 5; i++){
    emp[i] = i + '0';
  }
  ft_printf("emp: %s\n", emp);
  ft_printf("\n-\n-\n");
  ft_free(emp);
  ft_free(test2);
  ft_free(test1);
  ft_free(test3);
  show_alloc_mem();
  ft_printf("\n-\n-\n");
  ft_free(test4);
  ft_free(test5);
  show_alloc_mem();
  return 0;

}