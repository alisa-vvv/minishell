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

int	set_src(t_redir_list	*redirlist, element *tokenlist, int pos)
{
	t_token	*check_token;

	check_token = tokenlist->element_list.tokens[pos];
	redirlist->src_fd = -1;
	if (pos > 0)
		check_token = tokenlist->pf_element_get(tokenlist, pos - 1);
	if (check_token->type == NUMBER)
		redirlist->src_fd = ft_atoi(check_token->value);
	return (0);
}

void	set_redir_def(t_redir_list	*redirlist)
{
	redirlist->heredoc_delim = NULL;
	redirlist->type = 0;
	redirlist->src_fd = -1;
	redirlist->dest_fd = -1;
	redirlist->dest_filename = NULL;
	redirlist->next = NULL;
}

int set_type(t_redir_list *redirlist,
	element *tokenlist, 
	int pos)
{
	t_token	*check_token;
	t_token *prev_token;

	check_token = tokenlist->pf_element_get(tokenlist, pos);
	if (check_token->type == REDIRECT_IN)
		redirlist->type = input;
	else if (check_token->type == REDIRECT_OUT)
		redirlist->type = trunc;
	else if (check_token->type == REDIRECT_OUT_APP)
		redirlist->type = append;
	prev_token = lookbehind(tokenlist, pos);
	// we might need a WHITESPACE as a token type, or more robust way of excluding
	// arguments from being numbers during lexing
	//
	// currently, if the token type is NUMBER, it is still counted as a command argument
	// it should just be a string. the only time we care about something being a number is when
	// it's directly before a redirection character, without whitespace.
	// 
	// so in "ls -l 2>outfile":
	// 2 is a file descriptor that we need to recognize as a number and set to be src_fd.
	// currently, this case adds string "2" as a command argument, BUT the token type of "2" is number
	// it should not be passed as a command argument.
	//
	// in "ls -l 2 >outfile" 2 is a command argument, not related to redirects.
	// so it should NOT be a NUMBER.
	//
	// I think the easiest/most general way is to have whitespace be a token.
	// then we can check for whitespace whenever it is relevant to syntax. and just skip it
	// when checking for other things. @alisa
	if (prev_token->type == NUMBER)
		redirlist->src_fd = ft_atoi(prev_token->value);
	else
	{
		if (check_token->type == REDIRECT_IN)
			redirlist->src_fd = STDIN_FILENO;
		else if (check_token->type == REDIRECT_OUT
				|| check_token->type == REDIRECT_OUT_APP)
			redirlist->src_fd = STDOUT_FILENO;
	}
	// ^ I added this if-else @alisa
	return (0);
}


int	set_redirect(
	t_redir_list	*redirlist, 
	element *tokenlist, 
	int pos)
{
	t_token	*check_token;

	check_token = tokenlist->pf_element_get(tokenlist, pos);
	set_redir_def(redirlist);
	if (token_is_redirect(check_token))
	{
		set_src(redirlist, tokenlist, pos);
		if (lookahead(tokenlist, pos))
			redirlist->dest_filename = ft_strdup(lookahead(tokenlist, pos)->value);
		if (lookahead(tokenlist, pos + 1) && token_is_redirect(lookahead(tokenlist, pos +1)))
		{
			check_token = lookahead(tokenlist, pos);
			check_token->type = STRING;
		}
		set_type(redirlist, tokenlist, pos);
	}
	return (0);
}

// add redirect to list of execdata
int	add_redirect(t_exec_data *execdata, element *tokenlist, int pos,
		int pos_red)
{
	t_token			*check_token;
	t_redir_list	*redirlist;
	t_redir_list 	*next;

	check_token = tokenlist->pf_element_get(tokenlist, pos);
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
