/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   undups.c                                                :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/12/10 15:02:53 by avaliull            #+#    #+#           */
/*   Updated: 2025/12/10 16:06:32 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	undup_redirections(
	t_undup_list **undup_head)
{
	t_undup_list	*cur_undup;

	if (!undup_head)
		return ;
	cur_undup = *undup_head;
	while (cur_undup != NULL)
	{
		test_dup2(cur_undup->orig_fd, cur_undup->dup_fd);
			// add error handling ?
		safe_close(&cur_undup->orig_fd);
		cur_undup = cur_undup->prev;
		free(*undup_head);
		*undup_head = cur_undup;
	}
}

int		record_undup(
	t_undup_list **undup_list_head,
	t_undup_list **cur_undup)
{
	*cur_undup = ft_calloc(1, sizeof(t_undup_list));
	if (*cur_undup == NULL)
	{
		undup_redirections(undup_list_head);
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	}
	(*cur_undup)->prev = *undup_list_head;
	if (*undup_list_head == NULL)
		(*undup_list_head) = *cur_undup;
	else
		(*undup_list_head)->next = *cur_undup;
	(*cur_undup)->next = NULL;
	*undup_list_head = *cur_undup;
	return (success);
}
