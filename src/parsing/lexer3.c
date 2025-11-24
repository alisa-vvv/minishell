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
int merge_tokens(element *tokenlist, 
    int pos1, 
    int pos2)
{
	t_token *check_token;
	t_token *extra_token;

	check_token = tokenlist->element_list.tokens[pos1];
	extra_token = tokenlist->element_list.tokens[pos2];
    check_token->value = ft_strjoin(check_token->value, extra_token->value);
    tokenlist->pf_element_delete(tokenlist, pos2);
    //index_lexer(&tokenlist);
	return (0);
}

int contract_list(element *tokenlist,
    int i)
{
    t_token *check_token;
    int flag;
    int j;

    check_token = NULL;
    flag = -1;
    j = 0;  
    while ((size_t)i < tokenlist->element_list.total)
    {
        check_token = tokenlist->element_list.tokens[i];
        if (check_token->type == DOUBLE_Q || check_token->type == SINGLE_Q)
        {
            j = i; 
            flag = check_token->type;
            i++;
            while (lookahead(tokenlist, i) && lookahead(tokenlist, i)->type != (enum e_token_type)flag)
                i++;
            while (i >= j)
            {
                merge_tokens(tokenlist, i-1, i);
                i--;
            }
        }
        i++;
    }
    return (0);
}