/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   redirections.c                                    :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/12 17:27:04 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/12 18:48:28 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "minishell.h"
#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

static int	perform_input_redirection(
	t_redir_list *redirection
)
{
	if (redirection->dest_filename != NULL)
	{
		redirection->dest_fd = open(redirection->dest_filename, O_RDONLY);
		if (redirection->dest_fd != 0)
			return (msh_perror(NULL, FD_ERR, extern_err), fd_err); // do we return here?
	}
	test_dup2(redirection->dest_fd, STDIN_FILENO);
	test_close(redirection->dest_fd);
	return (0);
}

static int	perform_output_redirection(
	t_redir_list *redirection
)
{
	const int	trunc_f = O_WRONLY | O_CREAT | O_TRUNC;
	const int	app_f = O_WRONLY | O_CREAT | O_APPEND;

	if (redirection->dest_filename != NULL)
	{
		if (redirection->type == trunc)
			redirection->dest_fd = open(redirection->dest_filename, trunc_f, 0664);
		else if (redirection->type == append)
			redirection->dest_fd = open(redirection->dest_filename, app_f, 0664);
	}
	if (redirection->dest_fd < 0)
		return (msh_perror(NULL, FD_ERR, extern_err), fd_err);
	if (redirection->src_filename != NULL)
	{
		//this is for special cases only, refer to bash manual
		//redirection->src_fd = open(redirection->src_filename, O_RDONLY);
	}
	if (redirection->src_fd < 0)
	{
		test_close(redirection->dest_fd);
		return (msh_perror(NULL, FD_ERR, extern_err), fd_err);
	}
	test_dup2(redirection->dest_fd, redirection->src_fd);
	test_close(redirection->dest_fd);
	return (0);
}

// QUESTION: DO WE NEED TO EXIT ON REDIRECTION FAIL? (I ASSUME NO)
// turns out the answer is YES. lmao
// add a check for if this is child process
int	perform_redirections(
	t_redir_list *redirections
)
{
	int	err_check;

	err_check = 0;
	while (redirections != NULL)
	{
		if (redirections->type == input || redirections->type == heredoc)
			err_check = perform_input_redirection(redirections);
		else
			err_check = perform_output_redirection(redirections);
		// do we stop after first error or do we try them all?
		if (err_check != success)
			return (err_check);
		redirections = redirections->next;
	}
	return (err_check);
}
