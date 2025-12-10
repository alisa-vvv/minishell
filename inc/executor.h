/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   executor.h                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/03 15:25:20 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 20:26:31 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif
# define READ_END 0
# define WRITE_END 1
# define CLOSED_FD -1
// used to indicate that FD was already closed


typedef struct	s_command_io
{
	int	in_pipe[2];
	int	out_pipe[2];
}	t_command_io;

typedef struct	s_undup_list
{
	int	orig_fd;
	int	dup_fd;
	int	dest_fd;
	struct s_undup_list *prev;
	struct s_undup_list *next;
}	t_undup_list;


int	executor(
	t_msh_data *const msh_data,
	int command_count
);
/**/

/*		Here-doc		*/
int	create_here_doc(
	t_msh_data *const msh_data,
	const char *heredoc_delim
);
/**/

/*		Sets up the input and output of the command		*/
int	prepare_command_io(
	t_msh_data *const msh_data,
	const t_exec_data *command,
	t_command_io *const command_io,
	int i
);
/**/


/*		Redirections	*/
void	undup_redirections(
	t_undup_list **undup_head
);
int	perform_redirections(
	t_redir_list *redirections,
	t_undup_list **undup_list,
	bool record_redirs
);
/**/

/*		Undups		*/
int	record_undup(
	t_undup_list **undup_list_head,
	t_undup_list **cur_undup
);
void	undup_redirections(
	t_undup_list **undup_head
);
/**/

/*		Try_execve		*/
const char	*const *find_env_path(
	char **env
);
int	try_execve(
	char **env,
	char *const argv[]
);
/**/

/*		Utils		*/
void	safe_close(
	int *fd
);
/**/

/*		Test Functions		*/
void	test_close(int fd);
void	test_dup2(int oldfd, int newfd);
/**/

#endif
