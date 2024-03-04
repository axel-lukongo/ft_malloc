#include"MemoryManager.h"

void show_alloc_mem(){

    vm_page_family_t *pg_family = lookup_page_family_by_name("emp_t");
    vm_page_t * vm_page = NULL;
    block_meta_data_t * block_meta = NULL;
    if(pg_family->first_page){
      ITERATE_VM_PAGE_BEGIN(pg_family, vm_page)

        ITERATE_VM_PAGE_META_BLOCK_BEGIN(vm_page, block_meta)
        if(block_meta->is_free==FALSE)
          ft_printf("%p - %p: %d bytes\n", block_meta+1, (block_meta+1)+block_meta->size_of_block, block_meta->size_of_block);

        ITERATE_VM_PAGE_META_BLOCK_END(vm_page, block_meta)
        // ft_printf("page |||\n");
      ITERATE_VM_PAGE_END(pg_family, vm_page)
    }
}