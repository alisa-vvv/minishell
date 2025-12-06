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
int single_token(t_tokenlist *tokenlist)
{
    t_token *check_token;

    check_token = (t_token *)tokenlist->tokens[0];
    if (!check_token)
        return (1);
    if ((int)tokenlist->total == 1)
    {
        if (token_is_redirect(check_token))
            return (1);
        else 
            check_token->command = true;
    }
    return (0);
}

//checks if token is supposed to be command for list 
int token_is_cm(t_tokenlist *tokenlist, int pos)
{
    t_token *check_token;

    check_token = tokenlist->tokens[pos];

    if (!check_token)
        return (-1);
    else if (pos == 0 && check_token->type == HEREDOC)
        return (1);
    else if ((size_t)pos == (tokenlist->total - 1) && check_token->type == HEREDOC_DEL)
        return (1);
    else if (!token_is_redirect(check_token))
        return (1);
    
    return (0);
}


//(i == 0 && lookahead(tokenlist, i)->type
void set_pipe_cm(
	t_tokenlist *tokenlist, 
    size_t i)
{
    t_token *c_token;
    bool flag;

	flag = true;
	while(i < tokenlist->total && tokenlist->total > 1)
	{
		c_token = (t_token *)tokenlist->tokens[i];
		if (i == 0)
		{
            if (token_is_cm(tokenlist, i))
            {
			    c_token->command = true;
			    flag = false;
            }
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


//check if last token isn't redirect?
bool val_redir_out(
    t_tokenlist *tokenlist, 
    size_t pos)
{
    if (pos + 2 < (size_t)tokenlist->total)
    {
        return (false);
    }
    return (true);
}


//
static int check_heredoc(
    t_tokenlist *tokenlist, 
    size_t pos)
{
    t_token *check_token;

    check_token = (t_token *)tokenlist->tokens[pos];
    check_token->type = HEREDOC_DEL;
    if (pos == tokenlist->total -1)
        return (0);
    else
        return (1);
}

//validate if redir are well placed?
int val_redir(
    t_tokenlist *tokenlist, 
    size_t i)
{
    t_token *check_token;
    while (i < tokenlist->total)
    {
        check_token = (t_token *)tokenlist_get(tokenlist, i);
        if (!check_token)
            return(1);
        if (check_token->type == HEREDOC && i + 1 < tokenlist->total)
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
