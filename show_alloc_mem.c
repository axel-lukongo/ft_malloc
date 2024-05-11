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

void ft_print_address(void* variable) {
    char addr_str[18]; // enough to hold a 16-digit hex address plus a null byte
    char *p = addr_str;
    uintptr_t addr = (uintptr_t)variable;

    ft_memset(addr_str, 0, 18);
    *p++ = '0';
    *p++ = 'x';

    for(int i = 0; i < 4; i++, *p++)
      *p = 'f';

    for (int i = 7; i >= 0; i--) {
        uint8_t byte = (addr >> (i * 4)) & 0xf;
        if (byte < 10) {
            *p++ = byte + '0';
        } else {
            *p++ = byte - 10 + 'a';
        }
    }

    *p = '\0';

    write(1, addr_str, 18);
}

#include<stdio.h>
int show_TYNY(vm_page_family_t *pg_family){
    vm_page_t * vm_page = NULL;
    block_meta_data_t * block_meta = NULL;
    int total_alloc = 0;
    ft_printf("TYNY : \n");
    if(pg_family->first_page){
      ITERATE_VM_PAGE_BEGIN(pg_family, vm_page)

        ITERATE_VM_PAGE_META_BLOCK_BEGIN(vm_page, block_meta)
        if(block_meta->is_free==FALSE && (block_meta->size_of_block > 0 && block_meta->size_of_block < 100)){
            ft_print_address(block_meta+sizeof(vm_page_family_t));
            write(1," - ", 3);
            ft_print_address(block_meta+sizeof(vm_page_family_t)+block_meta->size_of_block);
            write(1," : ", 3);
            ft_uitoa(block_meta->size_of_block);
            total_alloc += block_meta->size_of_block;
        }
        ITERATE_VM_PAGE_META_BLOCK_END(vm_page, block_meta)

      ITERATE_VM_PAGE_END(pg_family, vm_page)
    }
  return total_alloc;
}

int show_SMALL(vm_page_family_t *pg_family){
    vm_page_t * vm_page = NULL;
    block_meta_data_t * block_meta = NULL;
    int total_alloc = 0;
    ft_printf("SMALL : \n");
    if(pg_family->first_page){
      ITERATE_VM_PAGE_BEGIN(pg_family, vm_page)

        ITERATE_VM_PAGE_META_BLOCK_BEGIN(vm_page, block_meta)
        if(block_meta->is_free==FALSE && (block_meta->size_of_block >= 100 && block_meta->size_of_block < 4000)){
            ft_print_address(block_meta+sizeof(vm_page_family_t));
            write(1," - ", 3);
            ft_print_address(block_meta+sizeof(vm_page_family_t)+block_meta->size_of_block);
            write(1," : ", 3);
            ft_uitoa(block_meta->size_of_block);
            total_alloc += block_meta->size_of_block;
        }
        ITERATE_VM_PAGE_META_BLOCK_END(vm_page, block_meta)

      ITERATE_VM_PAGE_END(pg_family, vm_page)
    }
  return total_alloc;
}


int show_LARGE(vm_page_family_t *pg_family){
    vm_page_t * vm_page = NULL;
    block_meta_data_t * block_meta = NULL;
    int total_alloc = 0;
    ft_printf("LARGE : \n");
    if(pg_family->first_page){
      ITERATE_VM_PAGE_BEGIN(pg_family, vm_page)

        ITERATE_VM_PAGE_META_BLOCK_BEGIN(vm_page, block_meta)
        if(block_meta->is_free==FALSE && (block_meta->size_of_block >= 4000 )){
            ft_print_address(block_meta+sizeof(vm_page_family_t));
            write(1," - ", 3);
            ft_print_address(block_meta+sizeof(vm_page_family_t)+block_meta->size_of_block);
            write(1," : ", 3);
            ft_uitoa(block_meta->size_of_block);
            total_alloc += block_meta->size_of_block;
        }
        ITERATE_VM_PAGE_META_BLOCK_END(vm_page, block_meta)

      ITERATE_VM_PAGE_END(pg_family, vm_page)
    }
  return total_alloc;
}


void show_alloc_mem(){
    vm_page_family_t *pg_family = lookup_page_family_by_name("emp_t");
    int total_alloc = 0;
    total_alloc += show_TYNY(pg_family);
    total_alloc += show_SMALL(pg_family);
    total_alloc += show_LARGE(pg_family);
    ft_printf("total: ");
    ft_uitoa(total_alloc);
}