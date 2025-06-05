/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   executor.c                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/03 15:24:57 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/03 18:05:31 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

t_exec_data	*test_get_dummy_exec_data(void)
{
	t_exec_data	*exec_data;

	exec_data = ft_calloc(3, sizeof(t_exec_data));
	exec_data[0].len = 2;
	exec_data[0].argv = ft_calloc(3, sizeof(char *));
	exec_data[0].argv[0] = ft_strdup("cat");
	exec_data[0].argv[1] = ft_strdup("-b");
	exec_data[0].input_type = custom_fd;
	exec_data[0].output_type = pipe_write;
	exec_data[0].in_filename = ft_strdup("infile");
	exec_data[1].len = 2;
	exec_data[1].argv = ft_calloc(3, sizeof(char *));
	exec_data[1].argv[0] = ft_strdup("cat");
	exec_data[1].argv[1] = ft_strdup("-e");
	exec_data[1].input_type = pipe_read;
	exec_data[1].output_type = custom_fd;
	exec_data[1].redirect_type = append;
	exec_data[1].out_filename = ft_strdup("outfile");
	return (exec_data);
}

void	set_in_fd(t_exec_data *command, int *in_fd)
{
	if (command->input_type == custom_fd)
		*in_fd = open(command->in_filename, O_RDONLY);
	else if (command->input_type == std_in)
		*in_fd = STDIN_FILENO;
}

void	set_out_fd(t_exec_data *command, int *out_fd)
{
	if (command->output_type == custom_fd
		&& command->redirect_type == trunc)
		*out_fd = open(command->out_filename, O_WRONLY | O_CREAT | O_TRUNC);
	else if (command->output_type == custom_fd
		&& command->redirect_type == append)
		*out_fd = open(command->out_filename, O_WRONLY | O_CREAT | O_APPEND);
	else if (command->output_type == std_out)
		*out_fd = STDOUT_FILENO;
	else if (command->output_type == std_err)
		*out_fd = STDERR_FILENO;
}

int	spawn_children(t_exec_data *command, t_command_io *command_io, int i)
{
	printf("command: %s\n", command->argv[0]);
	printf("input type: %d\n", command->input_type);
	printf("command in_fd: %d\n", command_io[i].in_fd);
	printf("output type: %d\n", command->output_type);
	printf("command out_fd: %d\n", command_io[i].out_fd);
	printf("placeholder - child process created\n");
	return (0);
}

int	prepare_io_chain(t_exec_data *command, t_command_io *command_io, int i)
{
	int	err_check;

	err_check = 0;
	if (command->input_type != pipe_read)
		set_in_fd(command, &command_io[i].in_fd);
	if (command_io[i].in_fd < 0)
	{
		// ADD ERROR MANAGEMENT!!
		printf("PLACEHOLDER, THIS SHOULD ERROR\n");
	}

	if (command->output_type == pipe_write)
	{
		err_check = pipe(
				(int [2]){
				command_io[i + 1].in_fd,
				command_io[i].out_fd
			}
		);
		if (err_check < 0)
			perror("pipe errored");
		printf("pipe read: %d\n", command_io[i + 1].in_fd);
		printf("pipe write: %d\n", command_io[i].out_fd);
	}
	else
		set_out_fd(command, &command_io[i].out_fd);
	if (command_io[i].in_fd < 0 || err_check < 0)
	{
		// ADD ERROR MANAGEMENT!!
		printf("PLACEHOLDER, THIS SHOULD ERROR\n");
	}
	return (err_check);
}

// I think I need to just iteratively setup pipes in the parent process
// then the next iteration would actually split the process into children since now we have pipe pairs
// and execution can start from 0 to N
// and if there's no pipes then it jsut uses whatever the in/out fd value is
// doest that regardless I think?
// probably special casse for heredoc but would need to research
//
int	executor(t_exec_data *exec_data, int dummy_minishell_struct)
{
	int				i;
	t_command_io	*io_chain;

	io_chain = ft_calloc(exec_data->len, sizeof(t_command_io));
	i = -1;
	while (++i < exec_data->len)
		prepare_io_chain(&exec_data[i], io_chain, i);
	i = -1;
	while (++i < exec_data->len)
		spawn_children(&exec_data[i], io_chain, i);
	return (1);
}
