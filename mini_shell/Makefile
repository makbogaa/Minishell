NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = minishell.c\
	   src/environment.c \
	   src/execute.c \
	   src/builtin/builtin.c \
	   src/builtin/env.c \
	   src/builtin/cd.c \
	   src/builtin/echo.c \
	   src/builtin/pwd.c \
	   src/builtin/exit.c \
	   src/builtin/export.c \
	   src/builtin/unset.c \
	   src/free.c \

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INC = -I.

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	rm -f $(OBJS)

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
