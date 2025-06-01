/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   minishell.h                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/31 15:32:15 by avaliull     #+#    #+#                  */
/*   Updated: 2025/05/31 15:33:30 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*	readline	*/ 
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h> /* isatty */
/* for waits	*/
# include <sys/types.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/wait.h>
/*	signals		*/
# include <signal.h>

# define TEST_MACRO 42

#endif
