/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/03 16:55:05 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/03 16:55:07 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */


#include "parser.h"

// top-down parser ll1 parser?, without backtracking,
// looking for elements to expand from 
// looking for: 
//      - double quotes
//      - single quotes 
//      - redirections
//      - $ sign

// construc parsing table 
// first() and follow() funcs
// lookahead func for check next token
// match function to check and expand on aliases 

// Different expansions to think of: 
//  - brace expansion
//  - tilde expansion
//  - parameter and variable expansion
//  - command substitution
//  - arithmetic expansion
//  - word splitting
//  - filename expansion 


//func to traverse list and find closing brackets or quotes
int find_closing_symbol(element *tokenlist, char symbol)
{
    t_token *check_token;
    size_t i;

    i = 1;
    while(i < tokenlist->elementList.total)
    {
        check_token = (t_token *)tokenlist->elementList.tokens[i];
        if (ft_strchr(check_token->value, symbol) != NULL)
            return (1);
        i++;
    }
    return (0);
}

int lookahead(element *tokenlist, size_t index)
{
    t_token *check_token;
    check_token = (t_token *)tokenlist->pfElementGet(&tokenlist, index);
    if (check_token->type)

}
