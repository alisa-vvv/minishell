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
	if (!str)
		return (str);
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
		else if (!check_in_quote(str, i))
			return (true);
		else if (check_in_quote_s(str, i, '\'') && check_in_quote_s(str, i, '"') && symbol == '"')
			return (false);
		else if (!check_in_quote_s(str, i, '\'') && check_in_quote_s(str, i,
				'"'))
			return (true);
		
	}
	return (false);
}



//have to make a quote removal for empty quotes before this func
//removes quotes in a string from outside in 
void	rm_str_quotes(char *str, t_quote *q)
{
	int	i;
	int start_q;

	i = 0;
	start_q = 0; 
	while (str[i])
	{
		if (check_quote_rm(str, i, q->quote))
		{
			while (str[i] && str[i] != str[start_q])
				i++;
			str = move_str(str, i);
			str = move_str(str, start_q -1);
			//move_str(ft_strrchr(str, q->quote), 0);
			printf("STRING2= (%s)\n", str);
		}
		else
			i++;
	}
}


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
