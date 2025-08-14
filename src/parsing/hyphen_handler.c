/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   hyphen_handler.c                                    :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/08/11 13:51:40 by adecheri       #+#    #+#                */
/*   Updated: 2025/08/11 13:51:42 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

//check to see if string is all alph or digits
int all_num_alph(
    const char *str)
{
    size_t  i;

    i = 0;
    if (!str)
        return (UNKNOWN);
    if (ft_isalpha(str[i]))
    {
        while(str[i] && ft_isalpha(str[i]))
            i++;
        if (!str[i])
            return (STRING);
    } 
    else if (ft_isdigit(str[i]))
    { 
        while (str[i] && ft_isdigit(str[i]))
        i++;
        if (!str[i])
            return (NUMBER);
    }
    return(UNKNOWN);
}

void check_hyphens(
    element *tokenlist
)
{
    size_t	i;
    int alnum; 

	t_token	*check_token;

	i = 0;
	while (i < (size_t)tokenlist->element_list.total)
	{
        check_token = (t_token *)tokenlist->element_list.tokens[i];
        if (check_token->type == HYPHEN)
        {
            if (all_num_alph(check_token->value + 1) == STRING)
                check_token->type = HYPHEN_ARG;
            else 
                check_token->type = all_num_alph(check_token->value +1);
        }
        i++;
    }
}
