#******************************************************************************#
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: pacovali@codam.nl                            +#+                      #
#                                                    +#+                       #
#    Created: 2020/01/01 00:00:42 by pacovali      #+#    #+#                  #
#    Updated: 2020/01/01 00:00:42 by pacovali      ########   odam.nl          #
#                                                                              #
#******************************************************************************#

NAME = ft_contrast

SRCS = main.c convert_image.c working_function.c
OBJS = main.o convert_image.o working_function.o

HDRS = -I . ./libft/libft.h ./libtp/ft_thpool.h
LFT = libft/libft.a
LTP = libtp/libtp.a

MKOBJ = @gcc -c
FLAGS = -Wall -Wextra -Werror
MKEXE = gcc -o $(NAME)

all: $(NAME)
$(NAME):
	@make -C libft
	@make -C libtp
	@$(MKOBJ) $(FLAGS) $(HDRS) $(SRCS)
	@$(MKEXE) $(OBJS) $(LFT) $(LTP)

clean:
	@make -C libft clean
	@make -C libtp clean
	@rm -f $(OBJS) *~

fclean: clean
	@make -C libft fclean
	@make -C libtp fclean
	@rm -f $(NAME)

re: fclean all
