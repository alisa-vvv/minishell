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

//func to traverse list and find a spec symbol
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
    return (-1);
}

// count occurrence of spec symbol in array
int count_symbol(element *tokenlist, int pos, char symbol)
{
    int count;
    int i;
    t_token* check_token;
    
    check_token = tokenlist->pfElementGet(tokenlist, pos);
    count = 0;
    i = 0;
    while ((size_t)pos < tokenlist->elementList.total)
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

//check if quotes are valid
int check_dquotes(element **tokenlist, int pos)
{
    t_token *check_token;
    int fpos;
    size_t total;

    if (count_symbol(tokenlist, pos, 34) % 2 != 0)
        return (write(1, "Unclosed quotes", 15));

    fpos = -1;
    total = (*tokenlist)->pfElementTotal;
    check_token = (t_token *)(*tokenlist)->pfElementGet(tokenlist, pos);
    if (ft_strchr(check_token->value) != '\0' && ft_strrchr(check_token->value) != check_token->value[0])
        check_token->type = DOUBLE_Q_CLOSED;
    else if 
    {
        fpos = find_symbol(tokenlist, pos, 34);
        if (fpos != 0)
        {
            while (fpos > pos)
            {
                if ((*tokenlist)->elementList.tokens[fpos] != DOLLAR_SIGN)
                    (int)(*tokenlist)->elementList.tokens[fpos].type = DOUBLE_Q_CLOSED;
                fpos--;
            }
        }
    }
    return (0);
}

//check if single quotes are closed and set types in arr to closed.
int check_squotes(element **tokenlist, int pos)
{
    t_token *check_token;
    int fpos;
    size_t total;

    total = (*tokenlist)->pfElementTotal;
    while (pos < total)
    {
        fpos = find_symbol(tokenlist, pos + 1, 39);
        check_token = (t_token *)(*tokenlist)->pfElementGet(tokenlist, pos);
        if (check_token->value[0] && check_token->value[ft_strlen(check_token->value)-1])
            check_token->type = SINGLE_Q_CLOSED;
        else if (!fpos == -1)
        {
            check_token = (t_token *)(*tokenlist)->pfElementGet(tokenlist, fpos);
            if (check_token->value[ft_strlen(check_token->value) - 1] == 39)
                check_token->type = SINGLE_Q_CLOSED;
            while (fpos > pos)
            {
                (*tokenlist)->elementList.tokens[fpos] = SINGLE_Q_CLOSED;
                fpos--;
            }
        }
        pos++;
    }
    return (-1);
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
