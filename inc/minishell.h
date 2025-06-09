/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   minishell.h                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/31 15:32:15 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/09 19:23:04 by avaliull     ########   odam.nl          */
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
# define DUP_ERR "Error! dup()"
# define DUP2_ERR "Error! dup2()"
# define FD_ERR "Error! Wrong file descriptor"

# define LIBFUNC_ERR 0
# define MINISHELL_ERR 1

int	perror_and_return(
	const char *error_msg,
	int	msh_errno
);

/*		Executor - data structures		*/

typedef enum	e_redirect_type
{
	none,
	append,
	trunc,
}	t_redirect_type;

typedef enum	e_io
{
	std_in,
	std_err,
	std_out,
	heredoc,
	pipe_read,
	pipe_write,
	custom_fd,
}	t_io;

typedef struct	s_exec_data
{
	char			**argv;
	int				len;
	int				is_builtin;
	t_io			input_type;
	t_io			output_type;
	t_redirect_type	redirect_type;
	char			*in_filename;
	char			*out_filename;
	char			*heredoc_delim;
}	t_exec_data;

#endif
