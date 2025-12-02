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


bool char_is_red(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

//|| ft_strchr(str, '|')
//checks if string contains redirect 
bool str_contains_red(char *str)
{
	if (ft_strchr(str, '>') || ft_strchr(str, '<'))
		return (1);
	return (0);
}


//checks if the redirs are only 1 or 2 correct tokens
bool check_val_redir(char *str)
{
	int i;
	char type;

	i = 0;
	type = '0';
	while (str[i])
	{
		if (char_is_red(str[i]))
		{
			type = str[i];
			if (str[i +1] == type && str[i+2] != type)
				return (true);
			else if (str[i +1] != type && !ft_isspace(str[i+1]) && !char_is_red(str[i+1]))
				return (true);
		}
		i++;
	}
	return(false);
}


//check if redirect
bool	token_is_redirect(
	t_token *check_token)
{
	if (!check_token)
		return (NULL);
	if (check_token->type == HEREDOC || check_token->type == REDIRECT_IN
		|| check_token->type == REDIRECT_OUT
		|| check_token->type == REDIRECT_OUT_APP)
		return (true);
	return (false);
}






