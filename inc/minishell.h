/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   msh.h                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/31 15:32:15 by avaliull     #+#    #+#                  */
/*   Updated: 2025/08/06 15:24:33 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*	readline	*/ 
/* for waits	*/
/*	signals		*/
/* for true/false macro (and bool if we need it) */
# include <stdbool.h>

/*		Standard/Default values		*/
# define STD_PATH "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
// max value for environment variables
# define PATH_MAX 4096 // change name?
# define ENV_MAX 4096
/**/

/*		Error Macros	*/
# define MALLOC_ERR "Error! malloc()"
# define OVERFLOW_ERR "Error! Integer overflow"
# define PIPE_ERR "Error! pipe()"
# define FORK_ERR "Error! fork()"
# define READLINE_ERR "Error! readline()"
# define PATH_ERR "Error! PATH not found"
# define DUP_ERR "Error! dup()"
# define DUP2_ERR "Error! dup2()"
# define EXECVE_ERR "Error! execve()"
# define FD_ERR "Error! Wrong file descriptor"
/**/

typedef enum	e_msh_errno // can define specific values later
{
	success = 0,
	malloc_err = -1,
	overflow_err = -2,
	pipe_err = -3,
	fork_err = -4,
	readline_err = -5,
	path_err = -6,
	dup_err = -7,
	dup2_err = -8,
	fd_err = -9,
	builtin_err = -10,
	execve_err = -11,
	child_error_delim = -12,
	child_fd_err = -13,
	child_heredoc = -14,
	child_success = -15,
	no_command = -16,
}	t_msh_errno;

# define LIBFUNC_ERR 0
# define MINISHELL_ERR 1

# define EXIT_MSH -3
# define RETURN_CONTROL -2
# define CONTINUE_EXEC -1
# define IGNORE_ERR 0

/**/


// this value is returned in a child process that only exists to create heredoc
// doesn't need to be -2

/*		Signal handling		*/

static int	g_msh_signal;

void	handle_signals_interactive(
	void
);
void	handle_signals_non_interactive(
	void
);
/**/

/*		Executor - data structures		*/

typedef enum	e_redirect_type
{
	input,
	heredoc,
	append,
	trunc,
}	t_redirect_type;

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
//	char				*src_filename; // equal to NULL if filename wasn't provided
	char				*heredoc_delim; // null or delim, will be used to check if input is heredoc
	struct s_redir_list	*next;
}	t_redir_list;

void	clean_redir_list(
	t_redir_list **head,
	t_redir_list *cur_node
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
typedef struct	msh_data
{
	char		**env;
	int			env_var_count;
	int			env_mem;
	bool		is_parent;
	t_exec_data	*exec_data;
	int			command_count;
	int			last_pipeline_return;
	char		*cur_dir;
}	t_msh_data;

void	free_and_close_exec_data(
	t_exec_data	*exec_data
);

/**/

// Error management logic
// error_prefix is for stuff to print before error message, like error during a builtin
// relation will specify behavior for different errors.
// function errors (extern_err) will use native perror() to print their messages.
// all other errors will put a "msh: " prefix automatic.
// generic_err is for an error that doesn't need specific prefix
// other two values are for parse_err and exec_err
/*		Error Mananagement Structures	*/
typedef enum	e_error_relation
{
	extern_err,
	msh_err,
	parse_err,
	exec_err,
}	t_error_relation;

/**/

/*		Error Mananagement Functions	*/
void	msh_perror(
	char *const error_prefix,
	char *const error_msg,
	t_error_relation relation
);
void	clean_exit(
	t_msh_data *msh_data,
	char *read_line,
	int exit_code,
	bool silent_exit
);
/**/

/*	TEST macros	*/	
# define RED "\033[31"
# define GREEN "\033[32"
# define BLUE "\033[34"
# define YEL "\033[34"
# define CYAN "\033[36"
# define NCLR "\033[0m"

#endif
