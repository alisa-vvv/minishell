/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   lexer3.c                                            :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/11/24 20:56:59 by adecheri       #+#    #+#                */
/*   Updated: 2025/11/24 20:57:00 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */


#include "parser.h"


//merge tokens with pos1 and pos2 into pos 1
int merge_tokens(t_tokenlist *tokenlist, 
    int pos1, 
    int pos2)
{
	t_token *check_token;
	t_token *extra_token;

	check_token = tokenlist->tokens[pos1];
	extra_token = tokenlist->tokens[pos2];
    check_token->value = ft_strjoin(check_token->value, extra_token->value);
    
    check_token->pos = pos1;
    tokenlist_delete(tokenlist, pos2);
    //index_lexer(&tokenlist);
	return (0);
}

// checks if str only contains quotes 
int check_all_quote(const char *str)
{
    int i;

    i = 0;
    while (str[i] && char_is_quote(str[i]))
        i++;
    if (str[i] == '\0')
        return (true);
    return (false);
}

//contract tokens when surrounded by quote tokens 
void contract_quotes(t_tokenlist *tokenlist,
    t_token *check_token,
    int pos)
{
    int flag;

    flag = -1;
    if (check_all_quote(check_token->value))
    { 
        flag = check_token->type;
        check_token = lookbehind(tokenlist, pos);
        while ((size_t)pos > 0)
        {
            if (check_token->type == (enum e_token_type)flag)
                break;
            else 
            {
                merge_tokens(tokenlist, pos-1, pos);
                if (pos > 0)
                    check_token = lookbehind(tokenlist, pos);
                pos--;
            }
        }
    }
}


//contracts tokens if surrounded by token quotes 
int contract_list(t_tokenlist *tokenlist,
    int i)
{
    t_token *check_token;

    check_token = NULL;
    while ((size_t)i > 0)
    {
        check_token = tokenlist->tokens[i];
        if (!check_token)
            return (1);
        if ((check_token->type == DOUBLE_Q || check_token->type == SINGLE_Q))
        {
            contract_quotes(tokenlist, check_token, i);
        }
        else if (i > 0 && lookbehind(tokenlist, i) && lookbehind(tokenlist, i)->type == OPERATOR)
        {
            merge_tokens(tokenlist, i -1, i);
        }
        i--;
    }
    index_lexer(&tokenlist);
    return (0);
}
