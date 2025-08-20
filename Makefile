# **************************************************************************** #
#                                                                              #
#                                                        ::::::::              #
#    Makefile                                          :+:    :+:              #
#                                                     +:+                      #
#    By: avaliull <avaliull@student.codam.nl>        +#+                       #
#                                                   +#+                        #
#    Created: 2025/07/22 19:01:25 by avaliull     #+#    #+#                   #
#    Updated: 2025/08/06 15:25:10 by avaliull     ########   odam.nl           #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := all

NAME	=	minishell

CFILES	=	minishell.c\
			signal_handlers.c\
			errors.c\
			cleanup.c\
			env_operations.c\
			$(CFILES_VECLIB)\
			$(CFILES_PARSER)\
			$(CFILES_EXECUTOR)\
			$(CFILES_BUILTINS)\
			test_funcs.c
CFILES_VECLIB	=	vec_lib.c\
					vec_lib2.c
CFILES_PARSER	=	debug.c\
					lexer_match.c\
					expansions.c\
					hyphen_handler.c\
					lexer.c\
					lexer2.c\
					quote_handler.c\
					val_redirect.c\
					errors.c\
					cleanup.c\
					env_operations.c\
					parser_utils.c\
					parser.c
CFILES_EXECUTOR	=	executor.c\
					command_io_setup.c\
					redirections.c\
					here-doc.c\
					try_execve.c\
					executor_test.c
CFILES_BUILTINS	=	exec_builtin.c\
					pwd.c\
					cd.c\
					echo.c\
					env.c\
					export.c\
					unset.c\
					exit.c

OFILES	= $(addprefix $(BUILDDIR),$(CFILES:.c=.o))
DEPFILES	= $(addprefix $(BUILDDIR),$(CFILES:.c=.d))

VPATH	= $(INCLUDE) $(SRCDIRS)
BUILDDIR = build/
SRCDIR = src/
LIBDIR = lib/
INCDIR = inc/
SRCDIRS = $(addprefix $(SRCDIR), parsing executor builtins env_lib) \
		  $(SRCDIR)
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
CFLAGS	= -Wall -Wextra -Werror -fsanitize=undefined
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
	intercept-build-14 $(MAKE)

#debugging
debug: CPPFLAGS += -g
debug: clean $(NAME)
gdb: fclean debug
	gdb -tui ./$(NAME)
test: $(NAME) run
run:
	./$(NAME) $(INPUT)
# the suppression file is still incomplete.
# might not be worthwhile to have at all since we don't care about non-definite leaks?
#
leak:
	$(MAKE) -s debug
	valgrind --suppressions=minishell.supp --track-fds=yes --track-origins=yes \
	-s --leak-check=full --show-leak-kinds=all ./$(NAME) $(INPUT)
val:
	valgrind --track-fds=yes --track-origins=yes \
	-s --leak-check=full --show-leak-kinds=all ./$(NAME) $(INPUT)

.PHONY:	clangd all clean fclean re libs_clean test run leak debug gdb
