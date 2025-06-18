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
                check_token->value[i] = check_token->value[i +1];
                i++;
            }
        }
    }
}

//check quotes to rm later
int check_quotes(element **tokenlist, int pos, char quote)
{
    t_token *check_token;
    int fpos;

    pos = -1; 
    check_token = (t_token *)(*tokenlist)->elementList.tokens[pos];
    if (check_token->type == DOUBLE_Q_OPEN)
    {
        if (ft_strrchr(check_token->value) != '\0' && ft_strrchr(check_token->value) != check_token->value[0])
            check_token->type = STRING;
        else
        {
            fpos = find_symbol(tokenlist, pos, 34);
            while (fpos > 0)
            {
                if (find_symbol(tokenlist, fpos, '$') > 0)
                    (*tokenlist)->elementList.tokens[fpos].type = DOLLAR_SIGN;
                else 
                    (*tokenlist)->elementList.tokens[fpos].type = STRING;
                fpos--;
            }
        }
    }
    else if (check_token->type == SINGLE_Q_OPEN)
        find_symbol(tokenlist, pos, 39);
}

int check_lexer(element **tokenlist)
{
    while
}
