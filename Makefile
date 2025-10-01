CC = cc
CFLAGS = -g -Wall -Wextra -Werror -MMD -MP

NAME = minishell
CFLAGS_RD = -lreadline

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
BUILTINS_PATH  = builtins
LIBFT_PATH  := Libft
LIBFT_INCLUDE = -I./include

SRC_PATH = src
OBJ_DIR = build

SRC_FILES = main.c \
			\
			main_utils/promp.c \
			main_utils/signaux.c \
			main_utils/input.c \
			main_utils/input_utils.c \
			main_utils/input_process_utils.c \
			main_utils/promp_length.c \
			main_utils/free_cmd.c \
			main_utils/free_args.c \
			\
			\
			parsing/check_invalid/check_invalid.c \
			parsing/check_invalid/check_middle.c \
			parsing/check_invalid/check_start.c \
			parsing/check_invalid/print_error.c \
			\
			parsing/debug/print.c \
			\
			parsing/init_parsing/init_cmd.c \
			parsing/init_parsing/init_one_cmd.c \
			parsing/init_parsing/init_cmd_utils.c \
			parsing/init_parsing/init_cmd_free.c \
			parsing/init_parsing/manage_args.c \
			parsing/init_parsing/pipes.c \
			parsing/init_parsing/pipes_utils.c \
			parsing/init_parsing/spc_redir.c \
			parsing/init_parsing/redir_syntax.c \
			\
			parsing/redirections/infile.c \
			parsing/redirections/outfile.c \
			\
			parsing/smart_split/env_expansion.c \
			parsing/smart_split/env_utils.c \
			parsing/smart_split/get_arg_length.c \
			parsing/smart_split/manage_quote.c \
			parsing/smart_split/smart_split_and_parse.c \
			parsing/smart_split/smart_split_extract.c \
			parsing/smart_split/smart_split_utils_helper.c \
			parsing/smart_split/smart_split_utils.c \
			\
			parsing/parsing.c \
			\
			\
            exec/exec_utils/exec_cmd_utils.c \
            exec/exec_utils/exec_redirections.c \
            exec/exec_utils/exec_process.c \
            exec/exec_utils/exec_main.c \
            exec/exec_utils/exec_pipeline_utils.c \
            exec/exec_utils/exec_pipeline_child.c \
            exec/exec_utils/exec_pipeline.c \
			\
			exec/path/path_validation.c \
			exec/path/path_search.c \
			exec/path/path_main.c \
			\
			exec/redirect/redirect_expansion.c \
			exec/redirect/redirect_validation.c \
			exec/redirect/redirect_input_output.c \
			exec/redirect/redirect_heredoc.c \
			exec/redirect/redirect_main.c \
			\
            exec/$(BUILTINS_PATH)/ft_cd.c \
			exec/$(BUILTINS_PATH)/ft_cd_utils.c \
            exec/$(BUILTINS_PATH)/ft_echo.c \
			exec/$(BUILTINS_PATH)/ft_echo_utils.c \
            exec/$(BUILTINS_PATH)/ft_env.c \
            exec/$(BUILTINS_PATH)/ft_exit.c \
            exec/$(BUILTINS_PATH)/export/env_array.c \
			exec/$(BUILTINS_PATH)/export/env_list.c \
			exec/$(BUILTINS_PATH)/export/env_sort.c \
			exec/$(BUILTINS_PATH)/export/expand_exit_code.c \
			exec/$(BUILTINS_PATH)/export/export_print.c \
			exec/$(BUILTINS_PATH)/export/export_utils.c \
			exec/$(BUILTINS_PATH)/export/export_var.c \
			exec/$(BUILTINS_PATH)/export/ft_export.c \
            exec/$(BUILTINS_PATH)/ft_pwd.c \
            exec/$(BUILTINS_PATH)/ft_unset.c \
			\
			exec/exec.c \

SRC = $(addprefix $(SRC_PATH)/,$(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR)/,$(SRC_FILES:.c=.o))
DEPS = $(OBJ:.o=.d)

# On récupère tous les .c de libft pour forcer la recompilation si l'un d'eux change
LIBFT_SRCS = $(wildcard $(LIBFT_DIR)/*.c )
LIBFT_HDR = $(LIBFT_DIR)/libft.h

GREEN = \033[32m
RED = \033[31m
RESET = \033[0m

MAKEFLAGS += --no-print-directory

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(CFLAGS_RD) $(LIBFT) -o $(NAME)
	@echo "$(GREEN)Le programme est prêt !$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LIBFT_INCLUDE) -c $< -o $@

# Maintenant libft dépend de tous ses fichiers .c
$(LIBFT): $(LIBFT_SRCS) $(LIBFT_HDR)
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	@$(MAKE) clean -C $(LIBFT_DIR)
	@echo "$(RED)Fichiers objets supprimés.$(RESET)"

fclean: clean
	rm -f $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@echo "$(RED)Exécutable et objets supprimés.$(RESET)"

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)