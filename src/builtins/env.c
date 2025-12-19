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
#include <stdio.h>
#include <unistd.h>

int	msh_env(
	t_msh_data *const msh_data)
{
	int	i;

	i = -1;
	if (msh_data->env)
	{
		while (msh_data->env[++i])
			printf("%s\n", msh_data->env[i]);
	}
	return (0);
}
