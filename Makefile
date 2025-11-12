# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/02 12:39:36 by pedde-so          #+#    #+#              #
#    Updated: 2025/11/11 22:04:43 by beatde-a         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell
SRC_FILES = \
	builtin child child2 cleanup env \
	env_convert env_list env_utils error error_expander \
	error_executor error_parser error_utils executor executor2 \
	executor_and executor_builtin executor_builtin_utils executor_cd executor_echo \
	executor_env executor_exit executor_export executor_export2 executor_or \
	executor_pipe executor_pipe2 executor_pwd executor_redirect executor_redirect2 \
	executor_sort_env executor_subshell executor_subshell2 executor_unset executor_utils \
	executor_utils2 expander expander_dollar expander_metadata expander_quotes \
	expander_rebuild expander_redir expander_split expander_tilde get_next_line \
	get_next_line_utils input input_continuation input_prompt lexer \
	lexer_list lexer_type main parent parser \
	parser_cmd parser_heredoc_collect parser_heredoc_scan parser_redir parser_tree \
	parser_utils signal signal_handler signal_heredoc stack \
	stack2 stack3 test variable_utils wildcard \
	wildcard_match wildcard_rebuild


SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = include

PRINTF_URL  = https://github.com/pdrlrnc/ft_printf.git
PRINTF_DIR  = ft_printf
PRINTF_LIB  = $(PRINTF_DIR)/libftprintf.a

LIBFT_DIR = libft

CC          = cc -g -O0
CFLAGS      = -Wall -Wextra -Werror -I$(INC_DIR) -MMD -MP
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
DEP	    = $(addprefix $(OBJ_DIR)/, $(addsuffix .d, $(SRC_FILES)))

.PHONY: all clean fclean re headers test valgrind run

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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@ctags -R .
	@echo "tags\n.gitignore\n.vscode\nft_printf\ninclude/libft.h\ninclude/printf.h\nminishell\nobj\n.gitattributes\
		\noutfile*\n*.pdf\n*.png\ntest/results\ntest/file1\ntest/file2\nminishell_tester/\nminishell_testerv2/" > .gitignore
	@echo "* text=auto eol=lf" > .gitattributes

$(OBJ_DIR):
	@echo "Compiling minishell..."
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
	@$(RM) -rf test/results
	@$(RM) err.tmp

valgrind: $(NAME)
	valgrind --suppressions=readline.supp --leak-check=full --track-fds=yes --show-leak-kinds=all --trace-children=yes ./${NAME}
test: $(NAME)
	@chmod 755 test/tester.sh
	@./test/tester.sh

run:	$(NAME)
	@./minishell

re: fclean all

-include $(DEP)
