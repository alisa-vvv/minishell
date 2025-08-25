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
		if (cur_node->src_filename)
			free(cur_node->src_filename);
		if (cur_node->heredoc_delim)
			free(cur_node->heredoc_delim);
		if (cur_node->type == heredoc)
			test_close(cur_node->dest_fd);
		cur_node->dest_filename = NULL;
		cur_node->src_filename = NULL;
		cur_node->heredoc_delim = NULL;
		cur_node->dest_fd = CLOSED_FD;
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

	printf("is exec data null before thing? %p\n", exec_data->redirections);
	if (exec_data->redirections)
		clean_redir_list(exec_data->redirections, *(exec_data->redirections));
	printf("is exec data null after thing? %p\n", exec_data->redirections);
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
