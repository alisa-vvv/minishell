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

static void	 safe_open(
	char *filename,
	int *fd,
	int flags
)
{
	const int	trunc_f = O_WRONLY | O_CREAT | O_TRUNC; // make these into defines maybe? can we?
	const int	app_f = O_WRONLY | O_CREAT | O_APPEND;

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
	bool record
)
{
	if (record == true)
	{
		undup_elem->orig_fd = dup(STDIN_FILENO); // investigate this dup
		undup_elem->dup_fd = STDIN_FILENO; // wrong i think
		undup_elem->dest_fd = redirection->dest_fd;
		undup_elem->next = NULL; // wahto do we do here again
	}
	safe_open(redirection->dest_filename, &redirection->dest_fd, O_RDONLY);
	if (redirection->dest_fd < 0)
		return (msh_perror(NULL, redirection->dest_filename, extern_err), fd_err);// check return
	test_dup2(redirection->dest_fd, STDIN_FILENO);
	safe_close(&redirection->dest_fd); //ingestigae this close
	return (success);
}

static int	output_redirect(
	t_redir_list *redirection,
	t_undup_list *undup_elem,
	bool record
)
{
	const int	trunc_f = O_WRONLY | O_CREAT | O_TRUNC;
	const int	app_f = O_WRONLY | O_CREAT | O_APPEND;

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
		undup_elem->orig_fd = dup(redirection->src_fd); 
		undup_elem->dup_fd = redirection->src_fd;
		undup_elem->dest_fd = redirection->dest_fd;
		undup_elem->next = NULL;
	}
	test_dup2(redirection->dest_fd, redirection->src_fd);
	safe_close(&redirection->dest_fd);
	return (success);
}

void	undup_redirections(
	t_undup_list **undup_head
)
{
	t_undup_list	*cur_undup;

	if (!undup_head)
		return ;
	cur_undup = *undup_head;
	while (cur_undup != NULL)
	{
		dprintf(STDERR_FILENO, "undupping\n");
		test_dup2(cur_undup->orig_fd, cur_undup->dup_fd); // add error handling ?
		safe_close(&cur_undup->orig_fd);
		cur_undup = cur_undup->prev;
		free(*undup_head);
		*undup_head = cur_undup;
	}
}

void	record_undup(
	t_undup_list **undup_list_head,
	t_undup_list *cur_undup
)
{
	cur_undup->prev = *undup_list_head;
	if (*undup_list_head == NULL)
		(*undup_list_head) = cur_undup;
	else
		(*undup_list_head)->next = cur_undup;
	cur_undup->next = NULL;
	*undup_list_head = cur_undup;
}

int	perform_redirections(
	t_redir_list *redirections,
	t_undup_list **undup_list_head,
	bool record
)
{
	int				err_check;
	t_undup_list	*cur_undup;

	cur_undup = NULL;
	err_check = success;
	while (redirections != NULL)
	{
		if (record == true)
			cur_undup = ft_calloc(1, sizeof(t_undup_list)); // ugh
		if (redirections->type == input || redirections->type == heredoc)
			err_check = input_redirect(redirections, cur_undup, record);
		else
			err_check = output_redirect(redirections, cur_undup, record);
		if (err_check != success)
		{
			undup_redirections(undup_list_head); // FIX this!
			return (err_check);
		}
		redirections = redirections->next;
		if (record == true)
			record_undup(undup_list_head, cur_undup);
	}
	return (err_check);
}
