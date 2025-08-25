
NAME = webserver

CONFIG = lol.conf

SRC = $(wildcard */*.cpp) $(wildcard *.cpp)

HEADER = $(wildcard */*.hpp)

OBJ = ${SRC:.cpp=.o}

CC = c++

CXXFLAGS = -std=c++11 -g3 -Wall -Wextra -Werror

%.o: %.cpp
	@$(CC) $(CXXFLAGS) -c $^ -o $@

$(NAME): $(SRC) $(OBJ)
	@$(CC) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "the executable is ready"

clean:
	@rm -rf $(OBJ)
	@echo "objects are removed"

fclean:
	@rm -rf $(OBJ) $(NAME)
	@echo "objects and executable are removed"

debugg: $(NAME)
	@valgrind --leak-check=full --track-fds=yes ./$(NAME) $(CONFIG)


re:fclean $(NAME)

all:$(NAME)
