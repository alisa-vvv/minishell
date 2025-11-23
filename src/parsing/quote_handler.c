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

	i = ft_strlen(str)-1;
	while (i >0 && str[i] != symbol)
	{	
		if (char_is_quote(str[i]))
		{
			l_symbol = str[i];
		}
		i--;
	}
	return (l_symbol);
}



//loops count amount of times to remove outside quotes from string
char rm_str_quotes(char *str, int count, char symbol)
{
	char quote;
	int	i;
	int	len;

	while (count > 0)
	{	
		i = 0;
		len = ft_strlen(str)-1;
		if (char_is_quote(str[0]))
			quote = str[0];
		if (str[i] == symbol)
			count--;
		if (str_is_quote(str, symbol))
		{
			str[len] = '\0';
			while (str[i +1])
			{
				str[i] = str[i + 1];
				i++;
			}
			str[i] = '\0';
		}
	}
	return (quote);
}

//have to make another part to keep the leftover before the quoted part 
char	prep_q_str(char *str, int count, char symbol)
{
	char quote;
	char *leftover;
	char *before;

	char *cut;
	quote = '\0';
	cut = NULL;
	before = NULL;
	leftover = NULL;
	if (str_is_quote(str, symbol))
	{
		quote = rm_str_quotes(str, count, symbol);
		return(quote);
	}
	else 
	{
		leftover = refine_name_var(str, leftover, symbol);
		cut = ft_strrchr(str, symbol);
		cut[0] = '\0';
	}
	if (leftover)
		ft_strjoin(str, leftover);
	if (before)
		ft_strjoin(before, str);
	return (quote);
}

// rm quotes for certain pos in tokenlist
int	rm_quotes(element *tokenlist, int pos, char symbol)
{
	t_token	*check_token;
	int		i;
	int		count;

	i = 0;
	char quote;
	quote = 'q';
	check_token = (t_token *)tokenlist->element_list.tokens[pos];
	count = (count_symbols(check_token->value, symbol));
	// p_printf("CHECK TOKEN = %s\n", check_token->value);
	if (str_is_quote(check_token->value, symbol))
		quote = prep_q_str(check_token->value, count/2, symbol);
	if (quote == '\'' && char_is_quote(symbol))
		check_token->type = SINGLE_Q;
	else if (quote == '"')
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
