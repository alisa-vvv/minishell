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

//returns true if character is quote
bool char_is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (true);
	return (false);
}

bool str_is_quote(const char *str, char symbol)
{
	int len;

	len = ft_strlen(str) - 1;
	if (str[0] == symbol && str[len] == symbol)
		return (true);
	return (false);
}

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

void	move_str(char *str, int count, char symbol)
{
	int	i;
	int	len;

	i = 0;
	// p_printf("COUNT = %d\n", count);
	while (count > 0)
	{	
		i = 0;
		len = ft_strlen(str)-1;
		if (str[0] == symbol)
			count--;
		p_printf("COUNT = %d\n", count);
		if (char_is_quote(str[0]) && char_is_quote(str[len]))
		{
			str[len] = '\0';
			while (str[i + 1])
			{
				str[i] = str[i + 1];
				i++;
			}
			str[i] = '\0';
		}
	//	p_printf("TOKEN = %s\n", str);
	//	count--;
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
	count = (count_symbols(check_token->value, symbol) / 2);
	// p_printf("CHECK TOKEN = %s\n", check_token->value);
	if (str_is_quote(check_token->value, symbol))
		move_str(check_token->value, count, symbol);
	if (symbol == '\'')
		check_token->type = SINGLE_Q;
	else if (str_is_quote(check_token->value, '\''))
		check_token->type = SINGLE_Q;
	else if(str_is_quote(check_token->value, '"'))
	{
		if (ft_strchr(check_token->value, '$'))
			check_token->type = PARAMETER;
		else 
			check_token->type = DOUBLE_Q;
	}
	else 
		check_token->type = match_nonterminal(check_token->value);
	// p_printf("CHECK TOKEN AFTER = %s\n", check_token->value);
	return (0);
}
