/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   debug.c                                             :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/07/08 16:45:21 by adecheri       #+#    #+#                */
/*   Updated: 2025/07/08 16:45:23 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include <stdarg.h>

void	e_printf(const char *str, ...)
{
	va_list	args;

	if (!EXP_ON)
		return ;
	va_start(args, str);
	vfprintf(stderr, str, args);
	va_end(args);
}
void	l_printf(const char *str, ...)
{
	va_list	args;

	if (!LEX_ON)
		return ;
	va_start(args, str);
	vfprintf(stderr, str, args);
	va_end(args);
}

void	t_printf(const char *str, ...)
{
	va_list	args;

	if (!DEBUG_ON)
		return ;
	va_start(args, str);
	vfprintf(stderr, str, args);
	va_end(args);
}

void	p_printf(const char *str, ...)
{
	va_list	args;

	if (!PARSE_ON)
		return ;
	va_start(args, str);
	vfprintf(stderr, str, args);
	va_end(args);
}

void	q_printf(const char *str, ...)
{
	va_list	args;

	if (!QUOTE_ON)
		return ;
	va_start(args, str);
	vfprintf(stderr, str, args);
	va_end(args);
}

//print enums of tokens
const char	*enum_to_str(t_token_type symbols)
{
    switch (symbols)
    {
        case STRING: return "String";
        case COMMAND: return "Command";
        case OPERATOR: return "Operator";
        case PARAMETER: return "Parameter";
        case NON_TERMINAL: return "Non-terminal";
        case SINGLE_Q: return "Single Quotations";
        case DOUBLE_Q: return "Double Quotations";
        case WHITESPACE: return "Whitespace";
        case PIPE: return "PIPE";
        case HEREDOC: return "Heredoc";
        case RED_IN: return "Redirect In";
        case RED_OUT: return "Redirect Out";
        case RED_APP: return "Redirect Out Append";
        case EXIT: return "EXIT";
        case CD: return "CD";
        case ECHO: return "ECHO";
        case ENV: return "ENV";
        case EXPORT: return "EXPORT";
        case PWD: return "PWD";
        case UNSET: return "UNSET";
    }
    return "???";
}

//test token
void		test_tokens(
	t_tokenlist *tokenlist)
{
	size_t	i;
	t_token	*token_test;

	i = 0;
	while (i < tokenlist->total)
	{
		token_test = tokenlist->tokens[i];
		t_printf("Token value = %s$\n", token_test->value);
		t_printf("Token type = %s\n", enum_to_str(token_test->type));
		if (token_test->command)
			t_printf("Token_cm = true\n");
		else
			t_printf("Token_cm = false\n");
		i++;
	}
	t_printf("\n");
}

const char	*enum_red_type(t_redirect_type symbols)
{
	switch (symbols)
	{
	case input:
		return ("INPUT");
	case heredoc:
		return ("HEREDOC");
	case append:
		return ("APPEND");
	case trunc:
		return ("TRUNC");
	}
	return ("???");
}
const char	*enum_builtin(t_builtin_name builtin)
{
	switch (builtin)
	{
	case builtin_cd:
		return ("CD");
	case builtin_echo:
		return ("ECHO");
	case builtin_env:
		return ("ENV");
	case builtin_exit:
		return "EXIT";
	case builtin_export:
		return "EXPORT";
	case builtin_pwd:
		return "PWD";
	case builtin_unset:
		return "UNSET";
	case not_builtin:
		return "NOT BUILTIN";
	}
	return "???";
}

void	TEST_MINISHELLDATA(t_msh_data msh_data)
{
	size_t	i;

	i = 0;
	p_printf("\nMINISHELL DATA IS:");
	if (msh_data.command_count != 0)
	{
		while (i < (size_t)msh_data.command_count)
		{
			test_execdata(*(msh_data.exec_data + i), i);
			i++;
		}
		p_printf("\n");
	}
	i = 0;
	//while (i < (size_t)msh_data.env_var_count)
	//{
	//	p_printf("Env %d = %s\n", i, msh_data.env[i]);
	//	i++;
	//}
	p_printf("Last pipeline return = %d\n", msh_data.last_pipeline_return);
	p_printf("COMMAND COUNT: %d\n", msh_data.command_count);
	p_printf("\n");
}

void		test_execdata(
	t_exec_data execdata, size_t n)
{
	size_t	i;

	i = 0;
	p_printf("\nEXEC DATA %d IS:\n", n);
	if (execdata.argv != NULL)
	{
		while (execdata.argv[i] != NULL)
		{
			p_printf("Value is = %s$\n", execdata.argv[i]);
			i++;
		}
	}
	if (execdata.builtin_name == not_builtin)
		p_printf("Is builtin = false\n");
	else
		p_printf("Is builtin = %s\n", enum_builtin(execdata.builtin_name));
	if (execdata.input_is_pipe)
		p_printf("Input is pipe = true\n");
	else
		p_printf("Input is pipe = false\n");
	if (execdata.output_is_pipe)
		p_printf("Output is pipe = true\n");
	else
		p_printf("Output is pipe = false\n");
	while (execdata.redirections)
	{
		p_printf("Redirection Type = %s$\n",
					enum_red_type(execdata.redirections->type));
		p_printf("Redirection src_fd = %d\n", execdata.redirections->src_fd);
		p_printf("Redirection dest_fd = %d\n", execdata.redirections->dest_fd);
		p_printf("Redirection dest_filename = %s$\n",
					execdata.redirections->dest_filename);
		p_printf("Redirection heredoc delimeter = %s$\n",
					execdata.redirections->heredoc_delim);
		execdata.redirections = execdata.redirections->next;
	}
	t_printf("\n");
}
