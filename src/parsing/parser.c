/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/03 16:55:05 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/03 16:55:07 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// make an empty execdata
int	make_cm_list(
	t_tokenlist *tokenlist,
	t_exec_data *comm_list,
	t_pos *ind)
{
	int	total;
	int	i;

	i = -1;
	total = 0;
	comm_list->argv = NULL;
	if (ind->red > 0)
		total = count_args(tokenlist, ind->pos, ind->red);
	else
		total = count_args(tokenlist, ind->pos, tokenlist->total);
	if (total == 0)
		return (0);
	comm_list->argv = ft_calloc(sizeof(char *), (total + 1));
	if (!comm_list->argv)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	while (++i < total)
		comm_list->argv[i] = NULL;
	comm_list->argv[total] = NULL;
	return (success);
}
