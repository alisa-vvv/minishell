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
			return HEREDOC;
		else if (str_token[0] == '>')
			return (REDIRECT_OUT_APP);
	}
	else if (str_token[0] == '<')
		return (REDIRECT_IN);
	else if (str_token[0] == '>')
		return (REDIRECT_OUT);
	else if (ft_strchr(str_token, '$') != NULL)
		return (PARAMETER); 
	else if (ft_strchr(str_token, '|') != NULL)
		return (PIPE);
	else if (str_token[0] == '?')
		return (QUESTION_MARK);
	// add parsign error cases maybe?
	return (STRING);
}

//check what commands are given
int match_string(char *str_token)
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
        return(EXIT);
    else
        return (STRING);
}

int all_num_alph( // moved this from hyphen_handler @alisa
    const char *str)
{
	size_t  i;

	i = 0;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i] == '\0')
		return (NUMBER);
	else 
		return(STRING);
}

// set a va	lue to the token so we can expand on those later
// redirect issues:
// ls -l>outfile - does not add "-l" to argv
// ls -l >outfile1>outfile2 - creates a file named outfile1>outfile2 and redirects there.
// should instead have to redirections, one to outfile1, second to outfile2
// compare to: ls -l >"outfile1>outfile2"
int	match_token(char *str_token) // needs rework @alisa
{
	if (str_token[0] == '\'')
		return (SINGLE_Q);
	else if (str_token[0] == '"')
		return (DOUBLE_Q);
	else if (str_contains_red(str_token) || (str_token[0] == '$')
			|| str_token[0] == '|' || (str_token[0] == '?'))
		return (match_nonterminal(str_token));
	else if ((ft_strchr(str_token, '=') != NULL))
		return (OPERATOR);
	else if (ft_isdigit(str_token[0]))
		return (all_num_alph(str_token));
	return (STRING);
	// could there be parsing errors here? check
}
