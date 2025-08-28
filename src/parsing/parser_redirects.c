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


int set_redirect(
	t_exec_data **execdata,
	element *tokenlist,
	int pos, 
	int pos_red
)
{
	t_token *check_token;
	check_token = tokenlist->pf_element_get(tokenlist, pos);
	if (check_token->type == REDIRECT_IN)
	{
		(*execdata)->redirections->type = input;
		(*execdata)->redirections->dest_filename = ft_strdup(lookahead(tokenlist, pos -2)->value);
		(*execdata)->redirections->src_filename = ft_strdup(lookahead(tokenlist, pos)->value);
	}
	else if (check_token->type == REDIRECT_OUT_APP)
	{
		(*execdata)->redirections->type = append;
		(*execdata)->redirections->dest_filename = ft_strdup(lookahead(tokenlist, pos)->value);
		(*execdata)->redirections->src_filename = ft_strdup(lookahead(tokenlist, pos -2)->value);
	}
	else if (check_token->type == REDIRECT_OUT)
	{
		
		(*execdata)->redirections->type = trunc;
		(*execdata)->redirections->dest_filename = ft_strdup(lookahead(tokenlist, pos)->value);
		(*execdata)->redirections->src_filename = ft_strdup(lookahead(tokenlist, pos -2)->value);
	}
	(*execdata)->redirections->heredoc_delim = NULL;
	(*execdata)->redirections->src_fd = -1;
	(*execdata)->redirections->dest_fd = -1;
	return (0);
}

//add redirect to list of execdata
int add_redirect(
	t_exec_data **execdata,
	element *tokenlist, 
	int pos,
	int pos_red)
{
	t_token *check_token;
	t_redir_list *redirlist;
	
	check_token = tokenlist->pf_element_get(tokenlist, pos);
	if (check_token->type == PIPE)
		return (0);
	redirlist = ft_calloc(1, sizeof(t_redir_list));
	if (!redirlist)
		return (write(1, MALLOC_ERR, 15));
	if ((*execdata)->redirections == NULL)
		(*execdata)->redirections = redirlist;
	else
	{
		t_redir_list *next;
		
		next = (*execdata)->redirections->next;
		while (next)
		{
			next = next->next;
		}
		next = redirlist;
	}
	if (token_is_redirect(check_token))
		set_redirect(execdata,tokenlist, pos, pos_red);		
	return (0);
}
