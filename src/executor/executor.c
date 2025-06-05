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

// TEST
t_exec_data	*test_get_dummy_exec_data(void)
{
	t_exec_data	*exec_data;

	int i = 0;
	int len = 3;
	exec_data = ft_calloc(3, sizeof(t_exec_data));
	exec_data[i].len = len;
	exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = NULL;
	exec_data[i].argv[2] = NULL;
	exec_data[i].input_type = custom_fd;
	exec_data[i].output_type = pipe_write;
	exec_data[i].in_filename = ft_strdup("infile");
	i++;

	exec_data[i].len = len;
	exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = ft_strdup("-e");
	exec_data[i].argv[2] = NULL;
	exec_data[i].input_type = pipe_read;
	exec_data[i].output_type = pipe_write;
	exec_data[i].redirect_type = append;
	exec_data[i].out_filename = ft_strdup("midfile");
	i++;

	exec_data[i].len = len;
	exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = ft_strdup("-b");
	exec_data[i].argv[2] = NULL;
	exec_data[i].input_type = pipe_read;
	exec_data[i].output_type = custom_fd;
	exec_data[i].redirect_type = append;
	exec_data[i].out_filename = ft_strdup("outfile");
	i++;
	return (exec_data);
}

// IO CHAIN
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
		*out_fd = open(command->out_filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	else if (command->output_type == custom_fd
		&& command->redirect_type == append)
		*out_fd = open(command->out_filename, O_WRONLY | O_CREAT | O_APPEND, 0664);
	else if (command->output_type == std_out)
		*out_fd = STDOUT_FILENO;
	else if (command->output_type == std_err)
		*out_fd = STDERR_FILENO;
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
		err_check = pipe(command_io[i].out_pipe);
		if (err_check < 0)
		{
			// ADD ERROR MANAGEMENT!!
			printf("PLACEHOLDER, THIS SHOULD ERROR\n");
		}
		command_io[i].out_fd = command_io[i].out_pipe[1];
		command_io[i + 1].in_fd = command_io[i].out_pipe[0];
		command_io[i + 1].in_pipe[0] = command_io[i].out_pipe[0];
		command_io[i + 1].in_pipe[1] = command_io[i].out_pipe[1];
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

// CHILDREN
int	spawn_children(t_exec_data *command, t_command_io *command_io, int i)
{
	char	**path_var;
	pid_t	process_id;
//	printf("command: %s\n", command->argv[0]);
//	printf("input type: %d\n", command->input_type);
//	printf("command in_fd: %d\n", command_io[i].in_fd);
//	printf("output type: %d\n", command->output_type);
//	printf("command out_fd: %d\n", command_io[i].out_fd);
	path_var = find_env_path();
	process_id = fork();
	if (process_id == 0)
	{
		printf("i: %d\n", i);
		if (command->input_type == pipe_read)
		{
			printf("inpipe 0: %d\n", command_io[i].in_pipe[0]);
			printf("inpipe 1: %d\n", command_io[i].in_pipe[1]);
			dup2(command_io[i].in_pipe[0], STDIN_FILENO);
			close(command_io[i].in_pipe[1]);
		}
		else if (command->input_type != std_in)
			dup2(command_io[i].in_fd, STDIN_FILENO);

		if (command->output_type == pipe_write)
		{
			printf("outpipe 1: %d\n", command_io[i].out_pipe[1]);
			printf("outpipe 0: %d\n", command_io[i].out_pipe[0]);
			printf("next inpipe 1: %d\n", command_io[i + 1].in_pipe[1]);
			printf("next inpipe 0: %d\n", command_io[i + 1].in_pipe[0]);
			dup2(command_io[i].out_pipe[1], STDOUT_FILENO);
			close(command_io[i].out_pipe[0]);
		}
		else if (command->output_type != std_out)
		{
			printf("what?\n");
			dup2(command_io[i].out_fd, STDOUT_FILENO);
		}
		try_execve((const char **) path_var, command->argv);
		exit(9999);
	}
	else if (process_id > 0)
	{
		if (command->input_type == pipe_read)
		{
			close(command_io[i].in_pipe[1]);
			close(command_io[i].in_pipe[0]);
		}
		if (command->output_type == pipe_write)
		{
			close(command_io[i].out_pipe[1]);
			close(command_io[i].out_pipe[0]);
		}
//		if ((command)->output_type == pipe_write
//			&& (command + 1)->input_type == pipe_read)
//			waitpid(process_id, NULL, 0);
//		printf("this wait?\n");
		return (0);
	}
	else
	{
		// ADD ERROR MANAGEMENT!!
		printf("PLACEHOLDER, THIS SHOULD ERROR\n");
		return (1);
	}
	return (0);
}

int	executor(t_exec_data *exec_data, int dummy_minishell_struct)
{
	int				i;
	int				process_status;
	t_command_io	*io_chain;

	io_chain = ft_calloc(exec_data->len, sizeof(t_command_io));
	i = -1;
	while (++i < exec_data->len)
		prepare_io_chain(&exec_data[i], io_chain, i);
	i = -1;
	while (++i < exec_data->len)
		spawn_children(&exec_data[i], io_chain, i);
	wait(&process_status);
	printf("we have waited\n");
	return (1);
}
