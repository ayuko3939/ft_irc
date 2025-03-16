NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIR = srcs
SRCS = main.cpp \
		Client.cpp \
		ManageServer.cpp \
		ManageServerUtils.cpp \
		Server.cpp \
		Channel.cpp \
		parsing.cpp \
		utils.cpp \
		commands/join.cpp \
		commands/nick.cpp \
		commands/pass.cpp \
		commands/topic.cpp \
		commands/user.cpp \
		commands/cmmandUtils.cpp \
		commands/modes/topic.cpp


OBJ_DIR = objs
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

INCS = -I include

GREEN = \033[38;5;082;1m
END = \033[0m

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Compiling $(NAME)...$(END)"
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "$(GREEN)Removing object files...$(END)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(GREEN)Removing $(NAME)...$(END)"
	@rm -rf $(NAME)

re: fclean all