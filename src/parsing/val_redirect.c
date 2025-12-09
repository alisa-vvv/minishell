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
	if (looknxt(tokenlist, i) != NULL && looknxt(tokenlist, i)->type == HEREDOC)
		return (false);
	return (true);
}

//(i == 0 && looknxt(tokenlist, i)->type
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
		return (success);
	return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
}

//validate if redir are well placed
int	val_redir(
	t_tokenlist *tlist,
	size_t i,
	t_token *ctok)
{
	while (i < tlist->total)
	{
		ctok = (t_token *)tokenlist_get(tlist, i);
		if (!ctok)
			return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
		if (ctok->type == HEREDOC && i + 1 < tlist->total)
		{
			if (check_heredoc(tlist, i + 1))
				i++;
			else
				return (success);
		}
		else if ((tok_is_red(ctok) || ctok->type == PIPE) && !looknxt(tlist, i))
			return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
		else if (looknxt(tlist, i) != NULL && tok_is_red(looknxt(tlist, i)))
		{
			if (val_redir_out(tlist, i))
				return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
			i++;
		}
		else
			i++;
	}
	return (success);
}
