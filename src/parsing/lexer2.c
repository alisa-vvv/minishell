/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   lexer2.c                                            :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/18 18:22:33 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/18 18:22:35 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

//counts how many expansions need to be done
int	count_exp(t_tokenlist *tokenlist, char symbol)
{
	int		count;
	size_t	i;
	t_token	*check_token;

	i = 0;
	count = 0;
	check_token = NULL;
	while (i < tokenlist->total)
	{
		check_token = tokenlist->tokens[i];
		if (!check_token)
			return (count);
		if (symbol == '$' && ft_strchr(check_token->value, '$'))
		{
			if (i == 0 || (i > 0 && lookbehind(tokenlist, i)
					&& lookbehind(tokenlist, i)->type != HEREDOC))
				count++;
		}
		else if (symbol == '"' && ft_strchr(check_token->value, '"'))
			count++;
		else if (symbol == '\'' && check_token->type == QUOTES)
			count++;
		i++;
	}
	return (count);
}


// //expand vars and merge operators for export
// int	expand_param(t_tokenlist *tokenlist,
// 					t_msh_data *msh_data)
// {
// 	int	count;
// 	int	err;

// 	count = count_exp(tokenlist, '$');
// 	while (count > 0)
// 	{
// 		err = exp_lexer(tokenlist, msh_data, PARAMETER, 0);
// 		if (err != success)
// 			return (err);
// 		count--;
// 	}
// 	return (success);
// }

// go through lexer and clean up data
int	check_lexer(t_tokenlist *tokenlist,
				t_msh_data *msh_data)
{
	int	err;

	err = exp_lexer(tokenlist, msh_data, PARAMETER, 0);
	if (err != success)
		return (err);
	err = exp_lexer(tokenlist, msh_data, QUOTES, 0);
	if (err != success)
		return (err);
	err = clean_lexer(tokenlist, 0);
	if (err != success)
		return (err);
	if (tokenlist->total == 0)
		return (success);
	err = index_lexer(&tokenlist);
	if (err != success)
		return (err);
	test_tokens(tokenlist);
	err = check_spcases(tokenlist);
	if (err != success)
		return (err);
	err = check_pipes(tokenlist);
	if (err != success)
		return (err);
	err = prep_execdata(tokenlist, msh_data);
	return (err);
}
