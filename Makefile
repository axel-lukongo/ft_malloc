# ifeq ($(HOSTTYPE),)
#   HOSTTYPE := $(shell uname -m)_$(shell uname -s)
# endif

# CC = gcc
# CFLAGS = -g -Wall
# SRC = src/calloc.c \
# 			src/free.c \
# 			src/show_mem_alloc.c \
# 			src/malloc.c \
# 			src/realloc.c \
# 			src/utils.c \
# 			test_app.c\


# OBJ = $(SRC:.c=.o)
# LIB_NAME = libft_malloc_$(HOSTTYPE).so
# LINK_NAME = libft_malloc.so



# all: $(LIB_NAME) symlink

# $(LIB_NAME): $(OBJ)
# 		# make -C ft_printf
# 		$(CC) $(CFLAGS) -shared -o $(LIB_NAME) $(OBJ)
# symlink:
# 		ln -sf $(LIB_NAME) $(LINK_NAME)

# %.o: %.c
# 		$(CC) $(CFLAGS) -fPIC -c $< -o $@

# test: $(OBJ) test_app.o
# 		$(CC) $(CFLAGS) -o test_app test_app.o -L. -lft_malloc
# 		LD_LIBRARY_PATH=. ./test_app

# clean:
# 		rm -f $(OBJ) test_app.o test_app $(LIB_NAME) $(LINK_NAME)
# 		# make fclean -C ft_printf

# re: clean all

# .PHONY: all clean re test symlink







test:
		# gcc -g -c src/calloc.c -o calloc.o
		gcc -g -c src/free.c -o free.o
		gcc -g -c src/show_mem_alloc.c -o show_mem_alloc.o
		gcc -g -c src/malloc.c -o malloc.o
		gcc -g -c src/realloc.c -o realloc.o
		# gcc -g -c src/reallocf.c -o reallocf.o
		gcc -g -c src/utils.c -o utils.o
		gcc -g -c test_app.c -o test_app.o

		gcc -g free.o show_mem_alloc.o malloc.o realloc.o utils.o test_app.o -o exe
		./exe

docker:
		# sudo systemctl restart docker 
		sudo docker pull debian:latest
		sudo docker-compose -f docker-compose.yml build #--no-cache
		sudo docker-compose -f docker-compose.yml up --force-recreate -d #--force-recreate
		# sudo docker exec -it malloc2-malloc2-1 bash


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

malloc:
		sudo docker exec -it malloc-malloc-1 bash

.PHONY: start stop re ps clean%
