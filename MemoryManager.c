#include "MemoryManager.h"
// #include "uapi_mm.h"

/** The size of a system page. */
size_t SYSTEM_PAGE_SIZE = 0;



/** A pointer to the most recently requested page from the kernel. */
vm_page_for_families_t * first_vm_page_for_families = NULL; //this variable will always pointe on the most recently page request by the LLM from the kernel


/**
 * 
 * Initializes the memory manager by setting the system page size.
 */
void mm_init(){
  SYSTEM_PAGE_SIZE = getpagesize();
}




/**
 * Allocates a new virtual memory page from the kernel.
 *
 * @param units The number of system pages to allocate.
 * @return A pointer to the allocated memory, or NULL if the allocation failed.
 */
static void * mm_get_new_vm_page_from_kernel(int units){
  char * vm_page = mmap(0, units * SYSTEM_PAGE_SIZE, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_ANON|MAP_PRIVATE,0,0);

  if(vm_page == MAP_FAILED){
    ft_printf("mm_get_new_vm_page_from_kernel failed\n");
    return NULL;
  }
  ft_memset(vm_page, 0, units * SYSTEM_PAGE_SIZE);
  return (void *)vm_page;
}




/**
 * Returns a virtual memory page to the kernel.
 *
 * @param vm_page The virtual memory page to return.
 * @param units The number of system pages to return.
 */
static void mm_return_vm_page_to_kernel(void * vm_page, int units){
  if(munmap(vm_page, units * SYSTEM_PAGE_SIZE)){
    ft_printf("ERROR: could not mummap the vm page");
  }
}



/**
 
 * Instantiates a new page family and adds it to the list of page families.

 * step one: if this page is the first one;

 * step two: if a page already exists, iterate this page until the end
 *           and vm_page_family_curr will point to the family next to the last insertion

 * step tree: if the current page is already full, reclaim an other one

 * @param name_of_struct The name of the structure associated with this page family.
 * @param size_of_struct The size of the structure associated with this page family.
*/
void mm_instantiate_new_page_family(char * name_of_struct, uint32_t size_of_struct){
  vm_page_family_t * vm_page_family_curr = NULL;
  vm_page_for_families_t * new_vm_page_for_families_curr = NULL;

  if(size_of_struct > SYSTEM_PAGE_SIZE){
    ft_printf("ERROR: we don't this memory size");
    return;
  }

  // step one
  if(first_vm_page_for_families == NULL){
    first_vm_page_for_families = (vm_page_for_families_t *)mm_get_new_vm_page_from_kernel(1);
    first_vm_page_for_families->next = NULL;
    strncpy(first_vm_page_for_families->vm_page_family[0].name_of_struct, name_of_struct, 32);
    first_vm_page_for_families->vm_page_family[0].size_of_struct = size_of_struct;
    first_vm_page_for_families->vm_page_family[0].page_to_insert = NULL;
    return ;
  }

  // step two
  uint32_t counter = 0;
  ITERATE_PAGE_FAMILIES_BEGIN(first_vm_page_for_families, vm_page_family_curr){

    if((strncmp(vm_page_family_curr->name_of_struct, name_of_struct, 32) != 0) || 
    (counter <= 0 && strncmp(vm_page_family_curr->name_of_struct, name_of_struct, 32) == 0)){
      counter++;
      continue;
    }
    assert(0);
  } ITERATE_PAGE_FAMILIES_END(first_vm_page_for_families, vm_page_family_curr);

  // step tree
  if(counter == MAX_FAMILY_PER_VM_PAGE){
    new_vm_page_for_families_curr = (vm_page_for_families_t *)mm_get_new_vm_page_from_kernel(1);
    new_vm_page_for_families_curr->next = first_vm_page_for_families;
    first_vm_page_for_families = new_vm_page_for_families_curr;
    vm_page_family_curr = &first_vm_page_for_families->vm_page_family[0];
  }

  strncpy(vm_page_family_curr->name_of_struct, name_of_struct, 32);
  vm_page_family_curr->size_of_struct = size_of_struct;
  vm_page_family_curr->first_page = NULL;
}




//that return me the size of a free data_block of an empty vm page
uint32_t mm_max_page_allocatable_memory(int units){
  return (uint32_t)((SYSTEM_PAGE_SIZE * units) - offset_of(vm_page_t, page_memory));
}



/**
 * Allocates a new virtual memory page and returns a pointer to it.
 *
 * @param vm_page_family The page family to allocate the page for.
 * @return A pointer to the allocated virtual memory page.
 */
vm_page_t * allocate_vm_page(vm_page_family_t * vm_page_family){

  vm_page_t * vm_page = mm_get_new_vm_page_from_kernel(1);

  MARK_PAGE_EMPTY(vm_page);

  vm_page->meta_block.size_of_block = mm_max_page_allocatable_memory(1);
  vm_page->meta_block.is_free = TRUE;

  vm_page->meta_block.offset = offset_of(vm_page_t, meta_block); 

  vm_page->next = NULL;
  vm_page->prev = NULL;

  vm_page->page_family = vm_page_family;
  vm_page_family->page_to_insert = vm_page;

  if (!vm_page_family->first_page){
    vm_page_family->first_page = vm_page;
    return vm_page;
  }
  vm_page->next = vm_page_family->first_page;
  vm_page->next->prev = vm_page;
  vm_page_family->first_page = vm_page;
  return vm_page;
}




