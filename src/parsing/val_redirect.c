/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   val_redirect.c                                      :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/08/05 16:39:15 by adecheri       #+#    #+#                */
/*   Updated: 2025/08/05 16:39:17 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	set_cm_heredoc(t_tokenlist *tokenlist, int i)
{
	if (lookahead(tokenlist, i) != NULL && lookahead(tokenlist,
			i)->type == HEREDOC)
		return (false);
	return (true);
}

//(i == 0 && lookahead(tokenlist, i)->type
void	set_pipe_cm(
	t_tokenlist *tlist,
	size_t i)
{
	t_token	*c_token;
	bool	flag;

	flag = true;
	while (i < tlist->total && tlist->total > 1)
	{
		c_token = (t_token *)tlist->tokens[i];
		if (i == 0)
		{
			if (token_is_cm(tlist, i, c_token))
				flag = false;
		}
		else if (c_token->type == PIPE || c_token->type == HEREDOC_DEL)
			flag = true;
		else if (flag == true)
		{
			c_token->command = set_cm_heredoc(tlist, i);
			flag = false;
		}
		else
			c_token->command = false;
		i++;
	}
}

//check if last token isn't redirect?
bool	val_redir_out(
	t_tokenlist *tokenlist,
	size_t pos)
{
	if (pos + 2 < (size_t)tokenlist->total)
		return (false);
	return (true);
}

//check if statement contains a heredoc
static int	check_heredoc(
	t_tokenlist *tokenlist,
	size_t pos)
{
	t_token	*check_token;

	check_token = (t_token *)tokenlist->tokens[pos];
	check_token->type = HEREDOC_DEL;
	if (pos == tokenlist->total - 1)
		return (0);
	return (1);
}

//validate if redir are well placed
int	val_redir(
	t_tokenlist *tlist,
	size_t i)
{
	t_token	*c_token;

	while (i < tlist->total)
	{
		c_token = (t_token *)tokenlist_get(tlist, i);
		if (!c_token)
			return (1);
		if (c_token->type == HEREDOC && i + 1 < tlist->total)
		{
			if (check_heredoc(tlist, i + 1))
				i++;
			else
				return (0);
		}
		else if ((token_is_redirect(c_token) || c_token->type == PIPE) && !lookahead(tlist, i))
			return (1);
		else if (lookahead(tlist, i) != NULL
			&& token_is_redirect(lookahead(tlist, i)))
		{
			if (val_redir_out(tlist, i))
				return (1);
			i++;
		}
		else
			i++;
	}
	return (0);
}
