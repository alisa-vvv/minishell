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
int	check_in_quote(char *str, int pos)
{
	int	count_s;
	int	count_d;
	int	in_quote;
	int i;

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


//rm quotes for certain pos in tokenlist
void	rm_quotes(element *tokenlist, int pos, char symbol)
{
	t_token	*check_token;
	int		i;

	i = 0;
	check_token = (t_token *)tokenlist->element_list.tokens[pos];
	if (check_token->value[ft_strlen(check_token->value)-1] == symbol)
		check_token->value[ft_strlen(check_token->value)-1] = '\0';
	while (check_token->value[i])
	{
		if (check_token->value[i] == symbol)
		{
			while (check_token->value[i+1])
			{
				check_token->value[i] = check_token->value[i + 1];
				i++;
			}
		}
	}
	check_token->type = STRING;
}
