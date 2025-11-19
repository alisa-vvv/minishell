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

int env_var_realloc( // go through this again
	t_msh_data *msh_data
)
{
	char	**new_env;
	int		i;

	if (msh_data->env_mem * 2 > ENV_MAX)
		msh_perror(NULL, "Environment variable limit reached", extern_err); // add return?
	msh_data->env_mem *= 2;
	new_env = ft_calloc(msh_data->env_mem, sizeof(char *));
	if (!new_env)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err); // check return
	i = -1;
	while (msh_data->env[++i])
	{
		new_env[i] = ft_strdup(msh_data->env[i]);
		if (!new_env)
		{
			free_2d_arr((void **) new_env);
			return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err); // check return
		}
	}
	free_2d_arr((void **) msh_data->env);
	msh_data->env = new_env;
	return (success);
}

char	*env_var_find_identifier( // also change to return error?
	char *arg
)
{
	while (*arg)
	{
		if (*arg == '=' || (*arg == '+' && *(arg + 1) == '='))
			return (arg);
		else if (!ft_isalnum(*arg) && *arg != '_')
		{
			ft_putstr_fd(INVALID_IDENTIFIER, STDERR_FILENO);
			return (NULL);
		}
		arg++;
	}
	return (arg);
}

// compares the string name vs the variable in env.
// if finds match, checks if next symbol in env is '='
int	env_var_find_index( // change to return error if not found var?
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

char	*env_var_get_value(// change this to be int for errors. and write to buffer.
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
			if (!value)
				return (msh_perror(NULL, MALLOC_ERR, extern_err), NULL); // check return
			return (value);
		}
		else
			i++;
	}
	return (NULL);
}
