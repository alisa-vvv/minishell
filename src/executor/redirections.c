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

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

static void	safe_open(
	char *filename,
	int *fd,
	int flags)
{
	const int	trunc_f;
	const int	app_f;

	trunc_f = O_WRONLY | O_CREAT | O_TRUNC;
	app_f = O_WRONLY | O_CREAT | O_APPEND;
	if (filename == NULL)
		return ;
	// add access check here?
	if (flags == trunc_f || flags == app_f)
		*fd = open(filename, flags, 0664);
	else
		*fd = open(filename, flags);
}

static int	input_redirect(
	t_redir_list *redirection,
	t_undup_list *undup_elem,
	bool record)
{
	if (record == true)
	{
		undup_elem->orig_fd = dup(STDIN_FILENO);
		undup_elem->dup_fd = STDIN_FILENO;
		undup_elem->dest_fd = redirection->dest_fd;
		undup_elem->next = NULL;
	}
	safe_open(redirection->dest_filename, &redirection->dest_fd, O_RDONLY);
	if (redirection->dest_fd < 0)
	{
		msh_perror(NULL, redirection->dest_filename, extern_err);
		return (fd_err);
	}
	test_dup2(redirection->dest_fd, STDIN_FILENO);
	safe_close(&redirection->dest_fd);
	return (success);
}

static int	output_redirect(
	t_redir_list *redirection,
	t_undup_list *undup_elem,
	bool record)
{
	const int	trunc_f;
	const int	app_f;

	trunc_f = O_WRONLY | O_CREAT | O_TRUNC;
	app_f = O_WRONLY | O_CREAT | O_APPEND;
	if (redirection->type == trunc)
		safe_open(redirection->dest_filename, &redirection->dest_fd, trunc_f);
	else if (redirection->type == append)
		safe_open(redirection->dest_filename, &redirection->dest_fd, app_f);
	if (redirection->dest_fd < 0)
	{
		msh_perror(NULL, redirection->dest_filename, extern_err);
		return (fd_err); // check return
	}
	if (record == true)
	{
		undup_elem->orig_fd = dup(STDOUT_FILENO);
		undup_elem->dup_fd = STDOUT_FILENO;
		undup_elem->dest_fd = redirection->dest_fd;
		undup_elem->next = NULL;
	}
	test_dup2(redirection->dest_fd, STDOUT_FILENO);
	safe_close(&redirection->dest_fd);
	return (success);
}

int			do_redirections(
	t_redir_list *redirections,
	t_undup_list **undup_list_head,
	bool record)
{
	int				err_check;
	t_undup_list	*cur_undup;

	cur_undup = NULL;
	err_check = success;
	while (redirections != NULL)
	{
		if (record == true)
		{
			if (record_undup(undup_list_head, &cur_undup) != success)
				return (malloc_err);
		}
		if (redirections->type == input || redirections->type == heredoc)
			err_check = input_redirect(redirections, cur_undup, record);
		else
			err_check = output_redirect(redirections, cur_undup, record);
		if (err_check != success)
		{
			undup_redirections(undup_list_head);
			return (err_check);
		}
		redirections = redirections->next;
	}
	return (err_check);
}
