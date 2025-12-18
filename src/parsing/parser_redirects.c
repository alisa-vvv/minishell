/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser_redirects.c                                  :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/08/28 12:39:43 by adecheri       #+#    #+#                */
/*   Updated: 2025/08/28 12:39:44 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// func to traverse list and find a spec symbol
int	find_symbol(t_tokenlist *tokenlist, int pos, char symbol)
{
	t_token	*check_token;

	while ((size_t)pos < tokenlist->total)
	{
		check_token = (t_token *)tokenlist->tokens[pos];
		if (ft_strchr(check_token->value, symbol) != NULL)
			return (pos);
		pos++;
	}
	return (-1);
}

//sets default values for the redirs
void	set_redir_def(t_redir_list *redirlist)
{
	redirlist->heredoc_delim = NULL;
	redirlist->type = 0;
	redirlist->src_fd = -1;
	redirlist->dest_fd = -1;
	redirlist->dest_filename = NULL;
	redirlist->next = NULL;
}

//set specific type for the redirect list
int	set_type(t_redir_list *redirlist,
				t_tokenlist *tokenlist,
				int pos)
{
	t_token	*check_token;
	t_token	*prev_token;

	check_token = tokenlist_get(tokenlist, pos);
	if (check_token->type == RED_IN)
		redirlist->type = input;
	else if (check_token->type == RED_OUT)
		redirlist->type = trunc;
	else if (check_token->type == RED_APP)
		redirlist->type = append;
	if (pos > 0)
		prev_token = lookbehind(tokenlist, pos);
	if (check_token->type == RED_IN)
		redirlist->src_fd = STDIN_FILENO;
	else if (check_token->type == RED_OUT || check_token->type == RED_APP)
		redirlist->src_fd = STDOUT_FILENO;
	return (0);
}
