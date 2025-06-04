/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   lexer.c                                             :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/03 18:53:46 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/03 18:53:56 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */


#include "parser.h"

int is_EOS(char *str, int i)
{
    if (i == ft_strlen(str))
        return (1);
    return (0);
}

void push_token(t_token *token, t_token **tlist)
{
    t_token *last;
    if (!tlist)
    {
        *tlist = token;
        return ;
    }
    while ((*tlist)->next)
        last = (*tlist)->next;
    last = token; 
}

t_token *default_handler(char *str)
{
    int i;
    i = 0;

}

int match
// t_token *regular_expression(char *str)
// {
//     int i;
//     i = 0; 
//     while(isspace(str[i]))
//         i++;
//     if 
// }