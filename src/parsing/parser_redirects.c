/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser_redirects.c                                  :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/08/28 12:39:43 by adecheri       #+#    #+#                */
/*   Updated: 2025/08/28 12:39:44 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// func to traverse list and find a spec symbol
int	find_symbol(t_tokenlist *tokenlist, int pos, char symbol)
{
	t_token	*check_token;

	while ((size_t)pos < tokenlist->total)
	{
		check_token = (t_token *)tokenlist->tokens[pos];
		if (ft_strchr(check_token->value, symbol) != NULL)
			return (pos);
		pos++;
	}
	return (-1);
}

//sets default values for the redirs
void	set_redir_def(t_redir_list *redirlist)
{
	redirlist->heredoc_delim = NULL;
	redirlist->type = 0;
	redirlist->src_fd = -1;
	redirlist->dest_fd = -1;
	redirlist->dest_filename = NULL;
	redirlist->next = NULL;
}

//set specific type for the redirect list
int	set_type(t_redir_list *redirlist,
				t_tokenlist *tokenlist,
				int pos)
{
	t_token	*check_token;
	t_token	*prev_token;

	check_token = tokenlist_get(tokenlist, pos);
	if (check_token->type == RED_IN)
		redirlist->type = input;
	else if (check_token->type == RED_OUT)
		redirlist->type = trunc;
	else if (check_token->type == RED_APP)
		redirlist->type = append;
	if (pos > 0)
		prev_token = lookbehind(tokenlist, pos);
	if (check_token->type == RED_IN)
		redirlist->src_fd = STDIN_FILENO;
	else if (check_token->type == RED_OUT
		|| check_token->type == RED_APP)
		redirlist->src_fd = STDOUT_FILENO;
	return (0);
}

int	set_redirect(
	t_redir_list *redirlist,
	t_tokenlist *tokenlist,
	int pos)
{
	t_token	*check_token;

	check_token = tokenlist_get(tokenlist, pos);
	set_redir_def(redirlist);
	if (tok_is_red(check_token))
	{
		if (looknxt(tokenlist, pos))
			redirlist->dest_filename = ft_strdup(looknxt(tokenlist,
						pos)->value);
		if (looknxt(tokenlist, pos + 1)
			&& tok_is_red(looknxt(tokenlist, pos + 1)))
		{
			check_token = looknxt(tokenlist, pos);
			check_token->type = STRING;
		}
		set_type(redirlist, tokenlist, pos);
	}
	return (0);
}

// p_printf("HEREDOC TOKEN VAL = %s\n", check_token->value);
// add redirect to list of execdata
int	add_redirect(t_exec_data *execdata,
					t_tokenlist *tokenlist,
					int pos,
					int pos_red)
{
	t_token			*check_token;
	t_redir_list	*redirlist;
	t_redir_list	*next;

	check_token = tokenlist_get(tokenlist, pos);
	redirlist = ft_calloc(1, sizeof(t_redir_list));
	if (!redirlist)
		return (write(1, MALLOC_ERR, 15));
	if (execdata->redirections == NULL)
		execdata->redirections = redirlist;
	else
	{
		next = execdata->redirections;
		while (next->next)
			next = next->next;
		next->next = redirlist;
	}
	if (pos > 0 && lookbehind(tokenlist, pos)->type == PIPE)
		execdata->input_is_pipe = true;
	if (check_token->type == HEREDOC)
		set_heredoc(execdata, tokenlist, pos, pos_red);
	else
		set_redirect(redirlist, tokenlist, pos);
	return (0);
}
