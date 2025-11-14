/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   env_creation.c                                          :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/11/14 19:30:33 by avaliull            #+#    #+#           */
/*   Updated: 2025/11/14 20:23:49 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "builtins.h"
#include "minishell_env.h"

int	set_default_env(
	t_minishell_data *const minishell_data,
	int *env_var_count
)
{
	char	*path_var;

	path_var = ft_calloc(PATH_MAX, sizeof(char)); 
	minishell_data->env[0] = ft_strdup(STD_PATH);
	minishell_data->env[1] = ft_strdup("SHLVL=1");
	getcwd(path_var, PATH_MAX); //err manage
	minishell_data->env[2] = ft_strjoin("PWD=\0", path_var);
	*env_var_count = 3;
	return (success); // error manage!
}

char	**clone_env(
	t_minishell_data *const minishell_data,
	int *env_var_count,
	int *env_mem
)
{
	// SET A MAX VALUE FOR ALLOC SIZE
	extern	char **environ;
	char	**env; // yuck
	int		i;
	int		alloc_size;

	alloc_size = 128;
	minishell_data->env = ft_calloc(alloc_size, sizeof(char *));
	if (!minishell_data->env)
		return (NULL);
	env = minishell_data->env; // YUCK
	if (environ[0] == NULL)
		set_default_env(minishell_data, env_var_count);
	else
	{
		i = -1;
		while(environ[++i])
		{
			if (i == alloc_size)
			{
				alloc_size += alloc_size / 2;
				free_2d_arr((void **) env);
				env = ft_calloc(alloc_size, sizeof(char *));
				if (!env)
					return (NULL);
				i = 0;
			}
			env[i] = ft_strdup(environ[i]);
			if (!env[i])
			{
				free_2d_arr((void **) env);
				return (NULL);
			}
		}
		*env_var_count = i;
	}
	*env_mem = alloc_size;
	return (env);
}
