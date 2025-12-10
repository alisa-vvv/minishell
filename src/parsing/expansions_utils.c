/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   expansions_utils.c                                  :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/11/04 15:52:14 by adecheri       #+#    #+#                */
/*   Updated: 2025/11/04 15:52:16 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// expand the var when token is in quotes
char	*exp_token(char *str_token, char *start, char *value, int offset)
{
	char	*new_str;
	char	*leftover;
	char	*temp_left;

	temp_left = prep_leftover(str_token, start, offset);
	if (!temp_left)
		return (NULL);
	leftover = ft_strjoin(value, temp_left);
	new_str = ft_strjoin(str_token, leftover);
	if (!new_str || !leftover)
	{
		msh_perror(NULL, MALLOC_ERR, extern_err);
		ft_safe_free((unsigned char **)&leftover);
		ft_safe_free((unsigned char **)&new_str);
		ft_safe_free((unsigned char **)&temp_left);
		return (NULL);
	}
	ft_safe_free((unsigned char **)&leftover);
	ft_safe_free((unsigned char **)&temp_left);
	return (new_str);
}

// e_printf("\nRESULT = %s$\n", result);
// get name of env var from token_name
int	refine_name(char *token_name, char **result, char symbol)
{
	int	i;

	i = 0;
	if (!token_name)
	{
		*result = NULL;
		return (success);
	}
	*result = ft_strdup(token_name + 1);
	if (!*result)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	while ((*result)[i])
	{
		if (symbol == '$')
		{
			if (char_is_quote((*result)[i]) || (*result)[i] == '$'
				|| ft_isspace((*result)[i]) || char_is_red((*result)[i])
				|| !(ft_isprint((*result)[i]) || (*result)[i] == '_'))
				break ;
		}
		i++;
	}
	(*result)[i] = '\0';
	return (success);
}

// counts how many occurrences of a symbol are in one str
int	count_occ(const char *str, char symbol, bool inside)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (inside)
		{
			if (!check_in_quote_s(str, i, '\''))
				count++;
		}
		else if (inside == false)
		{
			if (str[i] == symbol)
				count++;
		}
		i++;
	}
	return (count);
}

//&& !check_in_quote_s(str, pos, '\'')
bool	check_quote_e(char *str, int pos)
{
	int	quote;

	quote = symbol_in_quote(str, '$');
	if (quote == '"' && !check_in_quote_s(str, pos, '\''))
		return (true);
	else if (quote == '\'' && check_in_quote_s(str, pos, '"'))
		return (true);
	else if (quote == '\'' && !check_in_quote_s(str, pos, '\''))
		return (true);
	else
		return (false);
}
