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
#include "executor.h"
#include "minishell_env.h"
#include <stdio.h>

int	minishell_pwd(
	t_minishell_data *const minishell_data
)
{
	// alloc this variable in advance? // why
	char *const	dir = ft_calloc(PATH_MAX, sizeof(char));
	int			err_check;
	
	err_check = 0;
	getcwd(dir, PATH_MAX);
	//if (!dir)
	//{
	//	printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n"); // do we need to error manage?
	//	return (-1);
	//}
	if (!getcwd(dir, PATH_MAX) || dir[0] == '\0') // manage the errors?
		printf("%s\n", minishell_data->cur_dir);
	else
	{
		printf("%s\n", dir);
		free(dir);
	}
	return (err_check);
}
