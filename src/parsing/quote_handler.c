/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   quote_handler.c                                     :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/29 10:52:49 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/29 10:52:51 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"


int check_quote_c(char *str, int symbol)
{
    int count;
    count = 0;
    while (*str)
    {
        if (*str == symbol)
            count++;
        str++;
    }
    return (count);
}

int check_d_quote_in(char *str)
{
    if (check_quote_c(str, 34) == 1)
        return (write(1, "Command not found\n", 18));
    else if (check_quote_c(str, 34) % 2 != 0)
        return (write(1, "Command not found\n", 18));
    return (0);
}

int check_s_quote_in(char *str)
{
    int i;
    i = 0;
    if (check_quote_c(str, 39) == 1)
        return (write(1, "Command not found\n", 18));
    
    while (str[i])
    {
        if (str[i] == 39)
        {
            if (str[i] != ft_isalpha(str[i]))
            {
                while()
            }
        }

        i++; 
    }
}



//check if quotes are valid
int check_dquotes(element **tokenlist, int pos)
{
    t_token *check_token;
    int fpos;
    size_t total;

    if (count_symbol(tokenlist, pos, 34) % 2 != 0)
        return (write(1, "Command not found\n", 18));

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