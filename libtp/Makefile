#******************************************************************************#
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: pacovali <marvin@codam.nl>                   +#+                      #
#                                                    +#+                       #
#    Created: 2020/01/01 00:00:42 by pacovali      #+#    #+#                  #
#    Updated: 2020/01/01 00:00:43 by pacovali      ########   odam.nl          #
#                                                                              #
#******************************************************************************#

NAME = libtp.a
SRCS = tpool.c tpool_utils.c

MKOBJ = @gcc -c -Wall -Wextra -Werror
HDRS = -I .
OBJS = tpool.o tpool_utils.o


MKLIB = @ar -rc

all: $(NAME)
$(NAME):
	$(MKOBJ) $(SRCS) $(HDRS)
	$(MKLIB) $(NAME) $(OBJS)

clean:
	@rm -f $(OBJS) *~

fclean: clean
	@rm -f $(NAME)

re: fclean all