/**
 * Merges two adjacent free blocks in a virtual memory page.
 *
 * @param first The first free block to merge.
 * @param second The second free block to merge.
 */
void mm_union_free_block(block_meta_data_t *first, block_meta_data_t *second){
  if(first->is_free == TRUE && second->is_free == TRUE) {
    first->size_of_block = sizeof(block_meta_data_t) + second->size_of_block;
    first->next = second->next;
    if(second->next){
      second->next->prev = first;
    }
  }
}



/**
 * @brief Frees a virtual memory page and returns it to the kernel.
 * 
 * case one: if we delete the first page.
 * 
 * case two: if we delete page who are not the first.
 * 
 * @param struct_name The name of the structure associated with the page family.
 * @return A pointer to the page family, or NULL if no matching page family was found.
 */
void free_and_delete_mv_page(vm_page_t * vm_page){

  vm_page_family_t * family = vm_page->page_family;

  //case one
  if (family->first_page == vm_page){
    family->first_page = vm_page->next;
    if(vm_page->next){
      vm_page->next->prev = NULL;
    }
    vm_page->next = NULL;
    vm_page->prev = NULL;
    mm_return_vm_page_to_kernel((void *)vm_page, 1);
    return ;
  }

  //case two
  if(vm_page->next){
    vm_page->next->prev = vm_page->prev;
  }
  vm_page->prev->next = vm_page->next;
  mm_return_vm_page_to_kernel((void *)vm_page, 1);
}



/**
* Splits a free data block in a page family to allocate a new block.
*
* case 1: if theire is no vm page yet (that mean i will create it) or
*         theire not enought space to satisfate the size request.
*
* case 2: if we have enought space to satisfy the request in our free meta_data.
*
* @param vm_page_family 
* @param size 
* @return block_meta_data_t* 
*/
static int mm_split_free_data_block_for_allocation(vm_page_family_t *vm_page_family, block_meta_data_t *meta_data, uint32_t size){

  block_meta_data_t * next_block_meta = NULL;

  if(meta_data->is_free != TRUE)
    return FALSE;

  if(meta_data->size_of_block < size)
    return FALSE;

  uint32_t remaning_size = meta_data->size_of_block - size;
  meta_data->is_free = FALSE;
  meta_data->size_of_block = size;

  // case 1: no split
  if(remaning_size == 0)
    return TRUE;
  else if(sizeof(block_meta_data_t) < remaning_size && 
  remaning_size < (sizeof(block_meta_data_t) + sizeof(vm_page_family->size_of_struct))){
    next_block_meta = NEXT_META_BLOCK_BY_SIZE(meta_data);
    next_block_meta->is_free = TRUE;
    next_block_meta->size_of_block = remaning_size - sizeof(block_meta_data_t);
    next_block_meta->offset = meta_data->offset + sizeof(block_meta_data_t) + meta_data->size_of_block;
    mm_bind_block_for_allocation(meta_data, next_block_meta);
  }
  else if (remaning_size < sizeof(block_meta_data_t)){
    return TRUE;
  }
  else{
    next_block_meta = NEXT_META_BLOCK_BY_SIZE(meta_data);
    next_block_meta->is_free = TRUE;
    next_block_meta->size_of_block = remaning_size - sizeof(block_meta_data_t);
    next_block_meta->offset = meta_data->offset + sizeof(block_meta_data_t) + meta_data->size_of_block;
    mm_bind_block_for_allocation(meta_data, next_block_meta);
  }
  return TRUE;
}



/**
 * @brief Splits a free data block in a page family to allocate a new block.
 *
 * case 1: if theire is no vm page yet (that mean i will create it) or
   theire not enought space to satisfate the size request.
 *
 * case 2: if we have enought space to satisfy the request in our free meta_data.
 *
 * @param vm_page_family 
 * @param size 
 * @return block_meta_data_t* 
 */
block_meta_data_t * mm_allocate_free_data_block(vm_page_family_t *vm_page_family, uint32_t size){

  vm_page_t * vm_page = find_page_to_insert(vm_page_family);
  int status = FALSE;
  block_meta_data_t * block_meta_data = get_biggest_free_meta_data_block(vm_page);

  //********** case 1:
  if(vm_page == NULL || block_meta_data->size_of_block < size){
    vm_page = allocate_vm_page(vm_page_family);
    status = mm_split_free_data_block_for_allocation(vm_page_family, &vm_page->meta_block, size);
    if(status)
      return &vm_page->meta_block;
    return NULL;
  }

  //********** case 2:
  if(block_meta_data){
    status = mm_split_free_data_block_for_allocation(vm_page_family, block_meta_data, size);
    if(status)
      return block_meta_data;
  }
  return NULL;
}



int mm_get_hard_internal_memory_frag_size(block_meta_data_t *first, block_meta_data_t *second){
  block_meta_data_t *next_block = NEXT_META_BLOCK_BY_SIZE(first);
  return (int)((unsigned long) second - (unsigned long)next_block);
}