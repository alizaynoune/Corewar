NAME = test
GCC = gcc -Wall -Werror -Wextra
INC =  includes/
INC_L = libft/includes
P_LIB = libft/
P_SRC = src/
SRC = asm.c	\
      error.c	\
      free.c	\
      lexic.c	\
      syntax.c

OBJ = $(addprefix $(P_SRC), $(SRC))

all:
	$(GCC) $(OBJ) -L $(P_LIB) -lprintf -I $(INC) -I $(INC_L) -o $(NAME)
