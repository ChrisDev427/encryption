# Makefile


NAME = encryptFile


CC = c++
CFLAGS = -std=c++17 -stdlib=libc++ -Wall -Wextra -Werror #-fsanitize=address

SRCS = ./srcs/main.cpp \
	   ./srcs/EncryptFile.cpp \
	   ./srcs/utils.cpp


OBJECT_FILES = $(SRCS:.cpp=.o)
# HEADER = -I
all: $(NAME) 

%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(YELLOW)Compiling $(NAME): << $< >>\033[K\r$(DEFAULT)"

$(NAME): $(OBJECT_FILES)
	@$(CC) $(CFLAGS) -o $(NAME) $(SRCS) 

#------------------------------------------------------------------------------
clean: 
	@rm -rf $(OBJECT_FILES)
	@echo "$(GREEN)<<<<< clean from $(NAME) done ! >>>>>\n$(DEFAULT)"

#------------------------------------------------------------------------------
fclean: clean
	@rm -rf $(NAME)
	@echo "$(GREEN)<<<<< fclean done ! >>>>>\n$(DEFAULT)"
#------------------------------------------------------------------------------
re: fclean all
	@echo "$(GREEN)<<<<< $(NAME) make re done ! >>>>>\n$(DEFAULT)"
	
#------------------------------------------------------------------------------

#COLORS
RED = \033[3;31m
GREEN = \033[3;32m
YELLOW = \033[3;33m
DEFAULT = \033[0m
#------------------------------------------------------------------------------
#DEBUG
# -fsanitize=address
# valgrind --leak-check=full
# find . -type f -name "*.o" -delete