/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   lexer_tokens.c                                      :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/12/07 20:34:47 by adecheri       #+#    #+#                */
/*   Updated: 2025/12/07 20:34:49 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// make a new token in the array
t_token	*new_token(
	t_tokenlist *tokenlist,
	char *str,
	int len,
	bool expand)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), NULL);
	token->value = ft_calloc((len + 1), sizeof(char));
	if (!token->value)
	{
		msh_perror(NULL, MALLOC_ERR, extern_err);
		free(token);
		return (NULL);
	}
	ft_strlcpy(token->value, str, len);
	if (!expand)
		token->type = match_token(token->value);
	else
		token->type = STRING;
	token->command = false;
	token->pos = tokenlist_total(tokenlist);
	return (token);
}

//add new token to the list and updates total
int	add_token(
	t_tokenlist *tokenlist,
	char *str,
	size_t len)
{
	t_token	*token;

	token = NULL;
	token = new_token(tokenlist, str, len, false);
	if (!token)
		return (msh_perror(NULL, MALLOC_ERR, malloc_err), malloc_err);
	return (tokenlist_add(tokenlist, token));
}

//preps str for redirect splitting and adding tokens
int	prep_token(
	t_tokenlist *tokenlist,
	char *str,
	int i,
	int len)
{
	char	*str_b_token;
	int		status;

	str_b_token = ft_calloc((len + 1), sizeof(char));
	if (!str_b_token)
		return (msh_perror(NULL, MALLOC_ERR, malloc_err), malloc_err);
	ft_strlcpy(str_b_token, str + i - len, len + 1);
	status = add_token(tokenlist, str_b_token, len + 1);
	ft_safe_free((unsigned char **)&str_b_token);
	return (status);
}
