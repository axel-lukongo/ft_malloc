# ifeq ($(HOSTTYPE),)
#   HOSTTYPE := $(shell uname -m)_$(shell uname -s)
# endif

# CC = gcc
# CFLAGS = -g -Wall
# SRC = MemoryManagerTools.c \
# 			MemoryManager.c \
# 			ft_malloc.c \
# 			ft_realloc.c \
# 			show_alloc_mem.c \
# 			ft_free.c \
# 			ft_memset.c \
# 			ft_memcpy.c \
# 			ft_printf/ft_printf.c\
# 			ft_printf/ft_printf_utils.c\
# 			ft_printf/print_hex.c\
# 			ft_printf/print_unsigned_int.c\
# 			ft_printf/print_ptr.c\


# OBJ = $(SRC:.c=.o)
# LIB_NAME = libft_malloc_$(HOSTTYPE).so
# LINK_NAME = libft_malloc.so



# all: $(LIB_NAME) symlink

# $(LIB_NAME): $(OBJ)
# 		make -C ft_printf
# 		$(CC) $(CFLAGS) -shared -o $(LIB_NAME) $(OBJ)
# symlink:
# 		ln -sf $(LIB_NAME) $(LINK_NAME)

# %.o: %.c
# 		$(CC) $(CFLAGS) -fPIC -c $< -o $@

# test: $(OBJ) test_app.o
# 		$(CC) $(CFLAGS) -o test_app ft_printf/libftprintf.a test_app.o -L. -lft_malloc
# 		LD_LIBRARY_PATH=. ./test_app

# clean:
# 		rm -f $(OBJ) test_app.o test_app $(LIB_NAME) $(LINK_NAME)
# 		make fclean -C ft_printf

# re: clean all

# .PHONY: all clean re test symlink







up:
		gcc -g -c MemoryManagerTools.c -o mm_tools.o
		gcc -g -c MemoryManager.c -o mm.o
		gcc -g -c ft_malloc.c -o ft_malloc.o
		gcc -g -c ft_realloc.c -o ft_realloc.o
		gcc -g -c show_alloc_mem.c -o show_alloc_mem.o
		gcc -g -c ft_free.c -o ft_free.o
		gcc -g -c test_app.c -o test_app.o
		gcc -g -c ft_memset.c -o mem.o
		gcc -g -c ft_memcpy.c -o memc.o
		gcc -g -c ft_printf/ft_printf.c -o fprintf.o
		gcc -g -c ft_printf/ft_printf_utils.c -o fprintfu.o
		gcc -g -c ft_printf/print_hex.c -o printHex.o
		gcc -g -c ft_printf/print_unsigned_int.c -o printUInt.o
		gcc -g -c ft_printf/print_ptr.c -o printPtr.o
		gcc -g mm_tools.o mm.o ft_realloc.o printPtr.o printUInt.o printHex.o fprintfu.o fprintf.o memc.o mem.o show_alloc_mem.o ft_malloc.o ft_free.o test_app.o -o exe
		./exe

docker:
		# sudo systemctl restart docker 
		sudo docker pull debian:latest
		sudo docker-compose -f docker-compose.yml build #--no-cache
		sudo docker-compose -f docker-compose.yml up --force-recreate -d #--force-recreate
		sudo docker exec -it malloc-malloc-1 bash


down:
		sudo docker-compose -f docker-compose.yml down 

ps:		
		sudo docker-compose -f docker-compose.yml ps -a
		sudo docker ps -a

clean:
		rm exe
		rm *.o

stop-docker:	down
		sudo docker system prune -af
		sudo docker system prune

re : 	clean up

# malloc:
#		sudo docker exec -it malloc-malloc-1 bash

# .PHONY: start stop re ps clean%