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

// Your shell must implement the following built-in commands:
//     - echo with option -n
//     - cd with only a relative or absolute path
//     - pwd with no options
//     - export with no options
//     - unset with no options
//     - env with no options or arguments
//     - exit with no options



//func to traverse list and find closing brackets or quotes
int find_closing_symbol(element *tokenlist, int pos, char symbol)
{
    t_token *check_token;
    int count;
    int bracket;

    count = 0;
    if (symbol == '{')
    {
        bracket = 1;
        symbol = '}';
    }
    while (pos < tokenlist->elementList.total)
    {
        check_token = (t_token *)tokenlist->elementList.tokens[pos];
        if (ft_strchr(check_token->value, symbol) != NULL)
            count++;
        pos++;
    }
    if (count % 2 == 0 && bracket == 0)
        return (1);
    else if (count % 2 != 0 && bracket == 1)
        return (1);
    return (0);
}

int lookahead(element *tokenlist, size_t index)
{
    t_token *check_token;
    check_token = (t_token *)tokenlist->pfElementGet(&tokenlist, index);
    if (check_token->type)

}
