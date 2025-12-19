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

#include "libft.h"
#include "minishell.h"
#include "builtins.h"
#include "minishell_env.h"

static int	env_var_realloc(
	t_msh_data *msh_data,
	int var_i
)
{
	char	**new_env;
	int		i;

	if (var_i == msh_data->env_mem - 1)
	{
		if (msh_data->env_mem + (msh_data->env_mem / 2) > ENV_MAX)
			return (msh_perror("export: ", ENV_VAR_MAX, msh_err), builtin_err);
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

static char	*find_identifier(
	char *const *argv,
	t_export_i i,
	int *was_err

)
{
	char	*identifier;

	identifier = env_var_find_identifier(argv[i.arg]);
	if (!identifier)
		*was_err = builtin_err;
	return (identifier);
}

static int	export_all_args(
	char *const *argv,
	char *identifier,
	t_msh_data *msh_data,
	int *was_err
)
{
	t_export_i	i;
	int			err_check;

	err_check = success;
	i.arg = -1;
	while (argv[++i.arg])
	{
		identifier = find_identifier(argv, i, was_err);
		if (!identifier)
			continue ;
		i.var = env_var_find_index(msh_data->env, argv[i.arg], identifier);
		err_check = env_var_realloc(msh_data, i.var);
		if (err_check != success)
			return (err_check);
		else if (err_check == builtin_err)
		{
			*was_err = builtin_err;
			continue ;
		}
		err_check = export_var(argv, msh_data, identifier, &i);
		if (err_check == malloc_err)
			return (err_check);
	}
	return (err_check);
}

int	msh_export(
	char *const *argv,
	t_msh_data *const msh_data
)
{
	char		*identifier;
	int			err_check;
	int			was_err;

	if (!*argv)
		return (msh_env(msh_data));
	identifier = NULL;
	was_err = success;
	err_check = success;
	err_check = export_all_args(argv, identifier, msh_data, &was_err);
	if (err_check == success && was_err != success)
		return (was_err);
	else
		return (err_check);
}
