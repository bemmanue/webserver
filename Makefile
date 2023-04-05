NAME		= webserver

CC			= c++
FLAGS		= -Wall -Wextra -Werror -std=c++17

SRC			=	main.cpp						\
				src/config/Config.cpp			\
				src/config/LocationConfig.cpp	\
				src/config/ServerConfig.cpp		\
				src/core/Connection.cpp			\
				src/core/Server.cpp				\
				src/core/Utils.cpp				\
				src/http/Global.cpp				\
				src/http/Pages.cpp				\
				src/http/Request.cpp			\
				src/http/Response.cpp			\
				src/http/Status.cpp				\
				src/http/URI.cpp				\
				src/http/Utils.cpp

OBJ			=	$(SRC:.cpp=.o)
OBJ_DIR		=	./objects/
OBJS		=	$(addprefix $(OBJ_DIR), $(OBJ))

all:			$(NAME)

$(NAME):		$(OBJS)
				@$(CC) $(FLAGS) -o $@ $^
				@echo "\033[32m"Make DONE

$(OBJ_DIR)%.o:	%.cpp
				@mkdir -p $(OBJ_DIR) $(OBJ_DIR)/src/config $(OBJ_DIR)/src/core $(OBJ_DIR)/src/http
				@$(CC) $(FLAGS) -c $< -o $@

clean:
				@rm -rf $(OBJ_DIR)
				@echo "\033[32m"clean DONE

fclean:			clean
				@rm -rf $(NAME)
				@echo "\033[32m"fclean DONE

re:				fclean all

.PHONY:			all fclean clean re