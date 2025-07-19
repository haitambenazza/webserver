# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hbenazza <hbenazza@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/17 08:32:59 by hbenazza          #+#    #+#              #
#    Updated: 2025/07/19 02:26:20 by hbenazza         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserver

CONFIG = lol.conf

SRC = $(filter-out client.cpp, $(wildcard */*.cpp) $(wildcard *.cpp))

HEADER = $(wildcard */*.hpp)

OBJ = ${SRC:.cpp=.o}

CC = c++

CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -g3 

%.o: %.cpp
	echo $(SRC)
	$(CC) $(CXXFLAGS) -c $^ -o $@

$(NAME): $(SRC) $(OBJ)
	$(CC) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -rf $(OBJ)
	@echo "objects are removed"

fclean:
	@rm -rf $(OBJ) $(NAME)
	@echo "objects and executable are removed"

debugg: $(NAME)
	@valgrind --track-fds=yes ./$(NAME) $(CONFIG)


re:fclean $(NAME)

all:$(NAME)
