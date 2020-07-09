NAME = test
GCC = gcc -g
INC =  includes/
INC_L = libft/includes
P_LIB = libft/
P_SRC = src/
SRC = asm.c	\
      error.c	\
      free.c	\
      lexic.c	\
      syntax.c	\
      code.c	\
      buf.c

OBJ = $(addprefix $(P_SRC), $(SRC))

all:
	$(GCC) $(OBJ) -L $(P_LIB) -lftdprintf -I $(INC) -I $(INC_L) -o $(NAME)
