/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser_argv.c                                       :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/11/02 12:35:05 by adecheri       #+#    #+#                */
/*   Updated: 2025/11/02 12:35:07 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

//sets default values for execdata 
int	set_exec_def(t_exec_data *execdata, element *tokenlist, size_t pos)
{
	t_token	*check_token;

	check_token = tokenlist->pf_element_get(tokenlist, pos);
	execdata->builtin_name = set_builtins(check_token->type);
	execdata->input_is_pipe = false;
	execdata->output_is_pipe = false;
	execdata->redirections = NULL;
	return (0);
}
//sets command details in the argv for execdata
void set_command(
	t_exec_data *comm_list,
	element *tokenlist,
	int pos,
	t_token *check_token,
	int *i)
{
	if (pos > 0 && lookbehind(tokenlist, pos)->type == PIPE)
	{
		comm_list->argv[*i] = ft_strdup(check_token->value);
		comm_list->input_is_pipe = true;
	}
	else
		comm_list->argv[*i] = ft_strdup(check_token->value);
	comm_list->builtin_name = set_builtins(check_token->type);
}


// push appropiate token to argv skipping redirects and heredoc delim
//(pos + 1 < tokenlist->element_list.total && token_is_redirect(lookahead(tokenlist, pos)))
int	add_arg_to_list(
	t_exec_data *comm_list, 
	element *tokenlist, 
	int *i,
	size_t pos, 
	int pos_red)
{
	t_token	*check_token;

	check_token = (t_token *)tokenlist->element_list.tokens[pos];
	if ((pos > 0 && token_is_redirect(lookbehind(tokenlist, pos))))
		*i -= 1;
	else if (check_token->command)
		set_command(comm_list, tokenlist, pos, check_token, i);
	else if (token_is_redirect(check_token))
	{
		*i -= 1;
		add_redirect(comm_list, tokenlist, pos, pos_red);
		return (0);
	}
	else if (check_token->type == PIPE)
	{
		comm_list->output_is_pipe = true;
		return (0);
	}
	else
		comm_list->argv[*i] = ft_strdup(check_token->value);
	return (0);
}

int	fill_comm_list(
	t_exec_data *exec_data, 
	element *tokenlist, 
	size_t pos,
	int pos_red)
{
	size_t	total;
	int		i;

	i = 0;
	set_exec_def(exec_data, tokenlist, pos);
	if (pos_red < 0)
		total = tokenlist->element_list.total;
	else
		total = pos_red;
	while (pos < total)
	{
		if (add_arg_to_list(exec_data, tokenlist, &i, pos, pos_red))
		{
			free_2d_arr((void *)exec_data->argv);
			return (write(1, MALLOC_ERR, 15));
		}
		i++;
		pos++;
	}
	p_printf("Token list total = %d\n Token list i = %d\n", total, i);
	return (0);
}
