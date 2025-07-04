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

	count_s = 0;
	count_d = 0;
	in_quote = -1;
	while (*str != str[pos])
	{
		if (*str == '"' && (in_quote == 2 || in_quote == -1))
			count_d++;
		else if (*str == '\'' && (in_quote == 1 || in_quote == -1))
			count_s++;
		if (count_d > 0 && count_d % 2 != 0)
			in_quote = 2;
		else if (count_s > 0 && count_s % 2 != 0)
			in_quote = 1;
		else
			in_quote = -1;
		str++;
	}
	return ((count_d + count_s) % 2);
}

// //check if single quotes are closed and set types in arr to closed.
// int check_squotes(element **tokenlist, int pos)
// {
//     t_token *check_token;
//     int fpos;
//     size_t total;

//     total = (*tokenlist)->pf_element_total;
//     while (pos < total)
//     {
//         fpos = find_symbol(tokenlist, pos + 1, '\'');
//         check_token = (t_token *)(*tokenlist)->pf_element_get(tokenlist,
//		pos);
//         if (check_token->value[0]
//	&& check_token->value[ft_strlen(check_token->value)-1])
//             check_token->type = SINGLE_Q_CLOSED;
//         else if (!fpos == -1)
//         {
//             check_token = (t_token *)(*tokenlist)->pf_element_get(tokenlist,
//		fpos);
//             if (check_token->value[ft_strlen(check_token->value)
//	- 1] == '\'')
//                 check_token->type = SINGLE_Q_CLOSED;
//             while (fpos > pos)
//             {
//                 (*tokenlist)->element_list.tokens[fpos] = SINGLE_Q_CLOSED;
//                 fpos--;
//             }
//         }
//         pos++;
//     }
//     return (-1);
// }

// rm quotes for certain pos in tokenlist
void	rm_quotes(element *tokenlist, int pos, char symbol)
{
	t_token	*check_token;
	int		i;

	i = 0;
	check_token = (t_token *)tokenlist->element_list.tokens[pos];
	if (check_token->value[ft_strlen(check_token->value)] == symbol)
		(*tokenlist)->elementList.tokens[pos].value[ft_strlen(check_token->value)] = '\0';
	while (check_token->value[i])
	{
		if (check_token->value[i] != symbol)
			i++;
		else
		{
			while (check_token->value[i])
			{
				check_token->value[i] = check_token->value[i + 1];
				i++;
			}
		}
	}
}
