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
	j = -1;
	if (!str || ft_strncmp(str, " ", 2))
		return (str);
	while (str[++j])
	{
		if ((i == 0 && skip_blanks(str, j) >= 1))
			j += skip_blanks(str, j);
		else if (skip_blanks(str, j) > 1)
			j += skip_blanks(str, j + 1);
		str[i] = str[j];
		i++;
	}
	str[i] = '\0';
	return (str);
}


//checks valid syntax for order of pipes /redirects
int	check_pipes(t_tokenlist *tlist)
{
	size_t	i;
	t_token	*check_token;

	i = 0;
	check_token = NULL;
	while (i < tlist->total)
	{
		check_token = tlist->tokens[i];
		if (check_token->type == PIPE && looknxt(tlist, i) && looknxt(tlist,
				i)->type == PIPE)
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
