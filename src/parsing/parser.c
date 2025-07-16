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


//func returning next token type in the array
int lookahead(element **tokenlist, size_t index)
{
    t_token *check_token;
    check_token = (t_token *)(*tokenlist)->pf_element_get(&tokenlist, index + 1);
    if (!check_token)
        return (-1);
    return (check_token->type);
}




// typedef struct	s_exec_data
// {
// 	char			**argv;
// 	int				is_builtin;
// 	int				input_is_pipe;
// 	int				output_is_pipe;
// 	t_redir_list	*redirections;
// }	t_exec_data;


