
#ifndef _MALLOC_H
#define _MALLOC_H

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <stdint.h>
#include <pthread.h>

#define TINY_MIN 32
#define SMALL_MIN 144
#define TINY_MAX SMALL_MIN - 1
#define SMALL_MAX 4000
// #define SMALL_MAX 1024

#define IS_TINY(x) (x <= TINY_MAX)
#define IS_SMALL(x) (x > TINY_MAX && x <= SMALL_MAX)
#define IS_LARGE(x) (x > SMALL_MAX)

#define METADATA_SIZE (sizeof(t_meta_block) + sizeof(t_ftr_block))
#define EFFECTIVE_SIZE(x) (x + METADATA_SIZE)
#define CEIL_DIV(a, b) ((a / b) + ((a % b) != 0))

#define SMALL_ZONE_SIZE (CEIL_DIV(((100 * EFFECTIVE_SIZE(SMALL_MAX)) + sizeof(t_memory_zone) + sizeof(t_meta_block)), getpagesize()) * getpagesize())
#define TINY_ZONE_SIZE  (CEIL_DIV(((100 * EFFECTIVE_SIZE(TINY_MAX)) + sizeof(t_memory_zone) + sizeof(t_meta_block)), getpagesize()) * getpagesize())
#define LARGE_ZONE_SIZE(x) ((CEIL_DIV((x + sizeof(t_memory_zone)), getpagesize())) * getpagesize())

#define ZONE_SIZE(type) (type == TINY_ZONE ? TINY_ZONE_SIZE : SMALL_ZONE_SIZE)

#define FIRST_BLOCK_SIZE(type) (ZONE_SIZE(type) - sizeof(t_memory_zone) - METADATA_SIZE)

#define GET_RIGHT_ZONE(size) (IS_TINY(size) ? g_zones.tiny : IS_SMALL(size) ? g_zones.small : g_zones.large)
#define GET_ZONE_BY_TYPE(type) (type == TINY_ZONE ? g_zones.tiny : type == SMALL_ZONE ? g_zones.small : g_zones.large)
#define GET_ZONE_TAIL(type) (type == TINY_ZONE ? g_zones.tiny_tail : type == SMALL_ZONE ? g_zones.small_tail : g_zones.large_tail)
#define GET_ZONE_TAIL_ADDR(type) (type == TINY_ZONE ? &g_zones.tiny_tail : type == SMALL_ZONE ? &g_zones.small_tail : &g_zones.large_tail)
#define GET_ZONE_ADDR(type) (type == TINY_ZONE ? &g_zones.tiny : type == SMALL_ZONE ? &g_zones.small : &g_zones.large)
#define GET_ZONE_NAME(type) (type == TINY_ZONE ? "TINY" : type == SMALL_ZONE ? "SMALL" : "LARGE")

#define GET_ZONE_FIRST_HEADER(zone) ((t_meta_block *)((void *)zone + sizeof(t_memory_zone) + sizeof(t_meta_block)))

#define GET_BLOCK_HEADER(block) ((void *)block - sizeof(t_meta_block))
#define GET_LBLOCK_HEADER(block) ((void *)block - sizeof(t_memory_zone))
#define GET_MEMORY_BLOCK(hdr) ((void *)hdr + sizeof(t_meta_block))
#define GET_L_MEMORY_BLOCK(zone) ((void *)zone + sizeof(t_memory_zone))
#define GET_BLOCK_FOOTER(hdr) ((t_ftr_block *)((void *)hdr + hdr->size + sizeof(t_meta_block) - sizeof(t_ftr_block)))
#define GET_NEXT_HEADER(hdr, size) ((t_meta_block *)((void *)hdr + sizeof(t_meta_block) + size))
#define GET_PREV_HEADER(hdr) ((t_meta_block *)((void *)hdr - *((t_ftr_block *)((void *)hdr - sizeof(t_ftr_block))) - sizeof(t_meta_block)))
#define GET_BLOCK_SIZE(hdr) (hdr->size - sizeof(t_ftr_block))
#define GET_L_BLOCK_SIZE(zone) (zone->size - sizeof(t_memory_zone))


#define PRINT_ADDR(addr) write(1, "0x", 2); print_base((long long)addr, 16)
#define PUT_NBR(number) print_base(number, 10)

#define MIN(a, b) (a < b ? a : b)

#define IS_ADDR_IN_ZONE(zone, addr) ((void *)zone < (void *)addr && (void *)addr < (void *)zone + zone->size)

#if __WORDSIZE == 32
    #define ALLIGN(x) (((((x) - 1) >> 2) << 2) + 4)
#else
    #define ALLIGN(x) (((((x) - 1) >> 3) << 3) + 8)
#endif



typedef enum        e_bool
{
    false = 0,
    true  = 1
}                   t_bool;

typedef enum        e_zone_type
{
    TINY_ZONE,
    SMALL_ZONE,
    LARGE_ZONE
}                   t_zone_type;

#if __WORDSIZE == 32
typedef uint16_t    t_ftr_block;
// strcutre representing a block of memory
typedef struct      s_meta_block
{
    uint16_t        is_free: 1;    // is the block free
    uint16_t        size: 15;   // size of the block
}                   t_meta_block;
#else
typedef uint64_t    t_ftr_block;

typedef struct      s_meta_block
{
    uint64_t        is_free: 1;    // is the block free
    uint64_t        size: 63;   // size of the block
}                   t_meta_block;
#endif

// structure representing a memory zone (tiny, small or large)
typedef struct      s_memory_zone
{
    size_t          size;     // pointer to the end of the zone
    struct s_memory_zone   *next;      // pointer to the next zone
    struct s_memory_zone   *prev;      // pointer to the previous zone
    uint64_t        padding; // padding
}                   t_memory_zone;

// structure containing the different zones
typedef struct      s_memory_zones
{
    t_memory_zone          *tiny;      // pointer to the tiny zone
    t_memory_zone          *tiny_tail;      // pointer to the next zone
    t_memory_zone          *small;     // pointer to the small zone
    t_memory_zone          *small_tail;      // pointer to the next zone
    t_memory_zone          *large;     // pointer to the large zone
    t_memory_zone          *large_tail;      // pointer to the next zone
}                   t_memory_zones;

extern t_memory_zones          g_zones;
extern pthread_mutex_t  g_mutex;

void            *ft_malloc(size_t size);
void            *malloc(size_t size);

void            *ft_realloc(void *ptr, size_t size);
// void            *reallocf(void *ptr, size_t size);
void            *realloc(void *ptr, size_t size);

void            ft_free(void *ptr);
void            free(void *ptr);

void            show_alloc_mem();
void            show_alloc_mem_ex();

void            set_block_metadata(t_meta_block *memory_block, t_bool is_free, size_t size);
t_bool          is_allocated(void *ptr);
size_t          get_alligned_size(size_t size);
t_meta_block*    search_in_zone(void *ptr, t_zone_type zone_type);
t_memory_zone*         search_in_large_zone(void *ptr);

void print_base(long long number, unsigned short base);
#endif