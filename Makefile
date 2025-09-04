NAME        = minishell

SRC_FILES   = cleanup env env_convert env_list error expander expander_dollar expander_quotes\
				lexer lexer_list main parser parser_cmd parser_redir parser_tree parser_utils\
				wildcard wildcard_argv wildcard_match
SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = include

PRINTF_URL  = https://github.com/pdrlrnc/ft_printf.git
PRINTF_DIR  = ft_printf
PRINTF_LIB  = $(PRINTF_DIR)/libftprintf.a

LIBFT_DIR = libft

CC          = cc -g -O0
CFLAGS      = -Wall -Wextra -Werror -I$(INC_DIR)
RM          = rm -rf

RED=\033[0;31m
ORANGE      := \033[38;5;208m
YELLOW      := \033[38;5;226m
GREEN       := \033[38;5;082m
BLUE        := \033[38;5;027m
INDIGO      := \033[38;5;057m
VIOLET      := \033[38;5;129m
DEF_COLOUR=\033[0m

SRC         = $(addprefix $(SRC_DIR)/, $(addsuffix .c, $(SRC_FILES)))
OBJ         = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_FILES)))
HDRS        = $(INC_DIR)/minishell.h $(INC_DIR)/printf.h $(INC_DIR)/libft.h

.PHONY: all clean fclean re headers

all: $(PRINTF_LIB) headers $(NAME)
	@echo "	\n\
		$(RED) ___ ___  ____  ____   ____ _____ __ __    ___  _      _     $(DEF_COLOUR)\n\
		$(ORANGE)|   |   ||    ||    \ |    / ___/|  |  |  /  _]| |    | |    $(DEF_COLOUR)\n\
		$(YELLOW)| _   _ | |  | |  _  | |  (   \_ |  |  | /  [_ | |    | |    $(DEF_COLOUR)\n\
		$(GREEN)|  \_/  | |  | |  |  | |  |\__  ||  _  ||    _]| |___ | |___ $(DEF_COLOUR)\n\
		$(BLUE)|   |   | |  | |  |  | |  |/  \ ||  |  ||   [_ |     ||     |$(DEF_COLOUR)\n\
		$(INDIGO)|   |   | |  | |  |  | |  |\    ||  |  ||     ||     ||     |$(DEF_COLOUR)\n\
		$(VIOLET)|___|___||____||__|__||____|\___||__|__||_____||_____||_____|$(DEF_COLOUR)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "tags\n.gitignore\n*.txt\n.vscode\nft_printf\ninclude/libft.h\ninclude/printf.h\nminishell\nobj" > .gitignore

$(OBJ_DIR):
	@mkdir -p $@

$(NAME): $(OBJ) $(PRINTF_LIB)
	@$(CC) $(CFLAGS) $(OBJ) -L$(PRINTF_DIR) -lftprintf -lreadline -o $(NAME)

$(PRINTF_DIR):
	@git clone --depth 1 $(PRINTF_URL) $(PRINTF_DIR)

$(PRINTF_LIB): | $(PRINTF_DIR)
	@$(MAKE) --no-print-directory -C $(PRINTF_DIR)

headers: $(INC_DIR)/printf.h $(INC_DIR)/libft.h

$(INC_DIR)/printf.h: $(PRINTF_LIB)
	@mkdir -p $(INC_DIR)
	@cp $(PRINTF_DIR)/include/printf.h $(INC_DIR)/printf.h

$(INC_DIR)/libft.h: $(PRINTF_LIB)
	@mkdir -p $(INC_DIR)
	@cp $(PRINTF_DIR)/include/libft.h $(INC_DIR)/libft.h

clean:
	@$(RM) $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)
	@$(RM) $(INC_DIR)/printf.h $(INC_DIR)/libft.h 
	@$(RM) -r $(LIBFT_DIR)
	@$(RM) -r $(PRINTF_DIR)

re: fclean all
