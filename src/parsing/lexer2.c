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

// validate input on quotes, pipes
int	val_inputline(char *str)
{
	int	i;

	i = 0;
	if (check_in_quote(str, ft_strlen(str) - 1))
		return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
	while (str[i])
	{
		if (char_is_red(str[i]))
			if (validate_redirect(str, i) != success)
				return (syntax_err);
		i++;
	}
	return (success);
}

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
