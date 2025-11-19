/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pwd.c                                             :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/19 15:31:28 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/15 19:21:40 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>

int	minishell_pwd(
	t_minishell_data *const minishell_data
)
{
	char *const	dir = ft_calloc(PATH_MAX, sizeof(char));
	int			err_check;
	
	if (!dir)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	err_check = success;
	getcwd(dir, PATH_MAX);
	if (!getcwd(dir, PATH_MAX) || dir[0] == '\0') // manage errors?
		printf("%s\n", minishell_data->cur_dir);
	else
	{
		printf("%s\n", dir);
		free(dir);
	}
	return (err_check);
}
