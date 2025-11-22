# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pifourni <pifourni@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/20 14:33:23 by pifourni          #+#    #+#              #
#    Updated: 2025/11/20 23:36:22 by pifourni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LIB = libft/libft.a
SRC = ./pipex.c \
	  ./utils.c \
	  ./error.c

OBJ = $(SRC:.c=.o)

NAME = pipex

all: libft/libft.a $(NAME)
	@echo -e '\033[32mPipex compiled successfully!\033[0m'

libft/libft.a:
	@echo -e '\033[34mCompiling libft...\033[0m'
	${MAKE} -C ./libft
	@echo -e '\033[32mlibft compiled successfully!\033[0m'

$(NAME): $(OBJ) libft/libft.a
	$(CC) -o $(NAME) $+ $(LIB)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ)
	${MAKE} -C ./libft clean

fclean: clean
	rm -f $(NAME)
	${MAKE} -C ./libft fclean

re: fclean all
