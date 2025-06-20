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

//rm quotes for certain pos in tokenlist
void rm_quotes(element *tokenlist, int pos, char symbol)
{
    t_token *check_token;
    int i; 
    i = 0;
    check_token = (t_token *)tokenlist->elementList.tokens[pos];
    if (check_token->value[ft_strlen(check_token->value)] == symbol)
       (*tokenlist)->elementList.tokens[pos].value[ft_strlen(check_token->value)] = '\0';
    while (check_token->value[i])
    {
        if (check_token->value[i] != symbol)
            i++;
        else 
        {
            while (check_token->value[i])
            {
                check_token->value[i] = check_token->value[i + 1];
                i++;
            }
        }
    }
}

//func to traverse list and find closing quotes
int find_symbol(element *tokenlist, int pos, char symbol)
{
    t_token *check_token;

    while ((size_t)pos < tokenlist->elementList.total)
    {
        check_token = (t_token *)tokenlist->elementList.tokens[pos];
        if (ft_strchr(check_token->value, symbol) != NULL)
            return(pos);
        pos++;
    }
    return (0);
}

// //check quotes to rm later
// int check_quotes(element **tokenlist, int pos, char quote)
// {
//     t_token *check_token;
//     int fpos;
//     int type;

//     fpos = -1;
//     check_token = (t_token *)(*tokenlist)->elementList.tokens[pos];
//     if (quote == DOUBLE_Q_OPEN)
//         type = 34;
//     else 
//         type = 39;
//     if (check_token->type == quote)
//     {
//         if (ft_strrchr(check_token->value) != '\0' && ft_strrchr(check_token->value) != check_token->value[0])
//             check_token->type = STRING;
//         else
//         {
//             fpos = find_symbol(tokenlist, pos, 34);
//             while (fpos > 0)
//             {
//                 if ((*tokenlist)->elementList.tokens[fpos].type != DOLLAR_SIGN)
//                     (*tokenlist)->elementList.tokens[fpos].type = STRING;
//                 fpos--;
//             }
//         }
//     }
//     return (0);
// }

int check_dquotes(element **tokenlist, int pos)
{
    t_token *check_token;
    int fpos;
    size_t total;

    fpos = -1;
    total = (*tokenlist)->pfElementTotal;
    check_token = (t_token *)(*tokenlist)->pfElementGet(tokenlist, pos);
    if (ft_strchr(check_token->value) != '\0' && ft_strrchr(check_token->value) != check_token->value[0])
        check_token->type = DOUBLE_Q_CLOSED;
    else if (check_token->type == DOUBLE_Q_OPEN && (t_token *)(*tokenlist)->elementList.tokens[total]->type == DOUBLE_Q_OPEN)
    {

    }
    else 
    {
        fpos = find_symbol(tokenlist, pos, 34);
        if (fpos != 0)
        {
            while (fpos > pos)
            {
                if ((*tokenlist)->elementList.tokens[fpos] != DOLLAR_SIGN)
                    (*tokenlist)->elementList.tokens[fpos].type = DOUBLE_Q_CLOSED;
                fpos--;
            }
        }
    }
    return (0);
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
    check_token = (t_token *)(*tokenlist)->elementList.tokens[i];
    while (i < (size_t)(*tokenlist)->elementList.total)
    {
        if (check_token->type == DOUBLE_Q_OPEN)
        {
            check_quotes(tokenlist, i, DOUBLE_Q_OPEN);
            rm_quotes(tokenlist, i, DOUBLE_Q_OPEN);
        }
        i++;
    }
}
