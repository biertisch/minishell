SRC_DIR = src/
OBJ_DIR = obj/
LIB_DIR = libft/

CC = cc
CFLAGS = -Wall -Werror -Wextra -g -Iinclude -I$(LIB_DIR)include
LFLAGS = -lreadline
RM = rm -rf

LIB = $(LIB_DIR)libft.a

NAME = minishell

SRC = $(SRC_DIR)env_list.c\
		$(SRC_DIR)env.c\
		$(SRC_DIR)expander.c\
		$(SRC_DIR)lexer.c\
		$(SRC_DIR)main.c\
		$(SRC_DIR)parser.c\

OBJ = $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))

all: $(NAME)

$(NAME): $(LIB) obj $(OBJ) 
	$(CC) $(OBJ) -o $@ $(LIB) $(LFLAGS)

$(LIB):
	$(MAKE) -C $(LIB_DIR)

obj:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)
	$(MAKE) -C $(LIB_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIB_DIR) fclean

re: fclean all

.PHONY: all clean fclean re