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


// push appropiate token to exec_data argv
int	add_arg_to_list(t_exec_data **comm_list, element *tokenlist, int pos)
{
	t_token		*check_token;

	check_token = (t_token *)tokenlist->element_list.tokens[pos];
	(*comm_list)->argv[pos] = ft_strdup(check_token->value);
	if (!(*comm_list)->argv[pos])
		return (write(1, MALLOC_ERR, 15));
	if (check_token->command && check_token->type >= CAT && check_token->type <= UNSET)
		(*comm_list)->is_builtin = check_token->type;
	if ( pos + 1 < tokenlist->element_list.total && lookahead(tokenlist, pos)->type == PIPE)
		(*comm_list)->input_is_pipe = true;
	if (check_token->type == HEREDOC)
	{
		(*comm_list)->redirections = malloc(sizeof(t_redir_list));
		if (!(*comm_list)->redirections)
			return (write(1, MALLOC_ERR, 15));
		if (pos == 0)
		{
			(*comm_list)->redirections->dest_fd = -1;
			(*comm_list)->redirections->dest_filename = NULL;
			(*comm_list)->redirections->src_fd = -1;
			(*comm_list)->redirections->src_filename = NULL;
			(*comm_list)->redirections->next = NULL;
		}
	}
	if (check_token->type == HEREDOC_DEL)
		(*comm_list)->redirections->heredoc_delim = ft_strdup(check_token->value);
	return (0);
}


//convert the tokenlist to executable data 
t_exec_data	*convert_data(element *tokenlist, size_t pos)
{
	t_exec_data	*comm_list;

	comm_list = malloc(sizeof(t_exec_data));
	if (!comm_list)
		return (NULL);
	comm_list->argv = malloc(sizeof(char *) *count_next_cm(tokenlist, pos + 1));
	if (!comm_list->argv)
		return (NULL);
	comm_list->argv[tokenlist->pf_element_total(tokenlist)] = NULL;
	while (pos < (size_t)tokenlist->element_list.total)
	{
		t_token* check_token;
		check_token = tokenlist->pf_element_get(tokenlist, pos);
		if (check_token->command)
			convert_data(tokenlist, pos);
		if (!add_arg_to_list(&comm_list, tokenlist, pos))
		{
			free_2d_arr((void *)comm_list->argv);
			return (NULL);
		}
		pos++;
	}
	return (comm_list);
}

// typedef struct	s_exec_data
// {
// 	char			**argv;
// 	int				is_builtin;
// 	int				input_is_pipe;
// 	int				output_is_pipe;
// 	t_redir_list	*redirections;
// }	t_exec_data;

