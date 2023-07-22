# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lclerc <lclerc@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/22 15:37:53 by lclerc            #+#    #+#              #
#    Updated: 2023/07/22 17:26:15 by lclerc           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#******************************************************************************#
# $@ and $^ are special variables that represent the target and the first
# dependency:
# - $@ represents the target, the file being built.
# - $< represents the first dependency, the file required to build the target.
#******************************************************************************#

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
DEBUG_FLAGS = -g -fsanitize=address -static-libsan
THREAD_DEBUG_FLAG = -g -fsanitize=thread -static-libsan
COMPILE = $(CC) $(CFLAGS)
SRCS = philo.c
OBJ = $(SRCS:%.c=%.o)
INC = philosophers.h

#******************************************************************************#
# Main rule
#******************************************************************************#

all: $(NAME)

$(NAME): $(OBJ) 
	$(COMPILE) $^ -o $@ 

%.o: %.c $(INC)
	$(COMPILE) -I $(INC) $< -c -o $@

#******************************************************************************#
# Debug rules
#******************************************************************************#

# Rule to compile the program with address sanitizer
debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

# Rule to compile the program with thread sanitizer
thread_debug: CFLAGS += $(THREAD_DEBUG_FLAG)
thread_debug: all

#******************************************************************************#
# Cleaning - Deletion
#******************************************************************************#

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

#******************************************************************************#
.PHONY: all clean fclean re debug thread_debug
#******************************************************************************#

