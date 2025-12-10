/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   quote_handler.c                                     :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/29 10:52:49 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/29 10:52:51 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

//removes 1 pos by shifting string
char	*move_str(char *str, int i)
{
	while (str[i])
	{
		str[i] = str[i + 1];
		i++;
	}
	str[i] = '\0';
	return (str);
}

//checks if quote needs to be removed or if within another quote
bool	check_quote_rm(const char *str, int i, char symbol)
{
	if (str[i] && str[i] == symbol)
	{
		if (check_in_quote_s(str, i, '\''))
			return (false);
		else if (!check_in_quote_s(str, i, '\'') && check_in_quote_s(str, i,
				'"'))
			return (true);
		else if (!check_in_quote(str, i))
			return (true);
	}
	return (false);
}

//loops around the str removing quotes when encountered
void	reform_str(char *str, t_quote *q)
{
	int	i;

	i = 0;
	while (str[i] && (q->count || q->o_count))
	{
		if (check_quote_rm(str, i, q->quote))
		{
			q->count--;
			str = move_str(str, i);
			move_str(ft_strrchr(str, q->quote), 0);
		}
		else if (check_quote_rm(str, i, q->o_quote))
		{
			q->o_count--;
			str = move_str(str, i);
			move_str(ft_strrchr(str, q->o_quote), 0);
		}
		else
			i++;
	}
}

//loops checks what kind or quotes need to be removed and sends to reform str
void	rm_str_quotes(char *str, t_quote *q)
{
	if (q->quote == '"')
		q->o_quote = '\'';
	else
		q->o_quote = '"';
	q->o_count = count_occ(str, q->o_quote, true);
	reform_str(str, q);
}

// if (str_is_quote(check_token->value, symbol))
// rm quotes for certain pos in tokenlist
int	rm_quotes(t_tokenlist *tokenlist, int pos, char symbol)
{
	t_token	*check_token;
	t_quote	*q;
	int		err;

	err = success;
	q = ft_calloc(1, sizeof(t_quote));
	if (!q)
		return (msh_perror(NULL, MALLOC_ERR, malloc_err), malloc_err);
	q->quote = symbol;
	check_token = (t_token *)tokenlist->tokens[pos];
	q->count = (count_occ(check_token->value, symbol, true));
	if (char_is_quote(symbol))
		rm_str_quotes(check_token->value, q);
	if (!check_token->value || ft_strncmp(check_token->value, "", 1) == 0)
		err = tokenlist_delete(tokenlist, pos);
	free(q);
	return (err);
}
