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
	const t_exec_data	*exec_data
)
{
	free_and_close_redir_list(exec_data->redirections);
	if (exec_data->argv)
		free_2d_arr((void **) exec_data->argv);
}
