# **************************************************************************** #
#                                                                              #
#                                                        ::::::::              #
#    Makefile                                          :+:    :+:              #
#                                                     +:+                      #
#    By: avaliull <avaliull@student.codam.nl>        +#+                       #
#                                                   +#+                        #
#    Created: 2025/05/21 19:45:55 by avaliull     #+#    #+#                   #
#    Updated: 2025/05/31 15:37:07 by avaliull     ########   odam.nl           #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := all
MAKEFLAGS =

NAME	=	minishell

CFILES	=	minishell.c\
			test_funcs.c

OFILES	= $(addprefix $(OBJDIR),$(CFILES:.c=.o))

VPATH	= $(INCLUDE) $(SRCDIRS)
OBJDIR = obj/
SRCDIR = src/
LIBDIR = lib/
INCDIR = inc/
#this is a template for when we have ultiple directories in src/
#SRCDIRS = $(addprefix $(SRCDIR), rendering controls init_exit\
#		  coordinate_manipulation map_manipulation) $(SRCDIR)
SRCDIRS = $(SRCDIR)
$(LIBDIR):
	mkdir -p $@
$(INCDIR):
	mkdir -p $@
$(OBJDIR):
	mkdir -p $@

LIBFT_DIR = $(LIBDIR)libft/
LIBFT	= $(LIBFT_DIR)libft.a
INCLUDE = $(INCDIR) $(LIBFT_DIR)

RM	= rm -rf
CC	= cc
CFLAGS	= -Wall -Wextra -Werror
LDFLAGS = -lreadline
INPUT	=

#include $(OBJ:.o=.d)
#
#$(DEPDIR)/%.d	: %.c
#	$(CC) $(CPPFLAG) $(INCLUDE) -MM -MF $@ -MT $(OBJDIR)/$(addsuffix .o,$(basename $<)) $<

$(OBJDIR)%.o: %.c $(INCLUDE) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(addprefix -I,$(INCLUDE))

$(LIBFT):
	$(MAKE) all -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OFILES)
	$(CC) $(CFLAGS) -o $@ $(OFILES) $(LIBFT) $(LDFLAGS) \
		$(addprefix -I,$(INCLUDE))

#Base/project requirements
all: $(NAME)
libs_clean:
	$(MAKE) fclean -C $(LIBFT_DIR)
clean:
	$(RM) $(OFILES)
fclean:	clean libs_clean
	$(RM) $(NAME)
re:	fclean all

#LSP connection for neovim
clangd:
	$(MAKE) fclean
	intercept-build-14 make all

#debugging
debug: CFLAGS += -g
debug: clean $(NAME)
gdb: fclean debug
	gdb ./$(NAME)
test: clean $(NAME) run
run:
	./$(NAME) $(INPUT)
leak:	debug
	valgrind  --suppressions=minishell.supp -s --leak-check=full \
	--show-leak-kinds=all --track-fds=yes ./$(NAME) $(INPUT)
val:
	valgrind  --suppressions=minishell.supp -s --leak-check=full \
	--show-leak-kinds=all --track-fds=yes ./$(NAME) $(INPUT)

.PHONY:	clangd all clean fclean re libs_clean test run leak debug gdb
