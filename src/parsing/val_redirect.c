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
            return (1);
        else 
            check_token->command = true;
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
	while(i < (size_t)tokenlist->element_list.total && (size_t)tokenlist->element_list.total > 1)
	{
		c_token = (t_token *)tokenlist->element_list.tokens[i];
		if (i == 0 && lookahead(tokenlist, i)->type != HEREDOC && c_token->type >= CAT && c_token->type <= UNSET || (i == 0 && c_token->type == HEREDOC) || (i == tokenlist->element_list.total - 1 && c_token->type == HEREDOC_DEL))
		{
			c_token->command = true;
			flag = false;
		}
		else if (c_token->type == PIPE || c_token->type == HEREDOC_DEL)
			flag = true;
		else if (flag == true && lookahead(tokenlist, i)->type != HEREDOC)
		{
			c_token->command = true;
			flag = false;
		}
		else
			c_token->command = false; 
		i++;
	}
}


int val_redir_out(
    element *tokenlist, 
    int pos)
{
    t_token * check_token;
    int status;
    status = -1;
    check_token = (t_token *)tokenlist->element_list.tokens[pos];
    check_token->type = check_file(check_token->value);
    if (pos + 2 < (size_t)tokenlist->element_list.total)
    {
        check_token = (t_token *)tokenlist->pf_element_get(tokenlist, pos +2);
        check_token->type = check_file(check_token->value);
        status = 0;
    }
    return (status);
}


//set values 
int val_redir(element *tokenlist)
{
    size_t i;
    t_token *check_token;
    i = 0;

    while (i < (size_t)tokenlist->element_list.total)
    {
        check_token = (t_token *)tokenlist->pf_element_get(tokenlist, i); 
        if (check_token->type == HEREDOC && i + 1 < tokenlist->pf_element_total(tokenlist))
        {
            check_token = (t_token *)tokenlist->element_list.tokens[i+1];
            check_token->type = HEREDOC_DEL;
            i++;
        }
        else if (lookahead(tokenlist, i) != NULL && lookahead(tokenlist, i)->type == REDIRECT_OUT_APP || lookahead(tokenlist, i)->type == REDIRECT_OUT || lookahead(tokenlist, i)->type == REDIRECT_IN)
        {
            if (val_redir_out(tokenlist, i))
                return(1);
            i++;
        }
        else if (!lookahead(tokenlist, i))
            return (1);
        else
            i++;
    }
   
//    t_printf("in val redirect\n");
    return (0);
}
