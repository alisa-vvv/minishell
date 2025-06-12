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

#include "minishell.h"
#include <stdio.h>
#include <errno.h>

int	perror_and_return(
	const char *error_msg,
	int	msh_errno,
	int return_value
)
{
	if (msh_errno == MINISHELL_ERR)
	{
		// PLACEHOLDER - FOR CUSTOM ERROR MESSSAGES NOT RELATED TO LIBRARY FUNCTIONS FAILING
		printf("ADD A MINISHELL_PERROR\n");
	}
	else
		perror(error_msg);
	return (msh_errno);
}
