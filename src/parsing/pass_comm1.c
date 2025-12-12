/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   pass_comm1.c                                            :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/12/10 20:13:17 by avaliull            #+#    #+#           */
/*   Updated: 2025/12/11 19:12:39 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	add_arg_to_execdata(
	t_msh_data *msh_data,
	t_token *cur_token,
	const size_t exdata_i,
	size_t *argv_i
)
{
	if (*argv_i == 0 && (cur_token->type >= CD && cur_token->type <= UNSET)
		&& msh_data->exec_data[exdata_i].builtin_name == not_builtin)
	{
		msh_data->exec_data[exdata_i].builtin_name = set_builtins(cur_token);
	}
	msh_data->exec_data[exdata_i].argv[*argv_i] = ft_strdup(cur_token->value);
	if (!msh_data->exec_data[exdata_i].argv[*argv_i])
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	*argv_i += 1;
	return (success);
}

int	add_redirection(
	t_redir_list **first,
	t_token_type type,
	char *next_token_val
)
{
	t_redir_list	*redir_node;
	
	redir_node = ft_calloc(1, sizeof(t_redir_list));
	if (!redir_node)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	if (type == RED_OUT)
		redir_node->type = trunc;
	if (type == RED_APP)
		redir_node->type = append;
	if (type == HEREDOC)
		redir_node->type = heredoc;
	if (next_token_val && type != HEREDOC)//  yes if we expand on empty, next token_val can be '\0' || I believe we do check if reds are not the last token 
	{
		redir_node->dest_filename = ft_strdup(next_token_val);
		if (!redir_node->dest_filename)
			return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	}
	else if (next_token_val && type == HEREDOC)
	{
		redir_node->heredoc_delim = ft_strdup(next_token_val);
		if (!redir_node->heredoc_delim)
			return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	}
	new_redir_elem(first, redir_node);
	return (success);
}

int set_redir_comm(
	t_tokenlist *tokenlist,
	t_msh_data *msh_data,
	t_passcom_indexes *i
)
{
	int 			err;
	t_token 		*cur_token; 
	t_token 		*next_token;

	err = success;
	cur_token = tokenlist_get(tokenlist, i->tok);
	next_token = looknxt(tokenlist, i->tok);
	if (!next_token)
		return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
	if (!legit_token(next_token))
		return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
	err = add_redirection(&msh_data->exec_data[i->exe].redirections,
								cur_token->type, next_token->value);
	i->tok++;
	return (err);
}

//when encountered pipe, set data 
int set_pipe_data(
	t_msh_data *msh_data,
	t_tokenlist *tokenlist, 
	t_passcom_indexes *i
)
{
	int err;
	t_token *next_token;

	err = success;

	next_token = looknxt(tokenlist, i->tok);
	if (!next_token || next_token->type == PIPE)
		return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
	msh_data->exec_data[i->exe].output_is_pipe = true;
	i->exe++;
	i->argve = 0;
	i->mem = 128;
	msh_data->exec_data[i->exe].argv = ft_calloc(i->mem, sizeof (char *));
	if (!msh_data->exec_data[i->exe].argv)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	msh_data->exec_data[i->exe].input_is_pipe = true;
	return (err);
}

// struct "i" holds indexes for tokenlist, argv, and exec_data, as well as the
// amount of memory currently used by argv.
// tok for tokenlist, argve for argv, exe for exec_data, mem for memory.
int pass_comm(
	t_tokenlist *tokenlist,
	t_msh_data *msh_data
)
{
	int					err;
	t_token				*cur_token;
	t_passcom_indexes	i;

	//test_tokens(tokenlist);
	i.tok = 0;
	i.argve = 0;
	i.exe = 0;
	i.mem = 128;
	err = success;
	while (i.tok < tokenlist->total && err == success)
	{
		if (reallocate_argv(msh_data, i.mem, i.argve, i.exe) != success)
			return (malloc_err);
		cur_token = tokenlist_get(tokenlist, i.tok);
		if (legit_token(cur_token)
				|| (cur_token->type >= CD && cur_token->type <= UNSET))
			err = add_arg_to_execdata(msh_data, cur_token, i.exe, &i.argve);
		else if (tok_is_red(cur_token))
			err = set_redir_comm(tokenlist, msh_data, &i);
		else if (cur_token->type == PIPE)
			err = set_pipe_data(msh_data, tokenlist, &i);
		i.tok++;
	}
	return (err);
}
