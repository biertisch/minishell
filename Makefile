# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/02 12:39:36 by pedde-so          #+#    #+#              #
#    Updated: 2025/10/21 22:52:01 by beatde-a         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell

SRC_FILES   = builtin cleanup env env_convert env_list error expander\
		expander_dollar expander_quotes input lexer lexer_list\
		lexer_utils main parser parser_cmd parser_redir parser_subshell\
		parser_tree parser_utils wildcard wildcard_argv wildcard_match\
		test stack parser_tree2 signal executor child parent\
		executor_utils executor_pipe executor_and executor_or executor_builtin\
		executor_echo executor_subshell get_next_line get_next_line_utils\
		executor_env executor_cd executor_pwd executor_exit expand_tilde\
		input_prompt executor_unset executor_error expander_dollar2\
		expander_dollar3 input_continue


SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = include

PRINTF_URL  = https://github.com/pdrlrnc/ft_printf.git
PRINTF_DIR  = ft_printf
PRINTF_LIB  = $(PRINTF_DIR)/libftprintf.a

LIBFT_DIR = libft

CC          = cc -g -O0
CFLAGS      = -Wall -Wextra -I$(INC_DIR) # add -Werror
RM          = rm -rf

RED=\033[0;31m
GREEN	    := \033[92;5;118m
ORANGE      := \033[38;5;208m
YELLOW      := \033[38;5;226m
GREEN       := \033[38;5;082m
BLUE        := \033[38;5;027m
INDIGO      := \033[38;5;057m
VIOLET      := \033[38;5;129m
BLINK       := \033[5m
CURSIVE	    := \033[33;3m
DEF_COLOUR=\033[0m

SRC         = $(addprefix $(SRC_DIR)/, $(addsuffix .c, $(SRC_FILES)))
OBJ         = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_FILES)))
HDRS        = $(INC_DIR)/minishell.h $(INC_DIR)/printf.h $(INC_DIR)/libft.h $(INC_DIR)/parser.h $(INC_DIR)/executor.h


.PHONY: all clean fclean re headers test valgrind

all: $(PRINTF_LIB) headers $(NAME)
	@echo "	\n\
		$(RED) ___ ___  ____  ____   ____ _____ __ __    ___  _      _     $(DEF_COLOUR)\n\
		$(ORANGE)|   |   ||    ||    \ |    / ___/|  |  |  /  _]| |    | |    $(DEF_COLOUR)\n\
		$(YELLOW)| _   _ | |  | |  _  | |  (   \_ |  |  | /  [_ | |    | |    $(DEF_COLOUR)\n\
		$(GREEN)|  \_/  | |  | |  |  | |  |\__  ||  _  ||    _]| |___ | |___ $(DEF_COLOUR)\n\
		$(BLUE)|   |   | |  | |  |  | |  |/  \ ||  |  ||   [_ |     ||     |$(DEF_COLOUR)\n\
		$(INDIGO)|   |   | |  | |  |  | |  |\    ||  |  ||     ||     ||     |$(DEF_COLOUR)\n\
		$(VIOLET)|___|___||____||__|__||____|\___||__|__||_____||_____||_____|$(DEF_COLOUR)\n"
	@printf "$(BLINK)$(CURSIVE)$(GREEN)\t\t\t\t\t\t- Minishell ready :)$(DEF_COLOUR)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@ctags -R .
	@echo "tags\n.gitignore\n.vscode\nft_printf\ninclude/libft.h\ninclude/printf.h\nminishell\nobj\n.gitattributes\
		\noutfile*\n*.pdf\n*.png\ntest/bash\ntest/minishell\ntest/diffs\ntest/file1\ntest/file2\ntest/leaks" > .gitignore
	@echo "* text=auto eol=lf" > .gitattributes

$(OBJ_DIR):
	@mkdir -p $@

$(NAME): $(OBJ) $(PRINTF_LIB)
	@$(CC) $(CFLAGS) $(OBJ) -L$(PRINTF_DIR) -lftprintf -lreadline -o $(NAME)

$(PRINTF_DIR):
	@git clone --quiet --depth 1 $(PRINTF_URL) $(PRINTF_DIR)

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
	@$(RM) tags

fclean: clean
	@$(RM) $(NAME)
	@$(RM) $(INC_DIR)/printf.h $(INC_DIR)/libft.h
	@$(RM) -r $(LIBFT_DIR)
	@$(RM) -r $(PRINTF_DIR)
	@$(RM) outfile*
	@$(RM) -rf test/bash
	@$(RM) -rf test/diffs
	@$(RM) -rf test/minishell
	@$(RM) -rf test/leaks
	@$(RM) test/tests

valgrind: $(NAME)
	valgrind --suppressions=readline.supp --leak-check=full --track-fds=yes --show-leak-kinds=all --trace-children=yes ./${NAME}
test: re
	@chmod 755 test/run_tests.sh
	@./test/run_tests.sh

re: fclean all
