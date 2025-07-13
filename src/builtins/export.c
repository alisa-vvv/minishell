/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   export.c                                          :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/01 15:41:12 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/01 16:47:40 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include "libft.h"
#include "minishell.h"
#include "executor.h"

static int	realloc_env_var(
	t_minishell_data *minishell_data,
	char *var_string
)
{
	char	**new_env;
	int		i;

	if (minishell_data->env_mem * 2 > MAX_ENV)
		perror_and_return("Environment variable limit reached", MINISHELL_ERR, 1);
	minishell_data->env_mem *= 2;
	new_env = ft_calloc(minishell_data->env_mem, sizeof(char *));
	if (!new_env)
		perror_and_return(MALLOC_ERR, LIBFUNC_ERR, 1);
	i = -1;
	while (minishell_data->environment[++i])
	{
		new_env[i] = ft_strdup(minishell_data->environment[i]);
		if (!new_env)
		{
			free_2d_arr((void **) new_env);
			perror_and_return(MALLOC_ERR, LIBFUNC_ERR, 1);
		}
	}
	new_env[i] = var_string;
	free_2d_arr((void **) minishell_data->environment);
	minishell_data->environment = new_env;
	return (0);
}

static int	search_env(
	char **environment,
	char *name,
	char *identifier
)
{
	int			i;
	const int	name_len = identifier - name + 1;

	i = 0;
	while (environment[i])
	{
		if (ft_strncmp(environment[i], name, name_len) != 0)
			i++;
		else
			return (i);
	}
	return (i);
}

static char	*find_identifier(
	char *arg
)
{
	while (*arg)
	{
		if (*arg == '=')
			return (arg);
		else if (!ft_isalnum(*arg) && *arg != '_')
		{
			// REPLACE ERROR TEXT WITH A VAR?
			perror_and_return("not a valid identifier", MINISHELL_ERR, 0);
			break ;
		}
		arg++;
	}
	return (arg);
}

int	minishell_export(
	char **argv,
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
		identifier = find_identifier(*argv);
		if (!identifier)
			return (1);
		var_index = search_env(minishell_data->environment, *argv, identifier);

		assert(var_index <= minishell_data->env_var_count); // REMOVE ASSERT
    	
		if (var_index == minishell_data->env_mem - 1)
			realloc_env_var(minishell_data, *argv);
		if (var_index == minishell_data->env_var_count)
			minishell_data->env_var_count += 1; 
		else if (var_index < minishell_data->env_var_count)
			free(minishell_data->environment[var_index]);
		minishell_data->environment[var_index] = ft_strdup(*argv);
		if (!minishell_data->environment[var_index])
			perror_and_return(MALLOC_ERR, LIBFUNC_ERR, 1);
		argv++;
	}

//	printf("\n\n\nPOST EXPORT\n\n\n");
//	minishell_env(minishell_data);

	return (0);
}
