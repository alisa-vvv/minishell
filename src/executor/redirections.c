/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   redirections.c                                    :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/12 17:27:04 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/12 17:28:01 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <stdio.h>
#include <fcntl.h>

static int	perform_input_redirection(
	t_redir_list *redirection
)
{
	int			fd;

	fd = open(redirection->dest, O_RDONLY);
	if (fd < 0)
	{
		printf("PLACEHOLDER ERROR\n");
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	perform_output_redirection(
	t_redir_list *redirection
)
{
	const int	truncate_flags = O_WRONLY | O_CREAT | O_TRUNC;
	const int	append_flags = O_WRONLY | O_CREAT | O_APPEND;
	int			fd;

	if (redirection->type == trunc)
		fd = open(redirection->dest, truncate_flags, 0664);
	else if (redirection->type == append)
		fd = open(redirection->dest, append_flags, 0664);
	if (fd < 0)
	{
		printf("PLACEHOLDER ERROR\n");
		return (1);
	}
	dup2(fd, redirection->src);
	close(fd);
	return (0);
}

int	perform_redirections(
	t_redir_list *redirections,
	const t_command_io *command_io
)
{
	int	err_check; // make sure to return here to see what kind of errros need to be tracked
	while (redirections != NULL)
	{
		if (redirections->type == input)
			perform_input_redirection(redirections);
		else if (redirections->type == heredoc)
		{
		}
		else
			perform_output_redirection(redirections);
		redirections = redirections->next;
	}
	return (0);
}
