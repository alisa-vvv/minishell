/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   env_operations.c                                  :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/15 19:44:31 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/15 20:15:08 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

char	**clone_environment(
	int *env_var_count,
	int *env_mem
)
{
	// SET A MAX VALUE FOR ALLOC SIZE
	char		**environment;
	int			i;
	int			len;
	int			alloc_size;

	alloc_size = 128;
	environment = ft_calloc(alloc_size, sizeof(char *));
	if (!environment)
		return (NULL);
	i = -1;
	while(__environ[++i])
	{
		if (i == alloc_size)
		{
			alloc_size += alloc_size;
			free_2d_arr((void **) environment);
			environment = ft_calloc(alloc_size, sizeof(char *));
			if (!environment)
				return (NULL);
			i = 0;
		}
		environment[i] = ft_strdup(__environ[i]);
		if (!environment[i])
		{
			free_2d_arr((void **) environment);
			return (NULL);
		}
	}
	*env_var_count = i;
	*env_mem = alloc_size;
	return (environment);
}

int env_var_realloc(
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
	while (minishell_data->env[++i])
	{
		new_env[i] = ft_strdup(minishell_data->env[i]);
		if (!new_env)
		{
			free_2d_arr((void **) new_env);
			perror_and_return(MALLOC_ERR, LIBFUNC_ERR, 1);
		}
	}
	new_env[i] = var_string;
	free_2d_arr((void **) minishell_data->env);
	minishell_data->env = new_env;
	return (0);
}

char	*env_var_find_identifier(
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

int	env_var_find_index(
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

char	*env_var_get_value(
	char **environment,
	char *name
)
{
	char	*value;
	int		name_len;
	int		i;

	name_len = ft_strlen(name);
	i = 0;
	while (environment[i])
	{
		if (ft_strncmp(environment[i], name, name_len) != 0)
			i++;
		else
		{
			value = ft_strdup(&environment[i][name_len + 1]);
			// add exit on malloc error?
			if (!value)
				perror_and_return(MALLOC_ERR, LIBFUNC_ERR, 1);
			return (value);
		}
	}
	return (NULL);
}
