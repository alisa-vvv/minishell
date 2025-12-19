/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   env_default_var_validation_shlvl.c                      :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/12/19 17:02:33 by avaliull            #+#    #+#           */
/*   Updated: 2025/12/19 17:03:32 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "libft.h"
#include "minishell.h"
#include "minishell_env.h"
#include "builtins.h"

static int	iterate_shlvl(
	t_msh_data *const msh_data,
	char **var_value,
	char *shlvl_string
)
{
	int				err_check;
	unsigned long	shlvl_int;
	char			*new_shlvl_val;
	int				i;

	err_check = 0;
	shlvl_int = 0;
	i = -1;
	while (shlvl_string[++i])
		shlvl_int = shlvl_int * 10 + shlvl_string[i] - '0';
	if (shlvl_int >= INT_MAX)
		shlvl_int = 0;
	shlvl_int += 1;
	new_shlvl_val = ft_itoa(shlvl_int);
	if (!new_shlvl_val)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	var_value[0] = ft_strjoin("SHLVL=", new_shlvl_val);
	ft_safe_free((unsigned char **) &new_shlvl_val);
	if (!var_value[0])
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	err_check = msh_export(var_value, msh_data);
	ft_safe_free((unsigned char **) &var_value[0]);
	return (err_check);
}

static int	shlvl_check(
	t_msh_data *const msh_data,
	char **var_value,
	bool *broken_shlvl,
	char *shlvl_val
)
{
	int	err_check;
	int	i;

	err_check = success;
	i = -1;
	*broken_shlvl = false;
	while (shlvl_val[++i])
	{
		if (!ft_isdigit(shlvl_val[i]) || i > 10)
		{
			err_check = export_default_var(msh_data, var_value, "SHLVL=1");
			*broken_shlvl = true;
			break ;
		}
	}
	return (err_check);
}

int	update_shlvl(
	t_msh_data *const msh_data,
	char **var_value
)
{
	int		err_check;
	char	*shlvl_val;
	bool	broken_shlvl;
	int		exists;

	err_check = success;
	shlvl_val = NULL;
	exists = 0;
	err_check = env_var_get_value(msh_data->env, "SHLVL", &shlvl_val, &exists);
	if (err_check != success)
		return (err_check);
	broken_shlvl = false;
	err_check = shlvl_check(msh_data, var_value, &broken_shlvl, shlvl_val);
	if (broken_shlvl == false)
		iterate_shlvl(msh_data, var_value, shlvl_val);
	ft_safe_free((unsigned char **) &shlvl_val);
	return (err_check);
}
