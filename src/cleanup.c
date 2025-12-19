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

#include "unistd.h"
#include "stdlib.h"
#include "executor.h"

static void	clean_msh_data(
	t_msh_data *msh_data,
	int *exit_code,
	bool *silent_exit
)
{
	int	i;

	if (msh_data->is_parent == false)
		*silent_exit = true;
	if (msh_data->exit_code != 0)
		*exit_code = msh_data->exit_code;
	else if (*exit_code == 0)
		*exit_code = msh_data->last_pipeline_return;
	if (msh_data->cur_dir)
		free(msh_data->cur_dir);
	if (msh_data->exec_data)
	{
		i = -1;
		while (++i < msh_data->command_count)
			free_and_close_exec_data(&msh_data->exec_data[i]);
		free(msh_data->exec_data);
	}
	if (msh_data->env)
	{
		i = -1;
		while (msh_data->env[++i])
			free(msh_data->env[i]);
		free(msh_data->env);
	}
}

void	clean_exit(
	t_msh_data *msh_data,
	char *read_line,
	int exit_code,
	bool silent_exit
)
{
	if (msh_data)
		clean_msh_data(msh_data, &exit_code, &silent_exit);
	if (read_line)
		free(read_line);
	if (silent_exit == false)
		write(STDOUT_FILENO, "exit\n", 5);
	exit (exit_code);
}

void	clean_redir_list(
	t_redir_list **head,
	t_redir_list *cur_node
)
{
	t_redir_list	*next_node;

	while (cur_node != NULL)
	{
		next_node = cur_node->next;
		if (cur_node->dest_filename)
			free(cur_node->dest_filename);
		if (cur_node->heredoc_delim)
			free(cur_node->heredoc_delim);
		if (cur_node->type == heredoc)
			safe_close(&cur_node->dest_fd);
		cur_node->dest_filename = NULL;
		cur_node->heredoc_delim = NULL;
		free(cur_node);
		cur_node = next_node;
	}
	*head = NULL;
}

void	free_and_close_exec_data(
	t_exec_data	*exec_data
)
{
	int	i;

	if (exec_data->redirections)
		clean_redir_list(&exec_data->redirections, exec_data->redirections);
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
