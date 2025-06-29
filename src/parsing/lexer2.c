/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   lexer2.c                                            :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/18 18:22:33 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/18 18:22:35 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */


#include "parser.h"


//func to traverse list and find a spec symbol
int find_symbol(element *tokenlist, int pos, char symbol)
{
    t_token *check_token;

    while ((size_t)pos < tokenlist->element_list.total)
    {
        check_token = (t_token *)tokenlist->element_list.tokens[pos];
        if (ft_strchr(check_token->value, symbol) != NULL)
            return(pos);
        pos++;
    }
    return (-1);
}

// count occurrence of spec symbol in array
int count_symbol(element *tokenlist, int pos, char symbol)
{
    int count;
    int i;
    t_token* check_token;
    
    check_token = tokenlist->pf_element_get(tokenlist, pos);
    count = 0;
    i = 0;
    while ((size_t)pos < tokenlist->element_list.total)
    {
        if (find_symbol(tokenlist, pos, symbol) > -1)
        {
            while(check_token->value[i])
            {
                if (check_token->value[i] == symbol)
                    count++;
                i++;
            }
        }
        pos++;
    }
    return (count); 
}


// //index lexer by traversing - not needed anymore
// int index_lexer(element **tokenlist)
// {
//     int i;
//     i = 0;

//     while (i < (size_t)(*tokenlist)->elementList.total)
//     {
//         (*tokenlist)->elementList.tokens[i].pos = i;
//         i++;
//     }
//     return (0);
// }

int check_lexer(element **tokenlist)
{
    size_t i;
    t_token *check_token;
    
    i = 0;
    check_token = (t_token *)(*tokenlist)->element_list.tokens[i];
    while (i < (size_t)(*tokenlist)->element_list.total)
    {
        if (check_token->type == DOUBLE_Q_OPEN)
        {
            check_quotes(tokenlist, i, DOUBLE_Q_OPEN);
        }
        i++;
    }
}
