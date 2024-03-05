#include"MemoryManager.h"

static void ft_uitoa(unsigned int nb)
{
	int		i;
	char	str[10000];

	i = nb_digit(nb, 10);
	if (i == 0)
		i = 1;
	str[i] = '\0';
	while (i > 0)
	{
		str[--i] = (nb % 10) + '0';
		nb = nb / 10;
	}
  ft_printf("%s bytes\n", str);
}
void ft_putchar2(char c){
  write(1, &c, 1);
}

void show_alloc_mem(){

    vm_page_family_t *pg_family = lookup_page_family_by_name("emp_t");
    vm_page_t * vm_page = NULL;
    block_meta_data_t * block_meta = NULL;
    // char * str = NULL;
    if(pg_family->first_page){
      ITERATE_VM_PAGE_BEGIN(pg_family, vm_page)

        ITERATE_VM_PAGE_META_BLOCK_BEGIN(vm_page, block_meta)
        if(block_meta->is_free==FALSE){
          ft_printf("%p - %p: ", block_meta+1, (block_meta+1)+block_meta->size_of_block);
          ft_uitoa(block_meta->size_of_block);
        }
        ITERATE_VM_PAGE_META_BLOCK_END(vm_page, block_meta)

      ITERATE_VM_PAGE_END(pg_family, vm_page)
    }
}