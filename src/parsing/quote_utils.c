/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   quote_utils.c                                       :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/12/07 17:23:10 by adecheri       #+#    #+#                */
/*   Updated: 2025/12/07 17:23:12 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

//returns true if character is quote
bool	char_is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (true);
	return (false);
}

//checks if quotes contains something other that quotes, returns true if so
bool	str_is_quote(const char *str, char symbol)
{
	int	len;

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
	return (count_d || count_s);
}

// check if a certain pos is inside quotes
int	check_in_quote_s(const char *str, int pos, char quote)
{
	int	count;
	int	in_quote;
	int	i;

	count = 0;
	in_quote = -1;
	i = -1;
	while (str[++i] && i <= pos)
	{
		if (str[i] == quote && (in_quote == 1 || in_quote == -1))
			count++;
		if (count > 0 && count % 2 != 0)
			in_quote = 1;
		else
			in_quote = -1;
	}
	count %= 2;
	return (count);
}

//returns the kind of quote that is within quotes for var
char	symbol_in_quote(char *str, char symbol)
{
	int		i;
	char	l_symbol;

	i = 0;
	l_symbol = '0';
	while (str[i])
	{
		if (char_is_quote(str[i]))
			l_symbol = str[i];
		if (str[i] == symbol)
			break ;
		i++;
	}
	return (l_symbol);
}
