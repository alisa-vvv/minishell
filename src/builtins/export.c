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

	if (var_index == msh_data->env_var_count)
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

static int env_var_realloc(
	t_msh_data *msh_data,
	int var_i
)
{
	char	**new_env;
	int		i;

	if (var_i == msh_data->env_mem - 1)
	{
		if (msh_data->env_mem + (msh_data->env_mem / 2) > ENV_MAX)
			return (msh_perror("export: ", TOO_MANY_ENV_VAR, msh_err), builtin_err);
		msh_data->env_mem += msh_data->env_mem / 2;
		new_env = ft_calloc(msh_data->env_mem, sizeof(char *));
		if (!new_env)
			return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
		i = -1;
		while (msh_data->env[++i])
		{
			new_env[i] = ft_strdup(msh_data->env[i]);
			if (!new_env[i])
			{
				free_2d_arr((void **) new_env);
				return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
			}
		}
		free_2d_arr((void **) msh_data->env);
		msh_data->env = new_env;
	}
	return (success);
}

int	export_var(
	char *const *argv,
	t_msh_data *const msh_data,
	char *identifier,
	t_export_i *i
)
{
	int	err_check;

	err_check = success;
	if (*identifier == '=')
		err_check = truncate_var(msh_data, i->var, argv[i->arg]);
	else if (*identifier == '+')
		err_check = append_var(msh_data, i->var, argv[i->arg], identifier);
	if (err_check == malloc_err)
		return (err_check);
	if (i->var == msh_data->env_var_count)
		msh_data->env_var_count += 1;
	return (err_check);
}

int	msh_export(
	char *const *argv,
	t_msh_data *const msh_data
)
{
	t_export_i	i;
	char		*identifier;
	int			err_check;

	if (!*argv)
		return (msh_env(msh_data));
	i.arg = -1;
	while (argv[++i.arg])
	{
		identifier = env_var_find_identifier(argv[i.arg]);
		if (!identifier)
			continue ;
		i.var = env_var_find_index(msh_data->env, argv[i.arg], identifier);
		err_check = env_var_realloc(msh_data, i.var);
		if (err_check == malloc_err)
			return (err_check);
		else if (err_check == builtin_err)
			continue ;
		err_check = export_var(argv, msh_data, identifier, &i);
		if (err_check == malloc_err)
			return (err_check);
	}
	return (err_check);
}
