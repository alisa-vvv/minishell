/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   minishell.h                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/31 15:32:15 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/01 15:56:16 by avaliull     ########   odam.nl          */
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

/*		Error Management	*/

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

int	perror_and_return(
	const char *error_msg,
	int	msh_errno,
	int return_value
);

# define MAX_ENV 4096

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
	char				*src_filename; // equal to NULL if filename wasn't provided
	char				*heredoc_delim; // null or delim, will be used to check if input is heredoc
	struct s_redir_list	*next;
}	t_redir_list;

typedef struct	s_exec_data
{
	char			**argv;
	int				is_builtin;
	int				input_is_pipe;
	int				output_is_pipe;
	t_redir_list	*redirections;
}	t_exec_data;

typedef struct	minishell_data
{
	char	**environment;
	int		env_var_count;
	int		env_mem;
}	t_minishell_data;

#endif
