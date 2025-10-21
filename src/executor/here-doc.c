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
#include <stdio.h>
#include <fcntl.h>

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

//static void allow_sigint_heredoc(
//)
//{
//	struct sigaction	handle_sigint;
////	struct sigaction	handle_sigquit;
//
//	sigemptyset(&handle_sigint.sa_mask);
//	sigaddset(&handle_sigint.sa_mask, SIGINT);
//	sigaddset(&handle_sigint.sa_mask, SIGQUIT);
//	handle_sigint.sa_handler = SIG_DFL;
//	handle_sigint.sa_flags = 0;
//	sigaction(SIGINT, &handle_sigint, NULL);
//}
//
static void	handle_sigint_heredoc(
)
{
	//kill(0, SIGQUIT);
//	g_msh_signal = SIGINT;
	rl_replace_line("", 0);
	rl_on_new_line();
	write(STDIN_FILENO, "\n", 1);
	rl_redisplay();
	//*rl_line_buffer = NULL;
	//ft_putstr_fd("are we looping here?\n", STDOUT_FILENO);			
}
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
	handle_sigquit.sa_handler = SIG_IGN;
	handle_sigquit.sa_flags = 0;
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
			ft_putstr_fd("PLACEHOLDER: ctrl-D'd the heredoc\n", STDERR_FILENO); // make bash like i guess
			break ;
		}
		if (ft_strncmp(input_str, heredoc_delim, delim_len) == 0)
			break ;
		ft_putstr_fd(input_str, here_doc[WRITE_END]);
		ft_putchar_fd('\n', here_doc[WRITE_END]);
		free(input_str);
	}
	if (input_str)
		free(input_str);
	return (0);
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
		// we probably don't need this check at all
		perror("heredoc child: waitpid -1");
	}
	return (exit_code);
}

int	create_here_doc(
	const char *heredoc_delim
)
{
	int				err_check;
	int				here_doc[2];
	int				pid;

	err_check = 0;
	err_check = pipe(here_doc);
	if (err_check < 0)
		perror_and_return(NULL, PIPE_ERR, extern_err, -1);
	pid = fork();
	if (pid == 0)
	{
		test_close(here_doc[READ_END]); // double check how this works just in case
		err_check = heredoc_readline_loop(heredoc_delim, here_doc);
		test_close(here_doc[WRITE_END]);
		return (HEREDOC_CHILD);
	}
	else if (pid > 0)
	{
		test_close(here_doc[WRITE_END]);
		ignore_sigint();
		err_check = heredoc_wait_for_child(pid);
		handle_signals_non_interactive();
	}
	if (err_check != 0)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		perror_and_return(NULL, PIPE_ERR, extern_err, -1);
	}
	return (here_doc[READ_END]);
}
