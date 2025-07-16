# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/17 08:32:59 by hbenazza          #+#    #+#              #
#    Updated: 2025/07/16 20:00:58 by kbassim          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserver

SRC = $(wildcard *.cpp) $(wildcard ParsingConfigFile/*.cpp) $(wildcard Includes/*.cpp)

HEADER = $(wildcard *.hpp) $(wildcard Includes/*.hpp)

OBJ = ${SRC:.cpp=.o}

CC = c++

CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -g3 #-fsanitize=address

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
	@valgrind --track-fds=yes ./$(NAME)


re:fclean $(NAME)

all:$(NAME)
