/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   cleanup.c                                         :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/18 16:33:46 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 20:15:50 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdlib.h"
#include "executor.h"

void	clean_exit(
	t_exec_data *exec_data,
	t_minishell_data *minishell_data,
	char *read_line,
	int exit_code
)
{
	while (exec_data)
	{
		free_and_close_exec_data(exec_data);
		exec_data++;
	}
	if (read_line)
		free(read_line);
	if (minishell_data)
	{
		if (minishell_data->env)
			free_2d_arr((void **) minishell_data->env);
	}
	exit (exit_code);
}

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

void	free_and_close_exec_data(
	t_exec_data	*exec_data
)
{
	int	i;
	printf("are we here?\n");
	if (exec_data)
	{
		if (exec_data->redirections)
			free_and_close_redir_list(exec_data->redirections);
		i = -1;
		if (exec_data->argv)
		{
			//free_2d_arr((void **) exec_data->argv);
			while (exec_data->argv[++i])
				free(exec_data->argv[i]);
			free(exec_data->argv);
		}
		//free(exec_data);
	}
}
