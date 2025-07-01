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
    int count_s;
    int count_d;
    int in_quote;

    count_s = 0;
    count_d = 0;
    in_quote = -1;
    while (*str)
    {
        if (count_d > 0 && count_d % 2 != 0)
            in_quote = 2;
        else if (count_s > 0 && count_s % 2 != 0)
            in_quote = 1;
        else
            in_quote = -1;
        if ((*str == '"' && in_quote == 2) || (*str == '"' && in_quote == -1))
            count_d++;
        else if ((*str == '\'' && in_quote == 1) || (*str == '\'' && in_quote == -1))
            count_s++;
        str++;
    }
    if (symbol == '"')
        return (count_d);
    return (count_s);
}

char *trim_str_quote(char *str, int symbol, int count)
{
    int in_quote;

    in_quote = 0;
    while (*str)
    {
        if (*str == symbol)
        {
            in_quote++;
            count--;
        }
        if (count && (in_quote % 2 == 0))
        {
            if (str
        }
        str++;
    }
    return (str);
}

// //check if quotes are valid
// int check_dquotes(element **tokenlist, int pos)
// {
//     t_token *check_token;
//     int fpos;
//     size_t total;

//     if (count_symbol(tokenlist, pos, '"') % 2 != 0)
//         return (write(1, "Command not found\n", 18));

//     fpos = -1;
//     total = (*tokenlist)->pf_element_total;
//     check_token = (t_token *)(*tokenlist)->pf_element_get(tokenlist, pos);
//     if (ft_strchr(check_token->value) != '\0' && ft_strrchr(check_token->value) != check_token->value[0])
//         check_token->type = DOUBLE_Q_CLOSED;
//     else if 
//     {
//         fpos = find_symbol(tokenlist, pos, '"');
//         if (fpos != 0)
//         {
//             while (fpos > pos)
//             {
//                 if ((*tokenlist)->element_list.tokens[fpos] != DOLLAR_SIGN)
//                     (int)(*tokenlist)->element_list.tokens[fpos].type = DOUBLE_Q_CLOSED;
//                 fpos--;
//             }
//         }
//     }
//     return (0);
// }


// //check if single quotes are closed and set types in arr to closed.
// int check_squotes(element **tokenlist, int pos)
// {
//     t_token *check_token;
//     int fpos;
//     size_t total;

//     total = (*tokenlist)->pf_element_total;
//     while (pos < total)
//     {
//         fpos = find_symbol(tokenlist, pos + 1, '\'');
//         check_token = (t_token *)(*tokenlist)->pf_element_get(tokenlist, pos);
//         if (check_token->value[0] && check_token->value[ft_strlen(check_token->value)-1])
//             check_token->type = SINGLE_Q_CLOSED;
//         else if (!fpos == -1)
//         {
//             check_token = (t_token *)(*tokenlist)->pf_element_get(tokenlist, fpos);
//             if (check_token->value[ft_strlen(check_token->value) - 1] == '\'')
//                 check_token->type = SINGLE_Q_CLOSED;
//             while (fpos > pos)
//             {
//                 (*tokenlist)->element_list.tokens[fpos] = SINGLE_Q_CLOSED;
//                 fpos--;
//             }
//         }
//         pos++;
//     }
//     return (-1);
// }

//rm quotes for certain pos in tokenlist
void rm_quotes(element *tokenlist, int pos, char symbol)
{
    t_token *check_token;
    int i; 
    i = 0;
    check_token = (t_token *)tokenlist->element_list.tokens[pos];
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