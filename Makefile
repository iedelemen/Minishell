NAME	= minishell

CC		= cc

RM		= rm -rf

CFLAGS	= -Wall -Wextra -Werror

SRC =	src/main.c					\
		garbage_collecter/alloc.c	\
		garbage_collecter/clean.c	\
		src/check/cmd_check.c		\
		src/command/cmd_cd.c		\
		src/command/cmd_env.c		\
		src/command/cmd_pwd.c		\
		src/command/cmd_exit.c		\
		src/command/cmd_echo.c		\
		src/command/cmd_unset.c		\
		src/command/cmd_export.c	\
		src/excec/parser.c			\
		src/excec/use_execve.c		\
		src/excec/execute_conf.c	\
		src/lexer/lexer.c			\
		src/lexer/token.c			\
		src/signal/signal.c			\
		src/expander/expander.c		\
		src/utils/exit.c			\
		src/utils/utils.c			\
		src/utils/rdir_utils.c		\
		src/utils/parser_utils.c	\
		src/utils/builtin_utils.c	\

LIBFT = libft/libft.a

OBJ_DIR = .objs

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all : $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBFT) -lreadline

$(OBJ_DIR)/%.o : %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C libft

clean:
	@$(RM) $(OBJ_DIR)
	@$(MAKE) -C libft clean

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C libft fclean

re: fclean all



iclean:
	@grep -v '^#' .gitignore | grep -v '^$$' | while read pattern; do \
		find . -name "$$pattern" -exec rm -rf {} +; \
	done

norm:
	@if norminette | grep -q "Error"; then \
		echo "Norminette: \033[31m[KO]\033[0m"; \
		norminette | grep "Error"; \
	else \
		echo "Norminette: \033[32m[OK]\033[0m"; \
	fi

.PHONY: all fclean clean re
