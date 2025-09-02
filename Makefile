# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/26 15:52:09 by tfournie          #+#    #+#              #
#    Updated: 2025/08/26 15:47:57 by tfournie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
			parsing/parsing.c \
			parsing/init_struct.c \
			parsing/init_struct_utils.c \
			parsing/print.c \
			parsing/manage_args.c \
			parsing/manage_quote.c \
			parsing/smart_split_utils.c \
			parsing/smart_split_and_parse.c \
			parsing/check_invalid.c \
			exec/exec.c \
            exec/exec_args.c \
			exec/path_exec_args.c \
            exec/free_cmd.c \
			exec/args_input.c \
			exec/redirections.c \
            exec/$(BUILTINS_PATH)/ft_cd.c \
            exec/$(BUILTINS_PATH)/ft_echo.c \
            exec/$(BUILTINS_PATH)/ft_env.c \
            exec/$(BUILTINS_PATH)/ft_exit.c \
            exec/$(BUILTINS_PATH)/ft_export.c \
            exec/$(BUILTINS_PATH)/ft_pwd.c \
            exec/$(BUILTINS_PATH)/ft_unset.c \

SRC = $(addprefix $(SRC_PATH)/,$(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR)/,$(SRC_FILES:.c=.o))
DEPS = $(OBJ:.o=.d)

# On récupère tous les .c de libft pour forcer la recompilation si l’un d’eux change
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


