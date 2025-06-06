/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   executor.c                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/03 15:24:57 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/05 20:15:39 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

// TEST - function with test_ in name will be either deleted or modified later
void	test_free_exec_data(t_exec_data	*exec_data)
{
	if (exec_data->argv)
		free_2d_arr((void **) exec_data->argv);
	if (exec_data->in_filename)
		free(exec_data->in_filename);
	if (exec_data->out_filename)
		free(exec_data->out_filename);
}

int	test_dummy_builtin(t_exec_data *exec_data)
{
	printf("Dummy built-in executed: %s\n", exec_data->argv[0]);
	return (0);
}

t_exec_data	*test_get_dummy_exec_data(void)
{
	t_exec_data	*exec_data;

	int i = 0;
	int len = 4;
	exec_data = ft_calloc(len, sizeof(t_exec_data));
	exec_data[i].len = len;
	exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	//exec_data[i].argv[1] = ft_strdup("-l");
	exec_data[i].argv[1] = NULL;
	exec_data[i].argv[2] = NULL;
	exec_data[i].is_builtin = false;
	exec_data[i].input_type = custom_fd;
	//exec_data[i].input_type = heredoc;
	exec_data[i].heredoc_delim = ft_strdup("EOF");
	exec_data[i].output_type = pipe_write;
	exec_data[i].in_filename = ft_strdup("infile");
	i++;

	exec_data[i].len = len;
	exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = ft_strdup("-e");
	exec_data[i].argv[2] = NULL;
	exec_data[i].is_builtin = false;
	exec_data[i].input_type = pipe_read;
	exec_data[i].output_type = pipe_write;
	exec_data[i].redirect_type = append;
	exec_data[i].heredoc_delim = NULL;
	exec_data[i].out_filename = ft_strdup("midfile");
	i++;

	exec_data[i].len = len;
	exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = ft_strdup("-T");
	exec_data[i].argv[2] = NULL;
	exec_data[i].is_builtin = false;
	exec_data[i].input_type = pipe_read;
	exec_data[i].output_type = pipe_write;
	exec_data[i].redirect_type = append;
	exec_data[i].heredoc_delim = NULL;
	exec_data[i].out_filename = ft_strdup("midfile");
	i++;

	exec_data[i].len = len;
	exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = ft_strdup("-b");
	exec_data[i].argv[2] = NULL;
	exec_data[i].is_builtin = false;
	exec_data[i].input_type = pipe_read;
	exec_data[i].output_type = custom_fd;
	exec_data[i].redirect_type = append;
	exec_data[i].heredoc_delim = NULL;
	exec_data[i].out_filename = ft_strdup("outfile");
	i++;

	return (exec_data);
}

// IO CHAIN
int	set_in_fd(t_exec_data *command, int *in_fd, int heredoc_pipe[2])
{
	int	err_check;

	if (command->input_type == custom_fd)
		*in_fd = open(command->in_filename, O_RDONLY);
	else if (command->input_type == heredoc)
	{
		err_check = create_here_doc(command, heredoc_pipe);
		*in_fd = heredoc_pipe[0];
	}
	return (err_check);
}

int	set_out_fd(t_exec_data *command, int *out_fd)
{
	int	err_check;

	err_check = 0;
	if (command->output_type == custom_fd
		&& command->redirect_type == trunc)
		*out_fd = open(command->out_filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	else if (command->output_type == custom_fd
		&& command->redirect_type == append)
		*out_fd = open(command->out_filename, O_WRONLY | O_CREAT | O_APPEND, 0664);
	else if (command->output_type == std_err)
		*out_fd = STDERR_FILENO;
	return (err_check);
}

int	prepare_io_chain(t_exec_data *command, t_command_io *command_io)
{
	int	err_check;

	err_check = 0;
	if (command->input_type == pipe_read)
	{
		command_io->in_fd = command_io->out_pipe[0];
		command_io->in_pipe[0] = command_io->out_pipe[0];
		command_io->in_pipe[1] = command_io->out_pipe[1];
	}
	else
	{
		set_in_fd(command, &command_io->in_fd, command_io->in_pipe);
	}
	if (command_io->in_fd < 0)
	{
		// ADD ERROR MANAGEMENT!!
		printf("PLACEHOLDER, THIS SHOULD ERROR\n");
	}

	if (command->output_type == pipe_write)
	{
		printf("out pipe prev[0]: %d\n", command_io->out_pipe[0]);
		printf("out pipe prev[1]: %d\n", command_io->out_pipe[1]);
		command_io->out_pipe[0] = 0;
		command_io->out_pipe[1] = 0;
		err_check = pipe(command_io->out_pipe);
		if (err_check < 0)
		{
			// ADD ERROR MANAGEMENT!!
			printf("PLACEHOLDER, THIS SHOULD ERROR\n");
		}
		command_io->out_fd = command_io->out_pipe[1];
		printf("out pipe new[0]: %d\n", command_io->out_pipe[0]);
		printf("out pipe new[1]: %d\n", command_io->out_pipe[1]);
	}
	else
		set_out_fd(command, &command_io->out_fd);
	if (command_io->in_fd < 0 || err_check < 0)
	{
		// ADD ERROR MANAGEMENT!!
		printf("PLACEHOLDER, THIS SHOULD ERROR\n");
	}
	return (err_check);
}

// CHILDREN

int	spawn_children(t_exec_data *command, t_command_io *command_io)
{
	char	**path_var;
	pid_t	process_id;

	path_var = find_env_path();
	process_id = fork();
	if (process_id == 0)
	{
		if (command->input_type != std_in)
		{
			dup2(command_io->in_fd, STDIN_FILENO);
			close(command_io->in_fd);
		}
		if (command->output_type != std_out)
		{
			dup2(command_io->out_fd, STDOUT_FILENO);
			close(command_io->out_fd);
		}
		if (command->output_type == pipe_write)
			close(command_io->out_pipe[0]);

		try_execve((const char **) path_var, command->argv);
		exit(9999); // placeholder obv
	}

	else if (process_id > 0)
	{
		if (command->input_type != std_in)
			close(command_io->in_fd);
		else if (command->input_type == heredoc || command->input_type == pipe_read)
		{
			close(command_io->in_pipe[0]);
			close(command_io->in_pipe[1]);
		}
		if (command->output_type != std_out && command->output_type != std_err)
			close(command_io->out_fd);
		test_free_exec_data(command);
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
	t_command_io	command_io;

	i = -1;
	while (++i < exec_data->len)
	{
		prepare_io_chain(&exec_data[i], &command_io);
		if (spawn_children(&exec_data[i], &command_io) != 0)
		{
			// ADD ERROR MANAGEMENT!!
			printf("PLACEHOLDER, THIS SHOULD ERROR\n");
		}
	}
	while (waitpid(-1, &process_status, 0) > 0) // check if there's more exit signals we need to handle here
	{
		if (WIFEXITED(process_status))
			if (WEXITSTATUS(process_status) != 0)
			{
				// ADD ERROR MANAGEMENT!!
				printf("PLACEHOLDER, THIS SHOULD ERROR\n");
				perror("Child process error\n");
			}
	}
	return (1);
}
