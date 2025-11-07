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
		dprintf(STDERR_FILENO, "recording input redirections\n");
		undup_elem->orig_fd = dup(STDIN_FILENO); 
		undup_elem->dup_fd = STDIN_FILENO; // wrong i think
		undup_elem->next = NULL; // wahto do we do here again
	}
	safe_open(redirection->dest_filename, &redirection->dest_fd, O_RDONLY);
	if (redirection->dest_fd < 0)
		return (msh_perror(NULL, FD_ERR, extern_err), fd_err);
	test_dup2(redirection->dest_fd, STDIN_FILENO);
	safe_close(&redirection->dest_fd);
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
		return (msh_perror(NULL, FD_ERR, extern_err), fd_err);// check return
	if (record == true)
	{
		dprintf(STDERR_FILENO, "recording output redirections\n");
		undup_elem->orig_fd = dup(redirection->src_fd); 
		undup_elem->dup_fd = redirection->src_fd;
		undup_elem->next = NULL;
	}
	if (redirection->src_filename != NULL)
	{
		//this is for special cases only, refer to bash manual
		//redirection->src_fd = open(redirection->src_filename, O_RDONLY);
	}
	test_dup2(redirection->dest_fd, redirection->src_fd);
	safe_close(&redirection->dest_fd);
	return (success);
}

// QUESTION: DO WE NEED TO EXIT ON REDIRECTION FAIL? (I ASSUME NO)
// turns out the answer is YES. lmao
// add a check for if this is child process
int	perform_redirections(
	t_redir_list *redirections,
	t_undup_list **undup_list,
	bool record
)
{
	int				err_check;
	t_undup_list	*cur_undup;
	//t_undup_list	*prev_undup; // add a subfunction for all the undup nonsense

	//prev_undup = NULL; // uuuhhhhhuhuhuhhhhuhhhh linked lsit linked list linked list lionked lsit
	err_check = success;
	if (record == true)
		cur_undup = *undup_list;
	// do we stop after first error or do we try them all?
	while (redirections != NULL)
	{
		if (redirections->type == input || redirections->type == heredoc)
			err_check = input_redirect(redirections, cur_undup, record);
		else
			err_check = output_redirect(redirections, cur_undup, record);
		if (err_check != success)
			return (err_check);
		redirections = redirections->next;
		if (record == true)
		{
			*undup_list = cur_undup;
			cur_undup = cur_undup->next;
		}
	}
	return (err_check);
}
