#include "MemoryManager.h"

/**
* Reallocates memory for the given pointer and new size.
*
* @param ptr The pointer to the memory block to reallocate.
* @param new_size The new size of the memory block.
* @return A pointer to the reallocated memory block.
*/
void * ft_realloc(void *ptr, int new_size){

  block_meta_data_t *meta_data =(block_meta_data_t *)((char *)ptr - sizeof(block_meta_data_t));
  void * new_alloc;

  if(ptr == NULL){
    return malloc(new_size);
  }
  else if(ptr != NULL && new_size == 0){
    ft_free(ptr);
  }
  else if(new_size > 0){
    new_alloc = malloc(new_size);
    if(new_size > meta_data->size_of_block)
      ft_memcpy(new_alloc, ptr, meta_data->size_of_block);
    else
      ft_memcpy(new_alloc, ptr, new_size);
    ft_free(ptr);
    return new_alloc;
  }
  return NULL;
}
