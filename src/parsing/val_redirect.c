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

//check single tokens
int single_token(element *tokenlist)
{
    t_token *check_token;

    check_token = (t_token *)tokenlist->element_list.tokens[0];
    if ((int)tokenlist->element_list.total == 1)
    {
        if (check_token->type == HEREDOC || check_token->type == REDIRECT_OUT || check_token->type == REDIRECT_OUT_APP || check_token->type == REDIRECT_IN)
            return (write(1, "Command not found\n", 18));
    }
    return (0);
}


//set values 
int val_redir(element *tokenlist)
{
    size_t i;
    t_token *check_token;
    i = 0;

    while (i < (size_t)tokenlist->element_list.total)
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
            if (i + 1 < (size_t)tokenlist->element_list.total && lookahead(tokenlist, i)->type != FILENAME)
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
