NAME     = exams
CXX      = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -Iincludes -MMD -MP

SRC_DIR  = src
OBJ_DIR  = obj

SRC      = main.cpp Student.cpp Course.cpp Exam.cpp Graph.cpp \
           University.cpp Scheduler.cpp Storage.cpp utils.cpp
OBJ      = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
DEP      = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re