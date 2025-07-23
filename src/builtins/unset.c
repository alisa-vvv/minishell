/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   unset.c                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/13 17:53:39 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/13 18:45:30 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_unset(
	char **argv,
	t_minishell_data minishell_data
)
{
	// step_1: look for variable name
	// if not exists, say: invalud parameter name
	// equal sign not included for var name, use as delimiter for actual name
	// step_2: from that variable and onwards, free(env[i]); env[i] = ft_strdup(env[i + 1]);
	// step_3: update len of env?

	return (0);
}
