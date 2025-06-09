/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   executor.h                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/03 15:25:20 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/09 18:15:14 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

# define READ_END 0
# define WRITE_END 1
typedef struct	s_command_io
{
	int	in_fd;
	int	out_fd;
	int	in_pipe[2];
	int	out_pipe[2];
}	t_command_io;

/*		Executor - temporary functions!		*/
t_exec_data	*test_get_dummy_exec_data();
int	executor(
	const t_exec_data *exec_data,
	int dummy_minishell_struct
);

/*		Here-doc		*/
int	create_here_doc(
	const t_exec_data *command,
	int here_doc[2]
);

/*		Sets up the input and output of the command		*/
int	prepare_command_io(
	const t_exec_data *command,
	t_command_io *const command_io
);

/*		Try_execve		*/
const char	**find_env_path(void);
int			try_execve(
	const char **path,
	char *const argv[]
);

#endif
