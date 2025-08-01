/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   minishell.h                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/31 15:32:15 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 20:16:12 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*	readline	*/ 
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h> /* isatty */
/* for waits	*/
# include <sys/types.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/wait.h>
/*	signals		*/
# include <signal.h>
/* for true/false macro (and bool if we need it) */
# include <stdbool.h>

/*		Error Macros	*/

# define MALLOC_ERR "Error! malloc()"
# define PIPE_ERR "Error! pipe()"
# define FORK_ERR "Error! fork()"
# define READLINE_ERR "Error! readline()"
# define PATH_ERR "Error! PATH not found"
# define DUP_ERR "Error! dup()"
# define DUP2_ERR "Error! dup2()"
# define FD_ERR "Error! Wrong file descriptor"

# define LIBFUNC_ERR 0
# define MINISHELL_ERR 1

# define EXIT_MSH -3
# define RETURN_CONTROL -2
# define CONTINUE_EXEC -1
# define IGNORE_ERR 0

/**/


# define MAX_ENV 4096

/*		Executor - data structures		*/

typedef enum	e_redirect_type
{
	input,
	heredoc,
	append,
	trunc,
}	t_redirect_type;

// t_redir_list will be assigned default values at start up.
// Parser will set available values for execution.
// Execution will free/reset defaults after each execution cycle.
// QUESTION: use buffers for filenames instead to avoid reallocs?
// this list assumes that the source of a redirection is either unspecified,
// specified as an integer, or opened from an env/direct path during parsing
// here are examples of what is a source and what is a destionation:
// [source] > [dest]
// [dest] < [source]
// [dest] << [heredoc as source]
// > [dest] - source unspecified, so defaults to source = stdout
// < [src] - dest unspecified, so defaults to dest = stdin
// << [heredoc as source] - dest unspecified, so defaults to dest = stdin
typedef struct	s_redir_list
{
	t_redirect_type		type;
	int					src_fd; // equal to -1 if fd not provided/not default
	int					dest_fd; // equal to -1 if fd not provided/not default
	char				*dest_filename; // equal to NULL if filename wasn't provided
	char				*src_filename; // equal to NULL if filename wasn't provided
	char				*heredoc_delim; // null or delim, will be used to check if input is heredoc
	struct s_redir_list	*next;
}	t_redir_list;

void	free_and_close_redir_list(
	t_redir_list *redirection
);

// Since parser will read the builtin name, makes sense to save name in enum
// to avoid redundant parsing of the name
typedef enum	e_builtin_name
{
	not_builtin,
	builtin_echo,
	builtin_cd,
	builtin_pwd,
	builtin_export,
	builtin_unset,
	builtin_env,
	builtin_exit,
}	t_builtin_name;

// This struct contains all information executor needs to execute a pipeline
// It's contents are filled during parsing, and NULLed/freed at the end of
// the execution cycle.
// The struct itself is located on the stack, the pointer to it will be reused
// again for consecutive cycles.
// QUESTION: make buffers to avoid reallocation on each execution cycle?
// smart choice, not technically necessary
// is_builtin bool not technically necessary but seems more readable to me then
// checking builtin_name to see if it is one
typedef struct	s_exec_data
{
	char			**argv;
	bool			is_builtin;
	t_builtin_name	builtin_name;
	bool			input_is_pipe;
	bool			output_is_pipe;
	t_redir_list	*redirections;
}	t_exec_data;

// This struct contains data that maybe accessed and modified by both parsing
// and execution modules at any point, and does not get deleted/freed after
// each execution cycle.
// env is cloned at start and contains environment.
// last_pipleline_return starts at 0 and stores return value of last pipeline.
typedef struct	minishell_data
{
	char	**env;
	int		env_var_count;
	int		env_mem;
	int		last_pipeline_return;
}	t_minishell_data;

void	free_and_close_exec_data(
	t_exec_data	*exec_data
);

/**/

// Error management logic
// error_prefix is for stuff to print before error message, like error during a builtin
// relation will specify behavior for different errors.
// function errors (extern_err) will use native perror() to print their messages.
// all other errors will put a "minishell: " prefix automatic.
// generic_err is for an error that doesn't need specific prefix
// other two values are for parse_err and exec_err
/*		Error Mananagement Structures	*/
typedef enum	e_msh_errno
{
	TEST_PLACEHOLDER,
}	t_msh_errno;

typedef enum	e_error_relation
{
	extern_err,
	generic_err,
	parse_err,
	exec_err,
}	t_error_relation;

/**/

/*		Error Mananagement Functions	*/
int	perror_and_return(
	char *const error_prefix,
	char *const error_msg,
	t_error_relation relation,
	int return_value
);
void	clean_exit(
	t_exec_data *exec_data,
	t_minishell_data *minishell_data,
	char *read_line,
	int exit_code
);

/**/

#endif
