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

static int	find_var_index_if_exists(
	char **environment,
	char *var_name
)
{
	int	i;
	const int	name_len = ft_strlen(var_name);

	i = 0;
	while (environment[i])
	{
		if (ft_strncmp(environment[i], var_name, name_len) != 0)
			i++;
		else
			return (i);
	}
	return (i);
}

int	minishell_export(
	char *var_name,
	char *var_val,
	t_minishell_data *minishell_data
)
{
	int		var_index;
	char	*name_equal;
	char	*var_string;

	name_equal = ft_strjoin(var_name, "=");
	if (!name_equal)
		perror_and_return(MALLOC_ERR, LIBFUNC_ERR, 1);
	var_string = ft_strjoin(name_equal, var_val);
	if (!var_string)
	{
		free(name_equal);
		perror_and_return(MALLOC_ERR, LIBFUNC_ERR, 1);
	}

	printf("\n\n\nPRE EXPORT\n\n\n");
	minishell_env(minishell_data);
	printf("\n\n\nENDPRE\n\n\n");

	var_index = find_var_index_if_exists(minishell_data->environment, var_name);
	assert(var_index <= minishell_data->env_var_count); // REMOVE ASSERT

	if (var_index == minishell_data->env_mem - 1)
		realloc_env_var(minishell_data, var_string);
	if (var_index == minishell_data->env_var_count)
		minishell_data->env_var_count += 1; 
	else if (var_index < minishell_data->env_var_count)
		free(minishell_data->environment[var_index]);
	minishell_data->environment[var_index] = var_string;

	printf("\n\n\nPOST EXPORT\n\n\n");
	minishell_env(minishell_data);

	return (0);
}
