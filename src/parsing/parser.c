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

// construc parsing table 
// first() and follow() funcs
// lookahead func for check next token
// match function to check and expand on aliases 

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
    check_token = (t_token *)(*tokenlist)->pf_element_get((*tokenlist), index + 1);
    if (!check_token)
        return (-1);
    return (check_token->type);
}

//check lexer on expansion and quotes
int exp_lexer(element *tokenlist, t_minishell_data **minishell_data, int type)
{
	size_t i;
	t_token *check_token;
    i = 0;	
    while (i < (size_t)tokenlist->element_list.total)
    {
		check_token = (t_token *)tokenlist->element_list.tokens[i];
		//e_printf("TYPE = %d \n", (int)check_token->type);
        if ((int)check_token->type == PARAMETER && type == PARAMETER  || ((int)check_token->type == DOUBLE_Q && type == DOUBLE_Q))
		{
			rm_quotes(tokenlist, i, '"');
			if (type == DOUBLE_Q && ft_strchr(check_token->value, '$') != NULL)
			{
				if (expand_var(&tokenlist, i, minishell_data, true))
					i--;
			}
			else if (type == PARAMETER)
			{
				if (expand_var(&tokenlist, i, minishell_data, false))
					i--;
			}
		}
		else if ((int)check_token->type == SINGLE_Q && type == SINGLE_Q)
			rm_quotes(tokenlist, i, '\'');
        i++;
    }
	return (0);
}

// t_exec_data *convert_data(element *tokenlist)
// {
// 	size_t i;
// 	i = 0; 

// 	while(i < (size_t)tokenlist->element_list.total)
// 	{
		
// 	}
// }

// typedef struct	s_exec_data
// {
// 	char			**argv;
// 	int				is_builtin;
// 	int				input_is_pipe;
// 	int				output_is_pipe;
// 	t_redir_list	*redirections;
// }	t_exec_data;


// *t_exec_data convert_data(element *tokenlist, )

