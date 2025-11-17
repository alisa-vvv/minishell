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

// check if a certain pos is inside quotes
int	check_in_quote(const char *str, int pos)
{
	int	count_s;
	int	count_d;
	int	in_quote;
	int	i;

	count_s = 0;
	count_d = 0;
	in_quote = -1;
	i = -1;
	while (str[++i] && i <= pos)
	{
		if (str[i] == '"' && (in_quote == 2 || in_quote == -1))
			count_d++;
		else if (str[i] == '\'' && (in_quote == 1 || in_quote == -1))
			count_s++;
		if (count_d > 0 && count_d % 2 != 0)
			in_quote = 2;
		else if (count_s > 0 && count_s % 2 != 0)
			in_quote = 1;
		else
			in_quote = -1;
	}
	count_d %= 2;
	count_s %= 2;
	q_printf("count_d = %d & count_s = %d\n", count_d, count_s);
	return (count_d || count_s);
}

void	move_str(char *str, char symbol, int count)
{
	int	i;
	int	len;

	while (count > 0)
	{	
		i = 0;
		len = ft_strlen(str);
		if (str[0] && str[len -1] == symbol)
		{
			str[len - 1] = '\0';
			while (str[i + 1])
			{
				str[i] = str[i + 1];
				i++;
			}
			str[i] = '\0';
		}
		count--;
	}
}

// rm quotes for certain pos in tokenlist
int	rm_quotes(element *tokenlist, int pos, char symbol)
{
	t_token	*check_token;
	int		i;
	int		count;

	i = 0;
	check_token = (t_token *)tokenlist->element_list.tokens[pos];
	count = count_symbols(check_token->value, symbol);
	if (check_token->value[i] == symbol
		&& check_token->value[ft_strlen(check_token->value) - 1] == symbol)
		move_str(check_token->value, symbol, count);
	if (symbol == '\'')
		check_token->type = STRING;
	else if (symbol == '"' && check_token->value[0] == '\''
		&& check_token->value[ft_strlen(check_token->value) - 1] == '\'')
		check_token->type = SINGLE_Q;
	else if (symbol == '"')
		check_token->type = match_nonterminal(check_token->value);
	return (0);
}
