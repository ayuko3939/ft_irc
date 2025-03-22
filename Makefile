NAME = ircserv

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRC_DIR = srcs
SRCS = main.cpp \
		Server.cpp \
		Client.cpp \
		Channel.cpp \
		ManageServer.cpp \
		ManageServerUtils.cpp \
		parsing.cpp \
		utils.cpp \
		commands/invite.cpp \
		commands/join.cpp \
		commands/kick.cpp \
		commands/mode.cpp \
		commands/nick.cpp \
		commands/part.cpp \
		commands/pass.cpp \
		commands/privmsg.cpp \
		commands/quit.cpp \
		commands/topic.cpp \
		commands/user.cpp \
		commands/cmmandUtils.cpp \
		commands/modes/invite.cpp \
		commands/modes/key.cpp \
		commands/modes/limit.cpp \
		commands/modes/operator.cpp \
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