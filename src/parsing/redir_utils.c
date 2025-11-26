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






