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

//check what kind of non-terminal for further action
int match_nonterminal(char *str_token)
{
    if (str_token[0] == str_token[1])
    {
        if (str_token[0] == '<')
            return (HEREDOC);
        else if (str_token[0] == '>')
            return (REDIRECT_OUT_APP);
        else if (str_token[0] == '.')
            return (DOUBLE_DOT);
    }
    if (str_token[0] == '<')
        return (REDIRECT_IN);
    else if (str_token[0] == '>')
        return (REDIRECT_OUT);
    else if (ft_strchr(str_token, '$') != NULL)
        return (DOLLAR_SIGN); 
    else if (str_token[0] == '|')
        return (PIPE);
    else if (str_token[0] == '.')
            return (DOT);
    else if (str_token[0] == '?')
        return (QUESTION_MARK);
    else if (str_token[0] == '!')
        return (EXCLAM_MARK);
    return(UNKNOWN);
}


//check what commands are given
int match_string(char *str_token)
{
    if (ft_strncmp(str_token, "cd", 3) == 0)
        return (CD);
    else if (ft_strncmp(str_token, "cat", 4) == 0)
        return (CAT);
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
        exit(0);
    else 
        return (STRING);
}

// set a value to the token so we can expand on those later
int	match_token(char *str_token)
{
	if (str_token[0] == '"')
		return (DOUBLE_Q);
	else if (str_token[0] == '\'')
		return (SINGLE_Q);
	else if (str_token[0] == '(' || str_token[0] == '{')
		return (OPEN_BRACKET);
	else if (str_token[0] == '-')
		return (HYPHEN);
	else if ((str_token[0] == '>') || (str_token[0] == '<')
		|| (str_token[0] == '$') || (str_token[0] == '|')
		|| (str_token[0] == '.') || (str_token[0] == '!') || (str_token[0] == '?'))
		return (match_nonterminal(str_token));
	else if ((ft_strchr(str_token, '=') != NULL))
		return (OPERATOR);
	else if (str_token[0] == '/')
		return (FORW_SLASH);
	else if (str_token[0] == '\\')
		return (BACKW_SLASH);
	else if (ft_isdigit(str_token[0]))
		return (NUMBER);
	else if (ft_isalpha(str_token[0]))
		return (match_string(str_token));
	else
		return(UNKNOWN);
}
