/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   executor.h                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/03 15:25:20 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/12 18:09:48 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

# define READ_END 0
# define WRITE_END 1
typedef struct	s_command_io
{
	int	in_pipe[2];
	int	out_pipe[2];
}	t_command_io;

/*		Executor - temporary functions!		*/
t_exec_data	*test_get_dummy_exec_data(int len);
int	executor(
	const t_exec_data *exec_data,
	int command_count,
	int dummy_minishell_struct
);

/*		Here-doc		*/
int	create_here_doc(
	const char *heredoc_delim
);

/*		Sets up the input and output of the command		*/
int	prepare_command_io(
	const t_exec_data *command,
	t_command_io *const command_io
);

int	perform_redirections(
	t_redir_list *redirections,
	const t_command_io *command_io
);

/*		Try_execve		*/
const char	**find_env_path(void);
int			try_execve(
	const char **path,
	char *const argv[]
);

/*		Test Functions		*/
void	test_close(int fd);
void	test_dup2(int oldfd, int newfd);

#endif
