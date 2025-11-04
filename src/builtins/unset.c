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

#include "libft.h"
#include "minishell.h"
#include "minishell_env.h"

// is this even needed?
static int	check_name_len_and_validity(char *name) // find out which chars are not valid
{
	int	len;

	len = -1;
	while (name[++len])
	{
		if (name[len] == '=')
			return (-1);
	}
	return (len);
}

int	minishell_unset(
	char **argv,
	t_minishell_data *minishell_data
)
{
	int	i;
	int	len;
	int	var_index;

	i = -1;
	while (argv[++i])
	{
		len = check_name_len_and_validity(argv[i]);
		if (len < 0)
			continue ;
		var_index = env_var_find_index(minishell_data->env, argv[i], &argv[i][len]);
		if (var_index == minishell_data->env_var_count)
			continue ;
		free(minishell_data->env[var_index]);
		while (var_index < minishell_data->env_var_count - 1)
		{
			minishell_data->env[var_index] = minishell_data->env[var_index + 1];
			var_index++;
		}
		minishell_data->env_var_count -= 1;
		minishell_data->env[minishell_data->env_var_count] = NULL;
	}
	return (0);
}
