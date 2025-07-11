RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
RESET	= \033[0;37m
SKY		= \033[1;36m

NAME = ircserv
SRC_DIR = srcs/
OBJ_DIR = build/

SUBDIRS = inputValidator/ \
		ircServer/ \
		utils/ \
		client/ \
		eventDispatcher/ \
		clientManager/ \
		exceptions/ \
		pollManager/ \
		iEventStrategy/ \
		acceptStrategy/ \
		errorStrategy/ \
		msgStrategy/ \
		cmdRegistry/ \
		cmdDispatcher/ \
		ACommand/ \
		commands/ \
		cmdParser/ \
		channel/ \
		channelManager/ \
		replies/

INCLPATH = includes/

SRCDIRS = $(addprefix $(SRC_DIR)/, $(SUBDIRS))
SRCS = $(notdir $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))) $(notdir $(SRC_DIR)/main.cpp)
OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic
DEBUG =  -fsanitize=address -g3

HEADERS     = $(shell find $(INCLPATH) -type f -name '*.hpp')
CMP_HEADERS = $(sort $(patsubst %,-I%,$(dir $(HEADERS))))

all : $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ_DIR) $(OBJ) $(HEADERS)  Makefile
	@echo "$(YELLOW) Compiling $(NAME) $(RESET)"
	@$(CC) $(CFLAGS) $(DEBUG) $(CMP_HEADERS) $(OBJ) -o $(NAME)
	@echo "$(GREEN) Executable file has been created $(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) Makefile
	@$(CC) $(DEBUG) $(CFLAGS) $(CMP_HEADERS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/*/%.cpp $(HEADERS) Makefile
	@$(CC) $(DEBUG) $(CFLAGS) $(CMP_HEADERS) -c $< -o $@

clean :
	@rm -rf $(OBJ_DIR)
	@echo "$(RED) Object files have been deleted $(RESET)"

fclean : clean
	@rm -f $(NAME)
	@echo "$(RED) Executable file has been deleted $(RESET)"

re : fclean all

test: all
	./ircserv 6667 password

push:
	@read -p "Enter branch name: " branch; \
	read -p "Enter commit message: " msg; \
	git add .; \
	git commit -m "$$msg"; \
	git push -u origin "$$branch"

.PHONY : all clean fclean re push
