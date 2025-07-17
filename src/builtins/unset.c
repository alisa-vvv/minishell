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

// maybe just add a list of characters that are invalid for a variable name?
static int	check_name_len_and_validity(char *name)
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

// this is horrible but i don't see a better way if we keep env as a string array
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
		printf("len: %d\n", len);
		var_index = env_var_find_index(minishell_data->environment, argv[i], &argv[i][len]);
		printf("what is var index? %d\n", var_index);
		printf("what is minishell len %d\n", minishell_data->env_var_count);
		if (var_index == minishell_data->env_var_count)
		{
			// PLACEHOLDER ADD PROPER ERROR MANAGEMENT
			printf("invalid parameter name\n");
			continue ;
		}
		while (var_index < minishell_data->env_var_count - 1)
		{
			free(minishell_data->environment[var_index]);
			minishell_data->environment[var_index] = ft_strdup(minishell_data->environment[var_index + 1]);
			if (!minishell_data->environment[var_index])
				perror_and_return(MALLOC_ERR, LIBFUNC_ERR, -1);
			var_index++;
			minishell_data->env_var_count -= 1;
		}
		free(minishell_data->environment[var_index]);
	}
	// step_1: look for variable name
	// if not exists, say: invalud parameter name
	// equal sign not included for var name, use as delimiter for actual name
	// step_2: from that variable and onwards, free(env[i]); env[i] = ft_strdup(env[i + 1]);
	// step_3: update len of env?

	return (0);
}
