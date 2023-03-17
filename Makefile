##
## EPITECH PROJECT, 2020
## makefile
## File description:
## Makefile
##

CFLAGS 			:=  --std=gnu99 -Wextra -Wall -g3 # -Werror
CPPFLAGS		:= 	-I ./inc

CC 				:= gcc

ifneq (,$(shell command -v ccache))
	CC := ccache gcc
endif

NM_SRC			:=	$(shell find src/nm -name "*.c" -type f)
NM_OBJ			:=	$(NM_SRC:.c=.o)
NM_NAME			:=	my_nm

OBJDUMP_SRC		:=	$(shell find src/objdump -name "*.c" -type f)
OBJDUMP_OBJ		:=	$(OBJDUMP_SRC:.c=.o)
OBJDUMP_NAME	:=	my_objdump

all:	 nm objdump

nm: $(NM_OBJ)
	@$(CC) -o $(NM_NAME) $(NM_OBJ)

objdump: $(OBJDUMP_OBJ)
	@$(CC) -o $(OBJDUMP_NAME) $(OBJDUMP_OBJ)

obj_test:
	make
	./tests/objdump/objdump.sh

nm_test:
	make
	./tests/nm/nm.sh

tests:
	make
	./tests/objdump/objdump.sh
	./tests/nm/nm.sh

VGFLAGS := --leak-check=full --error-exitcode=42 --trace-children=yes --quiet \
	--track-origins=yes

valgrind:
	make
	valgrind $(VGFLAGS) ./$(NAME)
	make clean

clean:
	rm -f $(NM_OBJ) $(OBJDUMP_OBJ) *.gcno *.gcda

fclean: clean
	rm -f $(NM_NAME) $(OBJDUMP_NAME)

re: fclean all

.PHONY: clean fclean re all
