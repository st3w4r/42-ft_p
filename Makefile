# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/05/13 18:05:30 by ybarbier          #+#    #+#              #
#    Updated: 2015/05/13 18:05:33 by ybarbier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#____________CONFIG____________#

NAME_CLI = client
NAME_SRV = serveur

PATH_SRC_CLI = ./src_client/
PATH_SRC_SRV = ./src_server/

PATH_INC_CLI = ./inc_client/
PATH_INC_SRV = ./inc_server/

PATH_INC_LIBFT = ./libft/includes/

INCLUDES =  -I $(PATH_INC_CLI) -I $(PATH_INC_SRV) -I $(PATH_INC_LIBFT)

CC = cc
CFLAGS = -Wall -Wextra -Werror -Ofast $(INCLUDES)
#CFLAGS = -Ofast $(INCLUDES) -g
LIBS = -L libft/ -lft

#____________FILES____________#

SRC_CLI =	ftp_client_ui.c \
			ftp_client_pi_1.c \
			ftp_client_pi_2.c \
			ftp_client_dtp.c \
			ftp_client_builtins_local.c \
			ftp_client_builtins_control.c \
			ftp_client_builtins_transfert.c \
			ftp_client_builtins_service_1.c \
			ftp_client_builtins_service_2.c \
			ftp_client_helper_1.c \
			ftp_client_helper_2.c \
			ftp_client_fs.c \
			ftp_client_conversion.c

SRC_SRV =	ftp_server_pi_1.c \
			ftp_server_pi_2.c \
			ftp_server_dtp.c \
			ftp_server_builtins_transfert.c \
			ftp_server_builtins_control.c \
			ftp_server_builtins_service_1.c \
			ftp_server_builtins_service_2.c \
			ftp_server_builtins_service_3.c \
			ftp_server_helper.c \
			ftp_server_ui.c \
			ftp_server_fs_1.c \
			ftp_server_fs_2.c \
			ftp_server_conversion.c \
			main.c

OBJ_CLI = $(addprefix $(PATH_SRC_CLI), $(SRC_CLI:.c=.o))
OBJ_SRV = $(addprefix $(PATH_SRC_SRV), $(SRC_SRV:.c=.o))

#____________RULES____________#

.PHONY: clean clean_cli clean_srv fclean fclean_cli fclean_srv re re_cli re_srv test clean_test

all: $(NAME_CLI) $(NAME_SRV)

$(NAME_CLI): $(OBJ_CLI)
	make -C libft/
	$(CC) $(OBJ_CLI) -o $(NAME_CLI) $(LIBS)

$(NAME_SRV): $(OBJ_SRV)
	make -C libft/
	$(CC) $(OBJ_SRV) -o $(NAME_SRV) $(LIBS)


#____CLEAN____#

clean: clean_cli clean_srv

clean_cli:
	rm -f $(OBJ_CLI)

clean_srv:
	rm -f $(OBJ_SRV)

#____FCLEAN____#

fclean: clean fclean_cli fclean_srv

fclean_cli: clean
	rm -f $(NAME_CLI)

fclean_srv: clean
	rm -f $(NAME_SRV)

#____RE____#

re: fclean all

re_cli: fclean_cli $(NAME_CLI)

re_srv: fclean_srv $(NAME_SRV)



#test: $(OBJ_CLI) $(OBJ_SRV) main.c
#	$(CC) main.c $(LIBS) -o $(NAME_TEST)

#clean_test:
#	/bin/rm -f $(NAME_TEST)
