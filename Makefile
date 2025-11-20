# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pifourni <pifourni@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/20 14:33:23 by pifourni          #+#    #+#              #
#    Updated: 2025/11/20 14:41:50 by pifourni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIB = 
SRC = ./pipex.c \
	  ./utils.c
OBJ = $(SRC:.c=.o)

NAME = pipex

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $+

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
