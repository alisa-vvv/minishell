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

// trims input to have 1 space
char	*trim_str_space(char *str)
{
	int	i;
	int	j;

	if (!str)
		return (NULL);
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

//returns len of unquoted token
int	move_o_unquoted(const char *str, int i)
{
	int	len;

	len = 0;
	while (str[i] && !char_is_quote(str[i]) && !ft_isspace(str[i])
		&& !char_is_red(str[i]))
	{
		len++;
		i++;
	}
	if (str[i] && char_is_quote(str[i]) && str[i - 1] == '=')
	{
		i++;
		while (str[i] && !char_is_quote(str[i]))
		{
			len++;
			i++;
		}
		i++;
		len += 2;
	}
	return (len);
}

//checks valid syntax for order of pipes /redirects 
int check_pipes(t_tokenlist *tlist)
{
	size_t i;
	i = 0;

	while (i < tlist->total)
	{
		t_token *check_token;
		check_token = tlist->tokens[i];
		if (check_token->type == PIPE && looknxt(tlist, i) && looknxt(tlist, i)->type == PIPE)
			return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
		else if (tok_is_red(check_token))
		{
			if (looknxt(tlist, i) && looknxt(tlist, i)->type == PIPE)
				return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
		}
		i++;
	}
	return (success);
}