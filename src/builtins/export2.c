/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   export2.c                                               :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/12/19 16:30:22 by avaliull            #+#    #+#           */
/*   Updated: 2025/12/19 16:30:24 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "builtins.h"
#include "minishell_env.h"

static int	truncate_var(
	t_msh_data *const msh_data,
	int var_index,
	char *arg
)
{
	if (var_index < msh_data->env_var_count
		&& msh_data->env[var_index])
		free(msh_data->env[var_index]);
	msh_data->env[var_index] = ft_strdup(arg);
	if (!msh_data->env[var_index])
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	return (success);
}

static int	append_var(
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

int	export_var(
	char *const *argv,
	t_msh_data *const msh_data,
	char *identifier,
	t_export_i *i
)
{
	int	err_check;

	if (*identifier == argv[i->arg][0])
	{
		msh_perror("export: ", INVALID_IDENTIFIER, msh_err);
		ft_putstr_fd(argv[i->arg], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (builtin_err);
	}
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
