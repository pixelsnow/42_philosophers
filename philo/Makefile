# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vvagapov <vvagapov@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/29 21:32:50 by vvagapov          #+#    #+#              #
#    Updated: 2023/08/04 21:17:35 by vvagapov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#******************************************************************************#
# $@ and $^ are special variables that represent the target and the first	   #
# dependency:																   #
# - $@ represents the target, the file being built.							   #
# - $< represents the first dependency, the file required to build the target. #
#******************************************************************************#

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
CFLAGS_OPTIMIZED = -O2
DEBUG_FLAGS = -g -fsanitize=address -static-libsan
THREAD_DEBUG_FLAGS = -g -fsanitize=thread -static-libsan
COMPILE = $(CC) $(CFLAGS)
SRCS =	philosophers.c \
		input.c \
		utils.c \
		party_preparations.c \
		start_threads.c \
		monitoring_routine.c \
		philo_routine.c \
		cleanup.c \
		join_threads.c
OBJ_DIR = build
OBJ = $(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
INC = philosophers.h

#******************************************************************************#
# Main rule																	   #
#******************************************************************************#

all: $(NAME)

$(NAME): $(OBJ) 
	$(COMPILE) $^ -o $@ 

$(OBJ_DIR)/%.o: %.c $(INC)
	mkdir -p $(OBJ_DIR)
	$(COMPILE) -I $(INC) $< -c -o $@

#******************************************************************************#
# Debug & Optimization rules												   #
#******************************************************************************#

# Rule to compile the program with address sanitizer
debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

# Rule to compile the program with thread sanitizer
thread_debug: CFLAGS += $(THREAD_DEBUG_FLAGS)
thread_debug: all

# Rule to compile with execution speed optimization
optimized : CFLAGS += $(CFLAGS_OPTIMIZED)
optimized : all

#******************************************************************************#
# Cleaning																	   #
#******************************************************************************#

clean:
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

#******************************************************************************#
# Protection																   #
#******************************************************************************#

.PHONY: all clean fclean re debug thread_debug
