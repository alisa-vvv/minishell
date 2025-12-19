/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   errors.c                                          :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/09 19:06:37 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/12 18:11:01 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>

void	msh_perror(
	char *const error_prefix,
	char *const error_msg,
	t_error_relation relation)
{
	ft_putstr_fd("msh: ", STDERR_FILENO);
	if (error_prefix)
		ft_putstr_fd(error_prefix, STDERR_FILENO);
	if (!error_msg)
		return ;
	if (relation == extern_err)
		perror(error_msg);
	else
	{
		ft_putstr_fd(error_msg, STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
	}
}
