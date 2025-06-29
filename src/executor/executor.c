/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   executor.c                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/03 15:24:57 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/12 19:30:26 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
// TEST - function with test_ in name will be either deleted or modified later
void	free_and_close_redir_list(
	t_redir_list *redirection
)
{
	while (redirection != NULL)
	{
		if (redirection->dest_filename)
			free(redirection->dest_filename);
		if (redirection->src_filename)
			free(redirection->src_filename);
		if (redirection->heredoc_delim)
			free(redirection->src_filename);
		if (redirection->type == heredoc)
			test_close(redirection->dest_fd);
		free (redirection);
		redirection = redirection->next;
	}
}
void	test_free_and_close_exec_data(
	t_exec_data	*exec_data
)
{
	free_and_close_redir_list(exec_data->redirections);
	if (exec_data->argv)
		free_2d_arr((void **) exec_data->argv);
}

int	test_dummy_builtin(
	const t_exec_data *exec_data
)
{
	printf("Dummy built-in executed: %s\n", exec_data->argv[0]);
	return (0);
}

t_redir_list	*test_add_redirection(
	const t_redir_list *first,
	const t_redirect_type type,
	const int src,
	const char *dest,
	const char *heredoc_delim
	)
{
	t_redir_list	*cur_node = (t_redir_list *) first;
	t_redir_list	*redir_list = ft_calloc(1, sizeof(*redir_list));

	redir_list->type = type;
	redir_list->src_fd = src;
	if (dest)
		redir_list->dest_filename = ft_strdup(dest);
	if (heredoc_delim)
		redir_list->heredoc_delim = ft_strdup(heredoc_delim);
	redir_list->next = NULL;
	if (first != NULL)
	{
		while (cur_node->next != NULL)
			cur_node = cur_node->next;
		cur_node->next = redir_list;
	}
	else
		first = redir_list;
	return ((t_redir_list *) first);
}

t_exec_data	*test_get_dummy_exec_data(int len)
{
	t_exec_data	*exec_data;

	len = 4;
	int i = 0;
	exec_data = ft_calloc(len + 1, sizeof(t_exec_data));
	exec_data[i].argv = ft_calloc(10, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("ca");
	//exec_data[i].argv[0] = ft_strdup("ls");
	//exec_data[i].argv[1] = ft_strdup("-l");
	exec_data[i].is_builtin = false;
	exec_data[i].input_is_pipe = false;
	exec_data[i].output_is_pipe = true;
	//exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, heredoc, STDIN_FILENO, NULL, "EOF");
	exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, input, STDIN_FILENO, "infile", NULL);
	//exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, heredoc, STDIN_FILENO, NULL, "EOF2");
	i++;

	exec_data[i].argv = ft_calloc(10, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = ft_strdup("-e");
	exec_data[i].is_builtin = false;
	exec_data[i].input_is_pipe = true;
	exec_data[i].output_is_pipe = true;
	i++;

	exec_data[i].argv = ft_calloc(10, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = ft_strdup("-T");
	exec_data[i].is_builtin = false;
	exec_data[i].input_is_pipe = true;
	exec_data[i].output_is_pipe = true;
	i++;

	exec_data[i].argv = ft_calloc(10, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = ft_strdup("-b");
	exec_data[i].is_builtin = false;
	exec_data[i].input_is_pipe = true;
	exec_data[i].output_is_pipe = false;
	exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, append, STDOUT_FILENO, "outfile", NULL);
	i++;

	//exec_data[i].len = len;
	//exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	//exec_data[i].argv[0] = ft_strdup("cat");
	//exec_data[i].argv[1] = ft_strdup("-b");
	//exec_data[i].argv[2] = NULL;
	//exec_data[i].is_builtin = true;
	//exec_data[i].input_type = std_in;
	//exec_data[i].output_type = std_out;
	//exec_data[i].redirect_type = trunc;
	//exec_data[i].heredoc_delim = NULL;
	//exec_data[i].out_filename = ft_strdup("outfile");
	//i++;

	return (exec_data);
}

// CHILDREN


static int	cleanup_in_parent_process(
	const t_exec_data *command,
	t_command_io *const command_io
)
{
	t_redir_list	*redirection;

	redirection = command->redirections;
	if (command->input_is_pipe == true)
		test_close(command_io->in_pipe[READ_END]);
	if (command->output_is_pipe == true)
		test_close(command_io->out_pipe[WRITE_END]);
	test_free_and_close_exec_data((t_exec_data *) command);
	return (0);
}

static int	run_child_process(
	const t_exec_data *command,
	const t_command_io *command_io,
	const char **path
)
{
	int				err_check;

	if (command->input_is_pipe == true)
	{
		test_dup2(command_io->in_pipe[READ_END], STDIN_FILENO);
		test_close(command_io->in_pipe[READ_END]);
	}
	if (command->output_is_pipe == true)
	{
		test_dup2(command_io->out_pipe[WRITE_END], STDOUT_FILENO);
		test_close(command_io->out_pipe[WRITE_END]);
	}
	perform_redirections(command->redirections, command_io);
	if (command->is_builtin == false)
		try_execve(path, command->argv);
	else if (command->is_builtin == true)
		err_check = test_dummy_builtin(command);
	exit(err_check);
}

int	execute_command(
	const t_exec_data *command,
	t_command_io *const command_io,
	const char **path
)
{
	pid_t	process_id;

	process_id = fork();
	if (process_id == 0)
		run_child_process(command, command_io, path);
	else if (process_id > 0)
		cleanup_in_parent_process(command, command_io);
	else if (process_id < 0)
		perror_and_return(FORK_ERR, LIBFUNC_ERR, EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	executor(
	const t_exec_data *exec_data,
	int command_count,
	int dummy_minishell_struct
)
{
	int				i;
	int				process_status;
	t_command_io	command_io;
	const char		**path = find_env_path();

	if (!path)
		return (EXIT_FAILURE);
	i = -1;
	// CURRENT LOGIC: fail in a process wil never stop execution of other commands. last exittable fail check was during pipe setup
	while (++i < command_count)
	{
		if (prepare_command_io(&exec_data[i], &command_io) < 0)
		{
			// QUESTION: WHAT IS THE BEHAVIOUR IF HEREDOC CAN'T BE CREATED?
			printf("PLACEHOLDER ERROR\n");
			return (EXIT_FAILURE);
		}
		execute_command(&exec_data[i], &command_io, path);
	}
	while (waitpid(-1, &process_status, 0) > 0) // check if there's some exit signals or codes we need to handle here
	{
		if (WIFEXITED(process_status) == true)
			if (WEXITSTATUS(process_status) != EXIT_SUCCESS)
			{
				// this should probably be removed and only relevant things need to be shown
				// check gnu manual? I don;t know where to find things that are re;evan0t
				//printf("exit status: %d\n", process_status == EBADF);
				//perror("PLACEHOLDER, Child process sent an error");
			}
	}
	return (EXIT_SUCCESS);
}
