/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   lexer_utils2.c                                      :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/12/07 19:49:01 by adecheri       #+#    #+#                */
/*   Updated: 2025/12/07 19:49:04 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// skip whitespaces in string
int	skip_blanks(char *str, int pos)
{
	int	count;

	count = 0;
	while (!check_in_quote(str, pos) && ft_isspace(*str))
	{
		count++;
		str++;
	}
	return (count);
}

// l_printf("trimmed str = %s$\n", str);
// trims input to have 1 space
char	*trim_str_space(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(str) - 1;
	while (ft_isspace(str[j] && char_is_quote(str[j])))
	{
		str[j] = '\0';
		j--;
	}
	j = 0;
	while (str[j])
	{
		if ((i == 0 && skip_blanks(str, j) >= 1))
			j += skip_blanks(str, j);
		else if (skip_blanks(str, j) > 1)
			j += skip_blanks(str, j + 1);
		str[i] = str[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}
