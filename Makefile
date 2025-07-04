# **************************************************************************** #
#                                                                              #
#                                                        ::::::::              #
#    Makefile                                          :+:    :+:              #
#                                                     +:+                      #
#    By: avaliull <avaliull@student.codam.nl>        +#+                       #
#                                                   +#+                        #
#    Created: 2025/05/21 19:45:55 by avaliull     #+#    #+#                   #
#    Updated: 2025/06/12 17:29:38 by avaliull     ########   odam.nl           #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := all
MAKEFLAGS =

NAME	=	minishell

CFILES	=	minishell.c\
			errors.c\
			executor.c\
			command_io_setup.c\
			redirections.c\
			here-doc.c\
			try_execve.c\
			executor_test.c\
			test_funcs.c

OFILES	= $(addprefix $(BUILDDIR),$(CFILES:.c=.o))
DEPFILES	= $(addprefix $(BUILDDIR),$(CFILES:.c=.d))

VPATH	= $(INCLUDE) $(SRCDIRS)
BUILDDIR = build/
SRCDIR = src/
LIBDIR = lib/
INCDIR = inc/
SRCDIRS = $(addprefix $(SRCDIR), parsing executor) $(SRCDIR)
$(SRCDIR):
	mkdir -p $@
$(LIBDIR):
	mkdir -p $@
$(INCDIR):
	mkdir -p $@
$(BUILDDIR):
	mkdir -p $@

LIBFT_DIR = $(LIBDIR)libft/
LIBFT	= $(LIBFT_DIR)libft.a
INCLUDE = $(INCDIR) $(LIBFT_DIR)

RM	= rm -rf
CC	= cc

# this apparently achieves dependency gen on per-file basis, on compilation
# without relying on hacky solutions like the one on GNU Makefile docs
# -MMD is a combination of a few flags:
#  -M generates dependency files
#  -MD is the same as -M -MF file
#  -MF file specifies file to write dependencies to
#  -MP adds a phony target so make does not cry when a .h file is removed
CPPFLAGS	= $(INCFLAGS) -MMD -MP

INCFLAGS	= $(addprefix -I,$(INCLUDE))
CFLAGS	= -Wall -Wextra -Werror
LDFLAGS	= -lreadline
INPUT	=


-include $(OFILES:.o=.d)

# builds .d files, then builds .o files based on .d.
# skips files that weren't changed (see CPPFLAGS)
$(BUILDDIR)%.o: %.c $(INCLUDE) | $(BUILDDIR)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) all -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OFILES)
	$(CC) $(CFLAGS) -o $@ $(OFILES) $(LIBFT) $(LDFLAGS) $(INCFLAGS)

#Base/project requirements
all: $(NAME)
libs_clean:
	$(MAKE) fclean -C $(LIBFT_DIR)
clean:
	$(RM) $(OFILES)
fclean:	clean libs_clean
	$(RM) $(NAME) $(DEPFILES)
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
test: $(NAME) run
run:
	./$(NAME) $(INPUT)
leak:	debug
	valgrind  --suppressions=minishell.supp -s --leak-check=full \
	--show-leak-kinds=all --track-fds=yes ./$(NAME) $(INPUT)
val:
	valgrind  --suppressions=minishell.supp -s --leak-check=full \
	--show-leak-kinds=all --track-fds=yes ./$(NAME) $(INPUT)

.PHONY:	clangd all clean fclean re libs_clean test run leak debug gdb
