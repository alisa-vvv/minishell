/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   env_utils.c                                             :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/11/21 18:02:58 by avaliull            #+#    #+#           */
/*   Updated: 2025/11/21 18:04:05 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"

int	env_util_get_cwd(
	char *cwd
)
{
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		msh_perror(NULL, "cound not set current working directory", extern_err);
		return (extern_err);
	}
	return (success);
}

