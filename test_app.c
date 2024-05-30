// #include "uapi_mm.h"
#include <string.h>
#include <unistd.h>
// #include <stdlib.h>
#include "inc/malloc.h"

/**
 * il faudra que je verifie demain dans quelle vm_page le block 4 se trouve
 * car lorsque je le free le realloc segfault donc cela signifie que cette page
 * a ete retourne au kernel mais que le pointeur n'a pas ete mis a null
 * donc pour le realloc lorsque je cherche le plus grand free, cela me fais
 * depasser dans ma boucle et donc je segfault
 * 
 * @return int 
 */
// void    print(char *s)
// {
//     write(1, s, strlen(s));
// }

// int main(){

//   char *test1 = malloc(70);
//   void *test2 = malloc(1990);
//   void *test3 = malloc(4000);
//   void *test4 = malloc(4000);
//   void *test5 = malloc(4007);
//   void *test6 = malloc(400);
//   void *test7 = malloc(408);
//   void *test8 = malloc(984);
//   char *test9;
//   for(int i = 0; i < 5; i++){
//     test9 = malloc(84);
//   }
//   test9[0] = 'A';
//   char *emp= malloc(5);
//   show_alloc_mem();
//   printf("-\n-\n");
//   for(int i = 0; i < 40; i++){
//     test9[i] = 'A';
//   }
//   printf(test9);
//   show_alloc_mem();
//   printf("-\n-\n");
//   free(emp);
//   free(test2);
//   free(test1);
//   printf(test1);
//   show_alloc_mem();

//   free(test3);
//   printf("-\n-\n");
//   free(test5);
//   free(test4);
//   show_alloc_mem();
//   test6 = realloc(test6, 300);
//   show_alloc_mem();
//   printf("-\n-\n");
//   free(test6);
//   free(test7);
//   free(test8);
//   free(test9);
//   show_alloc_mem();
//   return 0;
// }




// void    print(char *s)
// {
//     write(1, s, strlen(s));
// }

// int     main(void)
// {
//     int   i;
//     char  *addr;

//     i = 0;
//     while (i < 1024)
//     {
//         addr = (char*)malloc(2000);
//         if (addr == NULL)
//         {
//             print("Failed to allocate memory\n");
//             return (1);
//         }
//         addr[0] = 42;
//         i++;
//     }
//     return (0);
// }


// #include <stdlib.h>
// #include <string.h>

// void    print(char *s)
// {
//     write(1, s, strlen(s));
// }

// int     main(void)
// {
//     int   i;
//     char  *addr;

//     i = 0;
//     while (i < 1024) 
//     {
//         addr = (char*)malloc(1024);
//         if (addr == NULL)
//         {
//             print("Failed to allocate memory\n");
//             return (1);
//         }
//         addr[0] = 42;
//         free(addr); 
//         show_alloc_mem();
//         i++; 
//     }
//     return (0);
// }



// #include <stdlib.h>

#define M (1024 * 1024)

// void print(char *s)
// {
//     write(1, s, strlen(s));
// }

// int     main()
// {
//     char *addr1;
//     char *addr2;
//     char *addr3;

//     addr1 = (char*)malloc(16*M);
//     if (addr1 == NULL)
//     {
//         print("Failed to allocate memory\n");
//         exit(1);
//     }
//     strcpy(addr1, "Hello world!\n");
//     print(addr1);
//     addr2 = (char*)malloc(16*M);
//     if (addr2 == NULL)
//     {
//         print("Failed to allocate memory\n");
//         exit(1);
//     }
//     addr3 = (char*)realloc(addr1, 128*M);
//     if (addr3 == NULL)
//     {
//         print("Failed to reallocate memory\n");
//         exit(1);
//     }
//     addr3[127*M] = 42;
//     print(addr3);
//     return (0);
// }



int main()
{
    void* a = malloc(1);
    void* b = malloc(2);
    void* c = malloc(4);
    void* d = malloc(8);
    void* e = malloc(16);
    void* f = malloc(32);
    void* g = malloc(64);
    void* h = malloc(128);
    void* i = malloc(256);
    void* j = malloc(512);
    void* k = malloc(1024);
    void* l = malloc(1024 * 2);
    void* m = malloc(1024 * 4); 
    void* n = malloc(1024 * 32);
    void* o = malloc(M);
    void* p = malloc(16*M);
    void* q = malloc(128*M);
    show_alloc_mem(); 
    return (0); 
}

#include <string.h>
#include <unistd.h>
// #include <stdlib.h>

// void print(char *s)
// {
//     write(1, s, strlen(s));
// }

// int     main()
// {
//     int     i;
//     int     alignment;
//     char    *addr;

//     i = 1;
//     alignment = 2 * sizeof(size_t);
//     while (i <= 100)
//     {
//         addr = (char*)malloc(i);
//         // addr = malloc(42, NULL, ALLOC, NULL);
//         if (addr == NULL)
//         {
//             print("Failed to allocate memory\n");
//             exit(1);
//         }
//         if ((((unsigned long) (addr)) % alignment) != 0)
//         {
//             print("malloc returned a non aligned boundary\n");
//             exit(1);
//         }
//         i++;
//         free(addr);
//     }
//     print("malloc works fine\n");
// }