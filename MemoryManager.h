
#include <memory.h>
#include "ft_printf/ft_printf.h"
#include <sys/mman.h>
#include <stddef.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0

struct vm_page_family_s;

typedef struct vm_page_family_s{
  char name_of_struct [32];
  uint32_t size_of_struct;
  struct vm_page_s *first_page;
  struct vm_page_s *page_to_insert;
} vm_page_family_t;

typedef struct emp_s{
  char name[32];
  uint32_t emp_id;
}emp_t;

// typedef struct student_s{
//   char name[32];
//   uint32_t rollno;
//   uint32_t marks_phy;
//   uint32_t marks_chem;
//   uint32_t marks_maths;
//   struct student_s * next;
// }student_t;
//************************************************************************************************


typedef struct vm_page_for_families_s{
  struct vm_page_for_families_s * next;
  vm_page_family_t vm_page_family[0];
} vm_page_for_families_t;

extern size_t SYSTEM_PAGE_SIZE;
extern vm_page_for_families_t * first_vm_page_for_families;


#define MAX_FAMILY_PER_VM_PAGE \
  (SYSTEM_PAGE_SIZE - sizeof(vm_page_for_families_t *) / sizeof(vm_page_family_t))

// vm_page_family_t * curr = NULL;
#define ITERATE_PAGE_FAMILIES_BEGIN(vm_page_for_families_ptr, curr) \
  { \
  uint32_t count = 0; \
  for(curr = (vm_page_family_t *)&vm_page_for_families_ptr->vm_page_family[0]; \
  curr->size_of_struct && count < MAX_FAMILY_PER_VM_PAGE; \
  curr++, count++){
#define ITERATE_PAGE_FAMILIES_END(vm_page_for_families_ptr, curr) }}


//************************************************************************************************

typedef struct block_meta_data_s
{
  int is_free;
  uint32_t size_of_block;
  uint32_t offset;
  struct block_meta_data_s *prev;
  struct block_meta_data_s *next;
}block_meta_data_t;

#define offset_of(container_structure, field_name) ((size_t) & (((container_structure *)0)->field_name))

#define MM_GET_PAGE_FROM_META_BLOCK(block_meta_data_ptr) \
  ((void *) ((char *)block_meta_data_ptr - block_meta_data_ptr->offset))

#define NEXT_META_BLOCK(block_meta_data_ptr) \
  (block_meta_data_ptr->next)


#define NEXT_META_BLOCK_BY_SIZE(block_meta_data_ptr) \
  (block_meta_data_t *) ((char *) (block_meta_data_ptr + 1) + block_meta_data_ptr->size_of_block)

#define PREV_META_BLOCK(block_meta_data_ptr) \
  (block_meta_data_ptr->prev)


//************************************************************************************************


typedef struct vm_page_s{
  struct vm_page_s *next;
  struct vm_page_s *prev;
  struct vm_page_family_s *page_family;
  block_meta_data_t meta_block;
  char page_memory[0]; //pointer to the first data 
}vm_page_t;

//this macro allow me to set a vm page as empty
#define MARK_PAGE_EMPTY(vm_page_t_ptr)  \
  vm_page_t_ptr->meta_block.is_free = TRUE; \
  vm_page_t_ptr->meta_block.next = NULL;  \
  vm_page_t_ptr->meta_block.prev = NULL;  \

#define mm_bind_blocks_for_allocation(allocated_meta_block, free_meta_block)  \
    free_meta_block->prev = allocated_meta_block;        \
    free_meta_block->next = allocated_meta_block->next;    \
    allocated_meta_block->next = free_meta_block;                \
    if (free_meta_block->next)\
    free_meta_block->next->prev = free_meta_block

#define ITERATE_VM_PAGE_BEGIN(vm_page_family_ptr, curr) \
  { \
  for(curr = vm_page_family_ptr->first_page; curr != NULL; curr=curr->next){
#define ITERATE_VM_PAGE_END(vm_page_family_ptr, curr) }}\

#define ITERATE_VM_PAGE_META_BLOCK_BEGIN(vm_page_t_ptr, block_meta_data) \
  { \
  uint32_t count = 0; \
  for(block_meta_data = (block_meta_data_t *)&vm_page_t_ptr->meta_block; \
  block_meta_data; \
  block_meta_data=block_meta_data->next, count++){

#define ITERATE_VM_PAGE_META_BLOCK_END(vm_page_t_ptr, meta_block)}}

#define MM_REG_STRUCT(name_of_struct) \
  (mm_instantiate_new_page_family(#name_of_struct, sizeof(name_of_struct)))

void mm_instantiate_new_page_family(char * name_of_struct, uint32_t size_of_struct);
int mm_is_vm_page_empty(vm_page_t * vm_page);
// void mm_instantiate_new_page_family(char * name_of_struct, uint32_t size_of_struct);
void mm_init();
vm_page_t * find_page_to_insert(vm_page_family_t * page_family);
block_meta_data_t * get_biggest_free_meta_data_block(vm_page_t * vm_page);
vm_page_family_t *lookup_page_family_by_name(char *struct_name);
void mm_bind_block_for_allocation(block_meta_data_t *block_meta, block_meta_data_t *next_meta);
int mm_is_vm_page_empty(vm_page_t * vm_page);
uint32_t mm_max_page_allocatable_memory(int units);
block_meta_data_t * mm_allocate_free_data_block(vm_page_family_t *vm_page_family, uint32_t size);
block_meta_data_t * mm_free_blocks(block_meta_data_t * block_to_free);
int mm_get_hard_internal_memory_frag_size(block_meta_data_t *first, block_meta_data_t *second);
void mm_union_free_block(block_meta_data_t *first, block_meta_data_t *second);
void free_and_delete_mv_page(vm_page_t * vm_page);
void show_alloc_mem();
void * ft_realloc(void *ptr, int new_size);
void ft_free(void *ptr);
void * ft_malloc(size_t size);
void	*ft_memset( void *pointer, int value, size_t count );
void	*ft_memcpy(void *dest, const void *src, size_t n);
