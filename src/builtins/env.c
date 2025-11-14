/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   env.c                                             :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/24 17:17:16 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/15 19:21:59 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <unistd.h>
#include <stdio.h>

int	minishell_env(
	t_minishell_data *const minishell_data
)
{
	int	i;

	i = -1;
	if (minishell_data->env)
	{
		while(minishell_data->env[++i])
			printf("%s\n", minishell_data->env[i]);
	}
	return (0);
}
