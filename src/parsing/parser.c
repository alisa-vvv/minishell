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


#include "minishell.h"

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



