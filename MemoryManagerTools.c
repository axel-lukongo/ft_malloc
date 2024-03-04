#include "MemoryManager.h"

/**
 * @brief this function allow me to get a page with the most available free space
 * for allocate a new page
 * 
 * @param page_family 
 * @return vm_page_t* 
 */
vm_page_t * find_page_to_insert(vm_page_family_t * page_family){
  if(!page_family->first_page)
    return NULL;

  int size_of_free_block = 0;
  vm_page_t * page_to_insert = page_family->first_page;
  block_meta_data_t *current_meta_block;

  for(vm_page_t *vm_page = page_family->first_page; vm_page; vm_page = vm_page->next){
    current_meta_block = (block_meta_data_t *)&vm_page->meta_block;

    for(;current_meta_block; current_meta_block=current_meta_block->next){
      if(current_meta_block->is_free && current_meta_block->size_of_block > size_of_free_block){
        size_of_free_block = current_meta_block->size_of_block;
        page_to_insert = vm_page;
      }
    }
  }

  return (page_to_insert);
}



/**
 * @brief this function it return the bigger free meta block of this page
 * 
 * @param vm_page
 * @return block_meta_data_t* 
 */
block_meta_data_t * get_biggest_free_meta_data_block(vm_page_t * vm_page){

  block_meta_data_t * current_meta_block;
  block_meta_data_t * to_return = (block_meta_data_t *)&(vm_page->meta_block);
  int size_of_free_block = 0;
  if(!vm_page)
    return NULL;
  for(current_meta_block = (block_meta_data_t *)&(vm_page->meta_block);
  current_meta_block;
  current_meta_block = current_meta_block->next){

      if(current_meta_block->is_free && current_meta_block->size_of_block > size_of_free_block){
        size_of_free_block = current_meta_block->size_of_block;
        to_return = current_meta_block;
      }
  }
  return to_return;
}



/**
 * Looks up a page family by its name.
 *
 * @param struct_name The name of the structure associated with the page family.
 * @return A pointer to the page family, or NULL if no matching page family was found.
 */
vm_page_family_t *lookup_page_family_by_name(char *struct_name){

    vm_page_family_t *vm_page_family_curr = NULL;
    vm_page_for_families_t *vm_page_for_families_curr = NULL;

    for(vm_page_for_families_curr = first_vm_page_for_families;
            vm_page_for_families_curr;
            vm_page_for_families_curr = vm_page_for_families_curr->next){

        ITERATE_PAGE_FAMILIES_BEGIN(first_vm_page_for_families, vm_page_family_curr){

            if(strncmp(vm_page_family_curr->name_of_struct,
                        struct_name,
                        32) == 0){

                return vm_page_family_curr;
            }
        } ITERATE_PAGE_FAMILIES_END(first_vm_page_for_families, vm_page_family_curr);
    }
    return NULL;
}



void mm_bind_block_for_allocation(block_meta_data_t *block_meta, block_meta_data_t *next_block_meta){
  if(block_meta->next){
    block_meta->next->prev = next_block_meta;
    next_block_meta->next = block_meta->next;
    next_block_meta->prev = block_meta;
    block_meta->next = next_block_meta;
  }
  else{
    next_block_meta->next = NULL;
    next_block_meta->prev = block_meta;
    block_meta->next = next_block_meta;
  }
}






/**
 * Checks if a virtual memory page is empty.
 *
 * @param vm_page The virtual memory page to check.
 * @return True if the page is empty, false otherwise.
 */
int mm_is_vm_page_empty(vm_page_t * vm_page){
  block_meta_data_t *block_meta = &vm_page->meta_block;

  ITERATE_VM_PAGE_META_BLOCK_BEGIN(vm_page, block_meta)
    if(block_meta && block_meta->is_free==FALSE)
      return FALSE;
  ITERATE_VM_PAGE_META_BLOCK_END(vm_page, block_meta)
  return TRUE;

  // if(vm_page->meta_block.is_free == TRUE &&
  // vm_page->meta_block.next == NULL &&
  // vm_page->meta_block.prev == NULL){
  //   return TRUE;
  // }
  // return FALSE;
}
