# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: flevesqu <flevesqu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/26 15:52:41 by flevesqu          #+#    #+#              #
#    Updated: 2019/03/12 02:46:40 by flevesqu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = abstract_vm

FILES =	main.cpp\
		Operand.cpp\
		OperandFactory.cpp\
		AbstractVM.cpp\
		VMStack.cpp\

SRC_DIR		= src/
OBJ_DIR		= obj/
INCLUDES	= includes/

CC			= clang++ -std=c++11

WFLAGS		= -Wall -Werror -Wextra

SRC = $(addprefix $(SRC_DIR),$(FILES))
OBJ = $(addprefix $(OBJ_DIR),$(FILES:.cpp=.o))

all : $(OBJ_DIR) $(NAME)

obj/%.o: src/%.cpp
	$(CC) $(WFLAGS) -I $(INCLUDES) -c $< -o $@

$(NAME) : $(OBJ)
	$(CC) -o $@ $(OBJ) $(WFLAGS)

$(OBJ_DIR) :
	mkdir $@

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re
