/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parse_tree.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/03 16:55:05 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/03 16:55:07 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// top-down parser ll1 parser?, without backtracking,
// looking for elements to expand from 
// looking for: 
//      - double quotes
//      - single quotes 
//      - redirections
//      - $ sign


// typedef struct sVectorList
// {
//     void **elements;
//     size_t size; (size in memory)
//     size_t count; (number elements)
// } sVectorList;


// parsing table 
// first() and follow() funcs
// lookahead func for next token
// match function
// skip whitespace
enum {COMMAND, STRING, NUMBER}

typedef struct s_token {
    int type;
    void *value;
    

} t_token;

int is_EoS(char *str, int i)
{
    if (i == ft_strlen(str))
        return (1);
    return (0);
}

