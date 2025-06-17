CXX = g++

CXXFLAGS = -Wall -Werror -Wextra -std=c++17

NAME = tcpip

SRC =	src/test.cpp
		

GREEN = \033[32m
YELLOW = \033[33m
RESET = \033[0m

OBJ_DIR = build/

OBJ = $(SRC:src/%.c=$(OBJ_DIR)%.o)

$(OBJ_DIR)%.o : src/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME) : $(OBJ)
	@$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "$(GREEN)Compilation successful ! ✅ $(RESET)"

all : $(NAME)

clean :
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)All object files cleaned. 🧹$(RESET)"

fclean : clean
	@rm -f $(NAME)
	@echo "$(YELLOW)Executable cleaned. 🧹$(RESET)"

re : fclean all

.PHONY: clean fclean re

