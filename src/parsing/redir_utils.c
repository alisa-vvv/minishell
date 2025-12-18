/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   redir_utils.c                                       :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/10/23 12:53:50 by adecheri       #+#    #+#                */
/*   Updated: 2025/10/23 12:53:52 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	char_is_red(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

//checks if string contains redirect
bool	str_contains_red(char *str)
{
	if (ft_strchr(str, '>') || ft_strchr(str, '<'))
		return (1);
	return (0);
}

//function to check validity of redirects syntax
int	validate_redirect( // you were here
	const char *str,
	int i)
{
	if (str[i] == str[i + 1] && (str[i] == '>' || str[i] == '<'))
	{
		if (str[i + 2] == '>' || str[i + 2] == '<' || str[i + 2] == '|')
			return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
	}
	if ((str[i] == '<' && str[i + 1] == '>') || (str[i] == '>' && str[i
			+ 1] == '<'))
	{
		return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
	}
	return (success);
}
