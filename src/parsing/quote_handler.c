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

//checks if quotes contains something other that quotes, returns true if so 
bool str_is_quote(const char *str, char symbol)
{
	int len;
	int i;

	i = 0; 
	len = ft_strlen(str) - 1;
	if (str[0] == symbol && str[len] == symbol)
		return (true);
	return (false);
}

//	q_printf("count_d = %d & count_s = %d\n", count_d, count_s);
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
char symbol_in_quote(char *str, char symbol)
{
	int	i;
	char l_symbol;

	i = 0;
	while (str[i])
	{	
		if (char_is_quote(str[i]))
			l_symbol = str[i];
		if (str[i] == symbol)
			break;
		i++;
	}
	return (l_symbol);
}

//removes 1 pos by shifting string 
char *move_str(char *str, int i)
{ 

	while (str[i])
	{
		str[i] = str[i + 1];
		i++;
	}
	str[i] = '\0';
	return(str);
}


bool	check_quote_rm(const char *str, int i, char symbol)
{
	if (str[i] && str[i] == symbol)
	{
		if (check_in_quote_s(str, i, '\''))
			return (false);
		else if (!check_in_quote_s(str, i, '\'') && check_in_quote_s(str, i, '"'))
			return (true);
		else if (!check_in_quote(str, i))
			return (true);
	}
	return (false);
}


//	p_printf("STRING = %s\n", str);
//loops count amount of times to remove outside quotes from string
void rm_str_quotes(char *str, int count, char symbol)
{
	int o_count;
	char o_quote;
	int	i;

	i = 0;
	if (symbol == '"')
		o_quote = '\'';
	else 
		o_quote = '"';
	o_count = count_occ(str, o_quote, true);
	while (str[i] && (count || o_count))
	{	
		if (check_quote_rm(str, i , symbol))
		{
			count--;
			str = move_str(str, i);
			move_str(ft_strrchr(str, symbol), 0);
		}
		else if (check_quote_rm(str, i , o_quote))
		{
			o_count--;
			str = move_str(str, i);
			move_str(ft_strrchr(str, o_quote), 0);
		}
		else 
			i++;
	}
}

//	p_printf("CHECK TOKEN = %s\n", check_token->value);
// if (str_is_quote(check_token->value, symbol))
// rm quotes for certain pos in tokenlist
int	rm_quotes(t_tokenlist *tokenlist, int pos, char symbol)
{
	t_token	*check_token;
	int		i;
	int		count;

	i = 0;
	check_token = (t_token *)tokenlist->tokens[pos];
	count = (count_occ(check_token->value, symbol, true));
	if (char_is_quote(symbol))
		rm_str_quotes(check_token->value, count, symbol);
	if (!check_token->value || ft_strncmp(check_token->value, "", 1) == 0)
		tokenlist_delete(tokenlist, pos);

	return (0);
}
