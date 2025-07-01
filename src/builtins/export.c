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

#include "libft.h"
#include "minishell.h"

int	minishell_export(
char *var_name,
char *var_val,
t_minishell_data *minishell_data
)
{
	int		i;
	char	**new_env;
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
	if (minishell_data->env_var_count + 1 >= minishell_data->env_mem)
	{
		// CHANGE THIS TYO BE RESONABLE
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
	}
	else
		minishell_data->environment[minishell_data->env_var_count + 1] = var_string;
	minishell_data->env_var_count += 1; 
	return (0);
}
