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

void e_printf(const char *str, ...)
{
    if (!EXP_ON)
        return;
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    va_end(args);
}
void l_printf(const char *str, ...)
{
    if (!LEX_ON)
        return;
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    va_end(args);
}


void t_printf(const char *str, ...)
{
    if (!DEBUG_ON)
        return;
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    va_end(args);
}

void p_printf(const char *str, ...)
{
    if (!PARSE_ON)
        return;
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    va_end(args);
}

void q_printf(const char *str, ...)
{
    if (!QUOTE_ON)
        return;
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    va_end(args);
}

//print enums of tokens
const char* enum_to_str(t_token_type symbols)
{
    switch (symbols)
    {
        case STRING: return "String";
        case COMMAND: return "Command";
        case NUMBER: return "Number";
        case ARGUMENT: return "Argument";
        case OPERATOR: return "Operator";
        case PARAMETER: return "Parameter";
        case FILENAME: return "Filename";
        case NON_TERMINAL: return "Non-terminal";
        case SINGLE_Q: return "Single Quotations";
        case DOUBLE_Q: return "Double Quotations";
        case OPEN_BRACKET: return "Open Bracket";
        case CLOSED_BRACKET: return "Closed Bracket";
        case DOUBLE_DOT: return "Double Dot";
        case DOT: return "Dot";
        case HYPHEN: return "Hyphen";
        case PIPE: return "PIPE";
        case PIPE_IN: return "PIPE IN";
        case PIPE_OUT: return "PIPE OUT";
        case HEREDOC: return "Heredoc";
        case HEREDOC_DEL: return "Heredoc Delimeter";
        case REDIRECT_IN: return "Redirect In";
        case REDIRECT_OUT: return "Redirect Out";
        case REDIRECT_OUT_APP: return "Redirect Out Append";
        case FORW_SLASH: return "Forward Slash";
        case BACKW_SLASH: return "Backward Slash";
        case QUESTION_MARK: return "Question Mark";
        case EXCLAM_MARK: return "Exclamation Mark";
        case CAT: return "CAT";
        case CD: return "CD";
        case ECHO: return "ECHO";
        case ENV: return "ENV";
        case EXPORT: return "EXPORT";
        case PWD: return "PWD";
        case UNSET: return "UNSET";
        case UNKNOWN: return "Unknown";
    }
    return "???";
}


//test token
void test_tokens(
	element tokenlist)
{
	size_t i;
	t_token	*token_test;

	i = 0;
	while (i < tokenlist.element_list.total)
	{
		token_test = tokenlist.element_list.tokens[i];
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

const char* enum_red_type(t_redirect_type symbols)
{
    switch (symbols)
    {
        case 	input: return "INPUT";
        case heredoc:  return "HEREDOC";
        case append: return "APPEND";
        case trunc: return "TRUNC";
    }
    return "???";
}


void test_execdata(
    t_exec_data execdata)
{
    size_t i;
	t_redir_list	**redir_head;
	t_redir_list	*cur_redirection;

    i = 0;
    p_printf("\nEXEC DATA IS:\n");
    while (execdata.argv[i] != NULL)
    {
        p_printf("Value is = %s\n", execdata.argv[i]);    
        i++;
    }
    if (execdata.builtin_name == not_builtin)
        p_printf("Is builtin = false\n");
    else 
        p_printf("Is builtin = %s\n", enum_to_str(execdata.builtin_name));
    if (execdata.input_is_pipe)
        p_printf("Input is pipe = true\n");
    else 
        p_printf("Input is pipe = false\n");
    if (execdata.output_is_pipe)
        p_printf("Output is pipe = true\n");
    else 
        p_printf("Output is pipe = false\n");
    if (execdata.redirections)
    {
		cur_redirection = *(execdata.redirections);//*redir_head;
        p_printf("Redirection Type = %s\n", enum_red_type(cur_redirection->type));
        p_printf("Redirection src_fd = %d\n", cur_redirection->src_fd);
        p_printf("Redirection dest_fd = %d\n", cur_redirection->dest_fd);
        p_printf("Redirection dest_filename = %s\n", cur_redirection->dest_filename);
        p_printf("Redirection src_filename = %s\n", cur_redirection->src_filename);
        p_printf("Redirection heredoc delimeter = %s\n", cur_redirection->heredoc_delim);
    }
    t_printf("\n");
}

// typedef struct	s_exec_data
// {
// 	char			**argv;
// 	int				is_builtin;
// 	int				input_is_pipe;
// 	int				output_is_pipe;
// 	t_redir_list	*redirections;
// }	t_exec_data;


