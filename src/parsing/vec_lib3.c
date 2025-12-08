/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   vec_lib3.c                                          :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/08/23 13:20:41 by adecheri       #+#    #+#                */
/*   Updated: 2025/08/23 13:20:42 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	tokenlist_set(t_tokenlist *e, size_t index, void *token)
{
	if (e)
	{
		if ((index >= 0) && (index < e->total))
		{
			ft_free_s_token(&e->tokens[index]);
			e->tokens[index] = token;
			return (0);
		}
	}
	return (-1);
}

void	*tokenlist_get(t_tokenlist *e, size_t index)
{
	void	*token_data;

	token_data = NULL;
	if (e)
	{
		if ((index >= 0) && (index < e->total))
			token_data = e->tokens[index];
	}
	return (token_data);
}

int	tokenlist_total(t_tokenlist *e)
{
	if (e)
		return (e->total);
	return (-1);
}
