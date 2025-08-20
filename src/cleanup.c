/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   cleanup.c                                         :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/18 16:33:46 by avaliull     #+#    #+#                  */
/*   Updated: 2025/08/06 15:42:59 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_testfuncs.h"

#include "libft.h"
#include "stdlib.h"
#include "executor.h"

void	clean_exit(
	t_minishell_data *minishell_data,
	char *read_line,
	int exit_code
)
{
	int	i;
	if (minishell_data)
	{
		if (exit_code == 0)
			exit_code = minishell_data->last_pipeline_return;
		if (minishell_data->exec_data)
		{
			i = -1;
			while (++i < minishell_data->command_count)
				free_and_close_exec_data(&minishell_data->exec_data[i]);
			free(minishell_data->exec_data);
		}
		if (minishell_data->env)
		{
			i = -1;
			while (minishell_data->env[++i])
				free(minishell_data->env[i]);
			free(minishell_data->env);
		}
	}
	if (read_line)
		free(read_line);
	write(STDOUT_FILENO, "exit\n", 5); // this should msybe have a check if we are in  child process
	exit (exit_code);
}

void	free_and_close_redir_list(
	t_redir_list *redirection
)
{
	while (redirection != NULL)
	{
		if (redirection->dest_filename)
		{
			free(redirection->dest_filename);
			redirection->dest_filename = NULL;
		}
		if (redirection->src_filename)
		{
			free(redirection->src_filename);
			redirection->src_filename = NULL;
		}
		if (redirection->heredoc_delim)
		{
			free(redirection->src_filename);
			redirection->src_filename = NULL;
		}
		if (redirection->type == heredoc)
		{
			test_close(redirection->dest_fd);
			redirection->dest_fd = CLOSED_FD;
		}
		free(redirection);
		redirection = redirection->next;
	}
}

void	free_and_close_exec_data(
	t_exec_data	*exec_data
)
{
	int	i;

	if (exec_data->redirections)
		free_and_close_redir_list(exec_data->redirections);
	exec_data->redirections = NULL;
	if (exec_data->argv)
	{
		i = -1;
		while (exec_data->argv[++i])
		{
			free(exec_data->argv[i]);
			exec_data->argv[i] = NULL;
		}
		free(exec_data->argv);
		exec_data->argv = NULL;
	}
}
