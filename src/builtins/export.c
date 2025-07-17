/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   export.c                                          :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/01 15:41:12 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/15 19:57:06 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

//REMOVE
#include <assert.h>

#include "libft.h"
#include "minishell.h"
#include "builtins.h"
#include "minishell_env.h"

int	minishell_export(
	char *const *argv,
	t_minishell_data *minishell_data
)
{
	int		var_index;
	char	*identifier;

//	printf("\n\n\nPRE EXPORT\n\n\n");
//	minishell_env(minishell_data);
//	printf("\n\n\nENDPRE\n\n\n");

	if (!*argv)
		minishell_env(minishell_data);
	while (*argv)
	{
		identifier = env_var_find_identifier(*argv);
		if (!identifier)
			return (-1);
		var_index = env_var_find_index(minishell_data->environment, *argv, identifier);

		assert(var_index <= minishell_data->env_var_count); // REMOVE ASSERT
    	
		if (var_index == minishell_data->env_mem - 1)
			env_var_realloc(minishell_data, *argv);
		if (var_index == minishell_data->env_var_count)
			minishell_data->env_var_count += 1; 
		else if (var_index < minishell_data->env_var_count)
			free(minishell_data->environment[var_index]);
		minishell_data->environment[var_index] = ft_strdup(*argv);
		if (!minishell_data->environment[var_index])
			perror_and_return(MALLOC_ERR, LIBFUNC_ERR, -1);
		argv++;
	}

	//printf("\n\n\nPOST EXPORT\n\n\n");
	//minishell_env(minishell_data);

	//char **test_unset = ft_calloc(2, sizeof(char *));
	//test_unset[0] = "var";
	//test_unset[1] = "var2";
	//minishell_unset(test_unset, minishell_data);

	//printf("\n\n\nPOST UNSET\n\n\n");
	//minishell_env(minishell_data);
	return (0);
}
