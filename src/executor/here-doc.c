/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   here-doc.c                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/05 19:39:01 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/12 19:05:47 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <readline/readline.h>


//why do we have a handler ignoring the SIGINT and SIGQUIT
//-> can we just set the global flag to change when we get in the heredoc to have main ignore signals 
//and have the non-interactive sighandler handle the sigs from there 

static void	ignore_sigint(
)
{
	struct sigaction	handle_sigint;
	struct sigaction	handle_sigquit;

	sigemptyset(&handle_sigint.sa_mask);
	sigaddset(&handle_sigint.sa_mask, SIGINT);
	sigaddset(&handle_sigint.sa_mask, SIGQUIT);
	handle_sigint.sa_handler = SIG_IGN;
	handle_sigint.sa_flags = 0;
	sigaction(SIGINT, &handle_sigint, NULL);
	sigemptyset(&handle_sigquit.sa_mask);
	sigaddset(&handle_sigquit.sa_mask, SIGINT);
	sigaddset(&handle_sigquit.sa_mask, SIGQUIT);
	handle_sigquit.sa_handler = SIG_IGN;
	handle_sigquit.sa_flags = 0;
	sigaction(SIGQUIT, &handle_sigquit, NULL);
}


static void	handle_sigint_heredoc(int sig
)
{
	//kill(0, SIGQUIT);
	if (sig == SIGQUIT)
	{
		rl_clear_history();
		g_msh_signal = SIGQUIT;
	}else if (sig == SIGINT)
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
	// rl_replace_line("", 0);
	// rl_on_new_line();
	// write(STDIN_FILENO, "\n", 1);
	// rl_redisplay();
	//*rl_line_buffer = NULL;
	//ft_putstr_fd("are we looping here?\n", STDOUT_FILENO);
		
}

//why make so many handlers for the different signals?
//can we have handle_signals_non_interactive() and ""interactive() not handle all sigs?
//
// make this more general
static void handle_signals_heredoc(
)
{
	struct sigaction	handle_sigint;
	struct sigaction	handle_sigquit;

	sigemptyset(&handle_sigint.sa_mask);
	sigaddset(&handle_sigint.sa_mask, SIGINT);
	sigaddset(&handle_sigint.sa_mask, SIGQUIT);
	handle_sigint.sa_handler = handle_sigint_heredoc;
	handle_sigint.sa_flags = 0;
	sigaction(SIGINT, &handle_sigint, NULL);
	sigemptyset(&handle_sigquit.sa_mask);
	sigaddset(&handle_sigquit.sa_mask, SIGINT);
	sigaddset(&handle_sigquit.sa_mask, SIGQUIT);
	handle_sigquit.sa_handler = handle_sigint_heredoc;
	handle_sigquit.sa_flags = 0;
	//handle_sigquit.sa_handler = handle_sigint_heredoc;
	sigaction(SIGQUIT, &handle_sigquit, NULL);
}

int	heredoc_readline_loop(
	const char *const heredoc_delim,
	int here_doc[2]

)
{
	const size_t	delim_len = ft_strlen(heredoc_delim);
	char			*input_str;
//	bool			forced_end;

	handle_signals_heredoc();
	while (1)
	{
		input_str = readline("heredoc> ");
		if (!input_str)
		{
			ft_putstr_fd("msh: warning ctrl-D'd the heredoc\n", STDERR_FILENO); // make bash like i guess
			break ;
		}
		if (ft_strncmp(input_str, heredoc_delim, delim_len) == 0) //&& input_str[delim_len] == '\0')
			break ;
		ft_putstr_fd(input_str, here_doc[WRITE_END]);
		ft_putchar_fd('\n', here_doc[WRITE_END]);
		free(input_str);
	}
	if (input_str)
		free(input_str);
	rl_clear_history();
	return (success);
}

static int heredoc_wait_for_child( // this is probably completely not needed and we can just wait(NULL)
	int pid
)
{
	int	w_status;
	int	exit_code;

	exit_code = 0;
	if (waitpid(pid, &w_status, 0) > 0) // check if there's some exit signals or codes we need to handle here
	{
		if (WIFEXITED(w_status) == true)
			exit_code = WEXITSTATUS(w_status);
		else if (WIFSIGNALED(w_status) == true)
		{
			exit_code = 128 + WTERMSIG(w_status);
			if (WCOREDUMP(w_status))
				ft_putstr_fd("Core dumped\n", STDERR_FILENO);
		}
	}
	else
	{
		// this else case is for debug only, remove it
		perror("heredoc child: waitpid -1");
	}
	return (exit_code);
}

int	create_here_doc(
	t_msh_data *const msh_data,
	const char *heredoc_delim
)
{
	int				err_check;
	int				here_doc[2];
	int				pid;

	err_check = 0;
	err_check = pipe(here_doc);
	if (err_check < 0)
		return (msh_perror(NULL, PIPE_ERR, extern_err), malloc_err);
	pid = fork();
	if (pid < 0)
	{
		safe_close(&here_doc[READ_END]);
		safe_close(&here_doc[WRITE_END]);
		return(msh_perror(NULL, FORK_ERR, extern_err), fork_err);
	}
	else if (pid == 0)
	{
		safe_close(&here_doc[READ_END]); 
		msh_data->is_parent = false;
		err_check = heredoc_readline_loop(heredoc_delim, here_doc);
		safe_close(&here_doc[WRITE_END]);
		return (child_heredoc);
	}
	else if (pid > 0)
	{
		safe_close(&here_doc[WRITE_END]);
		ignore_sigint();
		err_check = heredoc_wait_for_child(pid);
		handle_signals_non_interactive();
	}
	if (err_check != 0)
	{
		safe_close(&here_doc[READ_END]);
		return (msh_perror(NULL, PIPE_ERR, extern_err), err_check);
	}
	return (here_doc[READ_END]);
}
