/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pwd.c                                             :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/19 15:31:28 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/19 16:36:43 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>

int	minishell_pwd(
	void
)
{
	// maybe declare this variable once during execution and pass it down to buitins? to avoid reallocing it every time
	// maybe doesn't make a difference since forking creates a new process anyway so memory is cipy. if anything should 
	// actually no, don't move it outside the functions unless something comes ip that would require it
//	char *const	dir_buf = ft_calloc(BUFFER_SIZE, sizeof(char));
//	
//	if (!dir_buf)
//		perror_and_return(MALLOC_ERR, MINISHELL_ERR, 1);
//
//	getcwd(dir_buf, BUFFER_SIZE);
	return (printf("%s\n", getenv("PWD")));
}
