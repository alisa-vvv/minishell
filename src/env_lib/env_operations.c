/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   env_operations.c                                  :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/15 19:44:31 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 16:41:07 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "minishell_env.h"

char	**clone_env(
	int *env_var_count,
	int *env_mem
)
{
	// SET A MAX VALUE FOR ALLOC SIZE
	char		**env;
	int			i;
	int			len;
	int			alloc_size;

	alloc_size = 128;
	env = ft_calloc(alloc_size, sizeof(char *));
	if (!env)
		return (NULL);
	i = -1;
	while(__environ[++i])
	{
		if (i == alloc_size)
		{
			alloc_size += alloc_size;
			free_2d_arr((void **) env);
			env = ft_calloc(alloc_size, sizeof(char *));
			if (!env)
				return (NULL);
			i = 0;
		}
		env[i] = ft_strdup(__environ[i]);
		if (!env[i])
		{
			free_2d_arr((void **) env);
			return (NULL);
		}
	}
	*env_var_count = i;
	*env_mem = alloc_size;
	return (env);
}

int env_var_realloc(
	t_minishell_data *minishell_data
)
{
	char	**new_env;
	int		i;

	if (minishell_data->env_mem * 2 > MAX_ENV)
		perror_and_return(NULL, "Environment variable limit reached", extern_err, 1);
	minishell_data->env_mem *= 2;
	new_env = ft_calloc(minishell_data->env_mem, sizeof(char *));
	if (!new_env)
		perror_and_return(NULL, MALLOC_ERR, extern_err, -1);
	i = -1;
	while (minishell_data->env[++i])
	{
		new_env[i] = ft_strdup(minishell_data->env[i]);
		if (!new_env)
		{
			free_2d_arr((void **) new_env);
			perror_and_return(NULL, MALLOC_ERR, extern_err, -1);
		}
	}
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
		if (*arg == '=' || (*arg == '+' && *(arg + 1) == '='))
			return (arg);
		else if (!ft_isalnum(*arg) && *arg != '_')
		{
			printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
			//ft_putstr_fd(INVALID_IDENTIFIER, STDERR_FILENO);
			//ft_putstr_fd(arg, STDERR_FILENO);
			//ft_putchar_fd('\n', STDERR_FILENO);
			return (NULL);
		}
		arg++;
	}
	return (arg);
}

// compares the string name vs the variable in env.
// if finds match, checks if next symbol in env is '='
int	env_var_find_index(
	char **env,
	char *name,
	char *identifier
)
{
	int			i;
	const int	name_len = identifier - name;

	i = 0;

	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 &&
				env[i][name_len] == '=')
			return (i);
		else
				i++;
	}
	return (i);
}

char	*env_var_get_value(
	char **env,
	char *name
)
{
	char	*value;
	int		name_len;
	int		i;

	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 &&
				env[i][name_len] == '=')
		{
			value = ft_strdup(&env[i][name_len + 1]);
			// add exit on malloc error?
			if (!value)
				perror_and_return(NULL, MALLOC_ERR, extern_err, 0);
			return (value);
		}
		else
			i++;
	}
	return (NULL);
}
