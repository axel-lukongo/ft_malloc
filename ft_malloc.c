#include "MemoryManager.h"

/**
 * @brief 
 * 
 * step 1: we check if we already have a page family corresponding to the structure name
 * 
 * step 2: if the size reclam is too big
 * 
 * step 3: i allocate the memory request
 * @param struct_name the name of the structure 
 * @param units the size that we want allocate
 * @return void* 
 */
void * ft_malloc(size_t size){
  //step 1:
  if (SYSTEM_PAGE_SIZE == 0){
      mm_init();
      MM_REG_STRUCT(emp_t);
  }

  vm_page_family_t *pg_family = lookup_page_family_by_name("emp_t");

  // step 2:
  if ((size + sizeof(block_meta_data_t)) > mm_max_page_allocatable_memory(1)){
    ft_printf("Error: Memory requested exceeds page size\n");
    return NULL;
  }

  //step 3:
  block_meta_data_t * free_block_meta_data = NULL;
  free_block_meta_data = mm_allocate_free_data_block(pg_family, size);

  if (free_block_meta_data){
    // ft_memset((char *) (free_block_meta_data + 1), 0,
    // free_block_meta_data->size_of_block);
    return (char *)(free_block_meta_data + 1);
  }
  return NULL;
}