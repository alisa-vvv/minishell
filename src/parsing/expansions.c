/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   expansions.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/17 14:24:03 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/17 14:24:05 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// $ expansion, env
// tilde expansion?
// $0 name of the shell script or command
// $1-9 indicated postiional arg
// $# number of shell args
// $@ value of each shell arg
// $* value of all shell arg

// var expansion: $sign, $@sign
// path expansion
//--> make expansion first, remove quotes later
// expand $EMPTY to nothing
// look for return value in msh struct when accessing $?

int	fpos_in_str(const char *str, char symbol)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == symbol)
			return (i);
		i++;
	}
	return (i);
}

bool count_q(const char *str, int i, char symbol)
{

	if (str[i] && str[i] == symbol)
	{
		if (check_in_quote_s(str, i, '\''))
			return (false);
		else if (!check_in_quote_s(str, i, '\'') && check_in_quote_s(str, i, '"'))
			return (false);
		else if (!check_in_quote_s(str, i, '\'') && !check_in_quote_s(str, i, '"'))
			return (true);
	}
	return (false);
}



// counts how many occurrences of a symbol are in one str
int	count_occ(const char *str, char symbol, bool inside)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (inside)
		{
			//prolly not right yet --> needs to not count if quoted quotes are there 
			if (count_q(str, i, symbol))
				count++;
		}
		else if (inside == false)
		{
			if (str[i] == symbol)
				count++;
		}
		i++;
	}
	return (count);
}

// ft_strchr(check_token->value, '$')
// expands unquoted var to the relevant environment value, keeping the rest of the string intact or replacing empty strings if env value is not present

int	expand_unquoted(t_tokenlist *tokenlist, t_token *check_token, char *name,
		int pos, char *env_value)
{
	int	count;
	char *new_str;

	count = count_occ(check_token->value, '$', false);
	if (!env_value && count == 1)
		return (tokenlist_delete(tokenlist, pos), 0);
	else if (!env_value)
		env_value = "";
	if (count >= 1)
	{
		new_str = exp_str_token(check_token->value, env_value, ft_strlen(name)+ 1);
		tokenlist_set(tokenlist, pos, new_token(tokenlist, new_str, ft_strlen(new_str) + 1));
		(ft_safe_free((unsigned char **)&new_str));
	}
	else
		tokenlist_set(tokenlist, pos, new_token(tokenlist, env_value,
				ft_strlen(env_value) + 1));
	check_token = tokenlist->tokens[pos];
	if (!check_token || !check_token->value)
		tokenlist_delete(tokenlist, pos);
	return (0);
}


bool	check_quote_e(char *str, int pos)
{
	int	quote;

	quote = symbol_in_quote(str, '$');
	if (quote == '"' && !check_in_quote_s(str, pos, '\''))
		return (1);
	else if (quote == '\'' && check_in_quote_s(str, pos, '"'))
		return (1);
	else if (quote == '\'' && !check_in_quote_s(str, pos, '\''))
		return (1);
	else
		return (0);
}



//expands quoted var when they fulfill check_quote_e conditions
void	expand_quoted(t_tokenlist *tokenlist, char *name, size_t pos,
		char *env_value)
{
	int		offset;
	t_token	*check_token;
	int		start;
	
	check_token = tokenlist->tokens[pos];
	start = fpos_in_str(check_token->value, '$');
	if (check_quote_e(check_token->value, start))
	{
        char *new_str;
	    t_token *n_token;

		offset = 0;
		n_token = NULL;
		if (!env_value || !name)
			env_value = "";
		offset = ft_strlen(name) + 1;
		new_str = exp_str_token(check_token->value, env_value, offset);
		n_token = new_token(tokenlist, new_str, ft_strlen(new_str) + 1);
		if (!n_token)
			tokenlist_free(tokenlist);
		tokenlist_set(tokenlist, pos, n_token);
		(ft_safe_free((unsigned char **)&new_str));
	}
}

// expand known var and otherwise delete and re-position all tokens
int	expand_var(t_tokenlist **tokenlist, int pos, t_msh_data *msh_data,
		t_token *check_token, bool quoted)
{
	char	*name;
	char	*env_value;
	int		count;

	name = NULL;
	env_value = NULL;
	count = count_occ(check_token->value, '$', false);
	p_printf("COUNT = %d\n", count);
	while (count > 0)
	{
		check_token = (*tokenlist)->tokens[pos];
		name = refine_name_var(check_token->value, name, '$');
		p_printf("\nNAME= %s \n", name);
		p_printf("TOKEN VAL= %s\n", check_token->value);
		if (name && (ft_strncmp(name, "?", 2) == 0))
            env_value = ft_itoa(msh_data->last_pipeline_return);
			//return (printf("%d\n", msh_data->last_pipeline_return));
		else if (env_var_get_value(msh_data->env, name, &env_value) != success)
			dprintf(STDERR_FILENO, "Failed to malloc env\n");
		// if (!env_value && ft_strncmp(check_token->value, "$", 2) == 0)
		// 	return (ft_safe_free((unsigned char **)&name), tokenlist_delete(*tokenlist, pos), 0);
		if (quoted)
			expand_quoted(*tokenlist, name, pos, env_value);
		else
			expand_unquoted(*tokenlist, check_token, name, pos, env_value);
		(ft_safe_free((unsigned char **)&name), ft_safe_free((unsigned char **)&env_value));
		index_lexer(tokenlist);
		count--;
	}
	return (0);
}

// check_in_quote(check_token->value, lpos_in_str(check_token->value, '$')
// check lexer on expansion and quotes
int	exp_lexer(t_tokenlist *tokenlist, t_msh_data *msh_data, int type, size_t i)
{
	t_token	*check_token;

	while (i < (size_t)tokenlist->total)
	{
		check_token = (t_token *)tokenlist->tokens[i];
		if (!check_token)
			return (1);
		if (type == PARAMETER && (check_token->type == PARAMETER
				|| check_token->type == DOUBLE_Q
				|| check_token->type == SINGLE_Q))
		{
			e_printf("Token type = %s\n", enum_to_str(check_token->type));
			if (check_token->type == DOUBLE_Q || check_token->type == SINGLE_Q)
				expand_var(&tokenlist, i, msh_data, check_token, true);
			else
				expand_var(&tokenlist, i, msh_data, check_token, false);
		}
		else if (type == SINGLE_Q && (int)check_token->type == SINGLE_Q)
			rm_quotes(tokenlist, i, '\'');
		else if (type == DOUBLE_Q && (int)check_token->type == DOUBLE_Q)
			rm_quotes(tokenlist, i, '"');
		i++;
	}
	return (0);
}
