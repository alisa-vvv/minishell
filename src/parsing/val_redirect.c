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
    if (!check_token)
        return (1);
    if ((int)tokenlist->element_list.total == 1)
    {
        if (token_is_redirect(check_token))
            return (1);
        else 
            check_token->command = true;
    }
    return (0);
}

void set_pipe_cm(
	element *tokenlist, 
    size_t i)
{
    t_token *c_token;
    bool flag;

	flag = true;
	while(i < (size_t)tokenlist->element_list.total && (size_t)tokenlist->element_list.total > 1)
	{
		c_token = (t_token *)tokenlist->element_list.tokens[i];
		if ((i == 0 && lookahead(tokenlist, i)->type != HEREDOC
			&& (c_token->type >= CD && c_token->type <= UNSET))
			|| (i == 0 && c_token->type == HEREDOC)
			|| (i == tokenlist->element_list.total - 1 && c_token->type == HEREDOC_DEL))
		{
			c_token->command = true;
			flag = false;
		}
		else if (c_token->type == PIPE || c_token->type == HEREDOC_DEL)
			flag = true;
		else if (flag == true)
		{
            if (lookahead(tokenlist, i) != NULL && lookahead(tokenlist, i)->type == HEREDOC)
			    c_token->command = false;
            else
                c_token->command = true; 
			flag = false;
		}
		else
			c_token->command = false; 
		i++;
	}
}


static int val_redir_out( // double check, not sure what status does @alisa
    element *tokenlist, 
    size_t pos)
{
    //t_token *check_token;
    int status;

    status = -1;
    //check_token = (t_token *)tokenlist->element_list.tokens[pos];
    if (pos + 2 < (size_t)tokenlist->element_list.total)
    {
      //  check_token = (t_token *)tokenlist->pf_element_get(tokenlist, pos +2);
        status = 0;
    }
    return (status);
}


static int check_heredoc(
    element *tokenlist, 
    size_t pos)
{
    t_token *check_token;

    check_token = (t_token *)tokenlist->element_list.tokens[pos];
    check_token->type = HEREDOC_DEL;
    if (pos == tokenlist->element_list.total -1)
        return (0);
    else
        return (1);
}

//set values 
int val_redir(
    element *tokenlist, 
    size_t i)
{
    t_token *check_token;
    while (i < tokenlist->element_list.total)
    {
        check_token = (t_token *)tokenlist->pf_element_get(tokenlist, i);
        if (!check_token)
            return(1);
        if (check_token->type == HEREDOC && i + 1 < tokenlist->element_list.total)
        {
            if (check_heredoc(tokenlist, i + 1))
                i++;
            else 
                return (0);
        }
        else if ((token_is_redirect(check_token) || check_token->type == PIPE) && !lookahead(tokenlist, i))
            return (1);
        else if (lookahead(tokenlist, i) != NULL && token_is_redirect(lookahead(tokenlist, i)))
        {
            if (val_redir_out(tokenlist, i))
                return(1);
            i++;
        }
        else
            i++;
    }
    return (0);
}
