/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   lexer_match.c                                       :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/14 16:07:28 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/14 16:07:29 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// add parsign error cases maybe?
//check what kind of non-terminal for further action
int	match_nonterminal(char *str_token)
{
	if (str_token[0] == str_token[1])
	{
		if (str_token[0] == '<')
			return (HEREDOC);
		else if (str_token[0] == '>')
			return (RED_APP);
	}
	else if (str_token[0] == '<')
		return (RED_IN);
	else if (str_token[0] == '>')
		return (RED_OUT);
	else if (ft_strchr(str_token, '$') != NULL)
		return (PARAMETER);
	else if (ft_strchr(str_token, '|') != NULL)
		return (PIPE);
	return (match_string(str_token));
}

//check what commands are given
int	match_string(char *str_token)
{
	if (ft_strncmp(str_token, "cd", 3) == 0)
		return (CD);
	else if (ft_strncmp(str_token, "pwd", 4) == 0)
		return (PWD);
	else if (ft_strncmp(str_token, "env", 4) == 0)
		return (ENV);
	else if (ft_strncmp(str_token, "echo", 5) == 0)
		return (ECHO);
	else if (ft_strncmp(str_token, "export", 7) == 0)
		return (EXPORT);
	else if (ft_strncmp(str_token, "unset", 6) == 0)
		return (UNSET);
	else if (ft_strncmp(str_token, "exit", 5) == 0)
		return (EXIT);
	else
		return (STRING);
}

// set a va	lue to the token so we can expand on those later
int	match_token(char *str_token) // needs rework @alisa
{
	if (ft_strchr(str_token, '\''))
		return (SINGLE_Q);
	else if (ft_strchr(str_token, '"'))
		return (DOUBLE_Q);
	else if (str_contains_red(str_token) || (str_token[0] == '$')
		|| str_token[0] == '|')
		return (match_nonterminal(str_token));
	return (match_string(str_token));
}
