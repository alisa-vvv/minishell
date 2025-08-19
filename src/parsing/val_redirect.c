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
void set_pipe_cm(
	element *tokenlist)
{
	size_t i;
    t_token *c_token;
    bool flag;

	i = 0;
	flag = true;
	while(i < (size_t)tokenlist->element_list.total)
	{
		c_token = (t_token *)tokenlist->element_list.tokens[i];
		if (i == 0 && c_token->type >= CAT && c_token->type <= UNSET || i == 0 && c_token->type == HEREDOC)
		{
			c_token->command = true;
			flag = false;
		}
		else if (c_token->type == REDIRECT_IN || c_token->type == REDIRECT_OUT || c_token->type == REDIRECT_OUT_APP || c_token->type == PIPE || c_token->type == HEREDOC)
			flag = true;
		else if (flag == true)
		{
			c_token->command = true;
			flag = false;
		}
		else
			c_token->command = false; 
		i++;
	}
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
        if (check_token->type == HEREDOC && i + 1 <= (size_t)tokenlist->element_list.total)
        {
            check_token = (t_token *)tokenlist->element_list.tokens[i+1];
            check_token->type = HEREDOC_DEL;
            i++;
        }
        else if (check_token->type == REDIRECT_OUT_APP || check_token->type == REDIRECT_OUT)
        {
            if (i + 1 <= (size_t)tokenlist->element_list.total)
            {
                check_token = (t_token *)tokenlist->element_list.tokens[i+1];
                check_token->type = check_filename(check_token->value);
                i++;
            }
        }
        else 
            i++;
    }
//    t_printf("in val redirect\n");
    return (0);
}
