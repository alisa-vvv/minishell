/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   env.c                                             :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/24 17:17:16 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/24 17:43:30 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include <executor.h>
#include <unistd.h>
#include <stdio.h>

int	minishell_env(
	t_minishell_data *const minishell_data
)
{
	int	i;

	i = -1;
	while(minishell_data->environment[++i])
		printf("%s\n", minishell_data->environment[i]);
	return (0);
}
