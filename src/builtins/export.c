/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   export.c                                          :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/01 15:41:12 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 16:51:00 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

//REMOVE
#include <assert.h>

#include "libft.h"
#include "minishell.h"
#include "builtins.h"
#include "minishell_env.h"

static int truncate_var(
	t_msh_data *const msh_data,
	int var_index,
	char *arg
)
{
	if (var_index < msh_data->env_var_count &&
			msh_data->env[var_index])
		free(msh_data->env[var_index]);
	msh_data->env[var_index] = ft_strdup(arg);
	if (!msh_data->env[var_index])
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	return (success);
}

static int append_var(
	t_msh_data *const msh_data,
	int var_index,
	char *arg,
	char *identifier
)
{
	char	*appended_var;

	if (var_index == msh_data->env_var_count) // check logic here, something's fishy
	{
		*identifier = '\0';
		appended_var = ft_strjoin(arg, identifier + 1);
		*identifier = '+';
	}
	else
		appended_var = ft_strjoin(msh_data->env[var_index], identifier + 2);
	if (!appended_var)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	if (msh_data->env[var_index])
		free(msh_data->env[var_index]);
	msh_data->env[var_index] = appended_var;
	return (success);
}

int	msh_export(
	char *const *argv,
	t_msh_data *const msh_data
)
{
	int		var_i;
	int		arg_i;
	char	*identifier;
	int		err_check;

	err_check = success;
	if (!*argv)
		return (msh_env(msh_data));
	arg_i = -1;
	while (argv[++arg_i])
	{
		
		identifier = env_var_find_identifier(argv[arg_i]);
		if (!identifier)
			continue ;
		var_i = env_var_find_index(msh_data->env, argv[arg_i], identifier);
		if (var_i == msh_data->env_mem - 1)
		{
			err_check = env_var_realloc(msh_data);
			if (err_check == malloc_err)
				return (err_check);
			else if (err_check == msh_err)
				continue ;
		}
		if (*identifier == '=')
			err_check = truncate_var(msh_data, var_i, argv[arg_i]);
		else if (*identifier == '+')
			err_check = append_var(msh_data, var_i, argv[arg_i], identifier);
		if (err_check != success)
			return (err_check);
		if (var_i == msh_data->env_var_count)
			msh_data->env_var_count += 1;  // check update here, something's fishy
	}
	return (err_check);
}
