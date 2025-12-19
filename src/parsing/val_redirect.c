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
	if (pos - 1 < tokenlist->total - 1)
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
			if (check_heredoc(tlist, i + 1) != success)
				return (syntax_err);
			else
				i++;
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
