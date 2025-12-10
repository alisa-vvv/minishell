/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser_utils2.c                                     :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/12/10 12:25:37 by adecheri       #+#    #+#                */
/*   Updated: 2025/12/10 12:25:38 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// count how many exec data structs need to be made
int	count_lists(t_tokenlist *tokenlist)
{
	size_t	i;
	size_t	count;
	t_token	*check_token;

	i = 0;
	count = 1;
	while (i < tokenlist->total)
	{
		check_token = (t_token *)tokenlist->tokens[i];
		if (check_token->type == PIPE)
			count++;
		if (i == tokenlist->total - 1 && check_token->type == PIPE)
			return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
		i++;
	}
	return (count);
}

// count until the next pos that is a command
int	count_next_cm(t_tokenlist *tokenlist, int pos)
{
	size_t	i;
	t_token	*check_token;

	i = pos + 1;
	while (i < tokenlist->total)
	{
		check_token = (t_token *)tokenlist->tokens[i];
		if (check_token->type == PIPE && looknxt(tokenlist, pos))
			return (check_token->pos + 1);
		if (check_token->command)
			return (check_token->pos);
		i++;
	}
	return (-1);
}
