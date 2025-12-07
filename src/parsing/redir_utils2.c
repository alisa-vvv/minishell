/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   redir_utils2.c                                      :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/12/07 22:10:22 by adecheri       #+#    #+#                */
/*   Updated: 2025/12/07 22:10:25 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

//check if redirect
bool	token_is_redirect(
	t_token *check_token)
{
	if (!check_token)
		return (NULL);
	if (check_token->type == HEREDOC || check_token->type == RED_IN
		|| check_token->type == RED_OUT || check_token->type == RED_APP)
		return (true);
	return (false);
}

//check single tokens
int	single_token(t_tokenlist *tokenlist)
{
	t_token	*check_token;

	check_token = (t_token *)tokenlist->tokens[0];
	if (!check_token)
		return (1);
	if ((int)tokenlist->total == 1)
	{
		if (token_is_redirect(check_token))
			return (1);
		else
			check_token->command = true;
	}
	return (0);
}

//checks if token is supposed to be command for list
int	token_is_cm(t_tokenlist *tokenlist, int pos, t_token *c_token)
{
	t_token	*check_token;

	check_token = tokenlist->tokens[pos];
	if (!check_token)
		return (-1);
	else if (pos == 0 && check_token->type == HEREDOC)
		return (c_token->command = true, 1);
	else if ((size_t)pos == (tokenlist->total - 1)
		&& check_token->type == HEREDOC_DEL)
		return (c_token->command = true, 1);
	else if (!token_is_redirect(check_token))
		return (c_token->command = true, 1);
	return (0);
}
