NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

SRCS		= main.c \
		  	  parsing.c \
		  	  init.c \
			  init_dongles.c \
		  	  coder.c \
		  	  dongle.c \
		  	  monitor.c \
		  	  heap.c \
		  	  time_utils.c \
		  	  log.c \
		  	  cleanup.c

OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re