#include "MemoryManager.h"


/**
 * This function frees a list of blocks in the memory manager.
 *
 * @param block_to_free A pointer to the first block_meta_data_t in the list of blocks to free.
 * @return A pointer to the head of the list of free blocks, or NULL if there was an error.
 */
block_meta_data_t * mm_free_blocks(block_meta_data_t * block_to_free){
  block_meta_data_t * return_block = NULL;
  if(block_to_free->is_free == FALSE){
    vm_page_t *page = MM_GET_PAGE_FROM_META_BLOCK(block_to_free);
    // vm_page_family_t * page_family = page->page_family;
    ft_memset((char *) (block_to_free + 1), 0,
    block_to_free->size_of_block);
    return_block = block_to_free;
    return_block->is_free = TRUE;
    block_meta_data_t * next_block = NEXT_META_BLOCK(block_to_free);
    if(next_block){
      block_to_free->size_of_block = mm_get_hard_internal_memory_frag_size(block_to_free, next_block);
    }
    else{
      char * end_addr_of_vm_page = (char *)((char *)page + SYSTEM_PAGE_SIZE);
      char * end_addr_of_free_data_block = (char*)((char *) block_to_free+1 + block_to_free->size_of_block);
      int internal_mem_frag = (int)((unsigned long)end_addr_of_vm_page - (int)(unsigned long)end_addr_of_free_data_block);
      block_to_free->size_of_block += internal_mem_frag;
    }

    //mergin
    if(next_block && next_block->is_free == TRUE){
      mm_union_free_block(block_to_free, next_block);
      return_block = block_to_free;
    }
    block_meta_data_t * prev_block = PREV_META_BLOCK(block_to_free);
    if(prev_block && prev_block->is_free == TRUE){
      mm_union_free_block(prev_block, block_to_free);
      return_block = prev_block;
    }
    if(mm_is_vm_page_empty(page)){
      free_and_delete_mv_page(page);
      return NULL;
    }
    return return_block;
  }
  return NULL;
}


void free(void *ptr){
  block_meta_data_t *meta_data =(block_meta_data_t *)((char *)ptr - sizeof(block_meta_data_t));
  if(ptr && meta_data->is_free == FALSE){
    mm_free_blocks(meta_data);
  }
}