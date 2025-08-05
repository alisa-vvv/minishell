/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   val_redirect.c                                      :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/08/05 16:39:15 by adecheri       #+#    #+#                */
/*   Updated: 2025/08/05 16:39:17 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */


#include "parser.h"



// int	val_redirects(element **tokenlist)
// {
// 	size_t	i;
// 	t_token	*check_token;

// 	i = 0;
// 	check_token = (t_token *)(*tokenlist)->element_list.tokens[i];
// 	while (i < (size_t)(*tokenlist)->element_list.total)
// 	{
// 		if (check_token->type == REDIRECT_IN)
// 		{
// 			if (lookahead(tokenlist, i) != STRING && lookahead(tokenlist, i) != PARAMETER)
// 				return (1);
// 			//check if leading arg is something or nothing 
// 		}
// 		else if (check_token->type == REDIRECT_OUT || REDIRECT_OUT_APP)
// 		{
// 			if ()
// 		}
// 		i++;
// 	}
// 	return (0);
// }


int val_redir(element *tokenlist)
{
    size_t i;
    t_token *check_token;
    i = 0;

    while (i + 1 < (size_t)tokenlist->element_list.tokens[i])
    {
        check_token = (t_token *)tokenlist->element_list.tokens[i];
        if (check_token->type == HEREDOC)
        {
            check_token = (t_token *)tokenlist->element_list.tokens[i+1];
            check_token->type = HEREDOC_DEL;
            i++;
        }
        else if (check_token->type == REDIRECT_OUT_APP || check_token->type == REDIRECT_OUT)
        {
            if (lookahead(tokenlist, i)->type != FILENAME)
            {
                check_token = (t_token *)tokenlist->element_list.tokens[i+1];
                check_token->type = FILENAME;
                i++;
            }
        }

        i++;
    }
    return (0);
}