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

// sets default values for execdata
int	set_exec_def(t_exec_data *execdata, t_tokenlist *tokenlist, size_t pos)
{
	t_token	*check_token;

	check_token = tokenlist_get(tokenlist, pos);
	execdata->builtin_name = set_builtins(check_token);
	execdata->input_is_pipe = false;
	execdata->output_is_pipe = false;
	execdata->redirections = NULL;
	return (0);
}
