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
			if (!check_in_quote_s(str, i, '\''))
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


//&& !check_in_quote_s(str, pos, '\'')
bool	check_quote_e(char *str, int pos)
{
	int	quote;

	quote = symbol_in_quote(str, '$');
	if (quote == '"' && !check_in_quote_s(str, pos, '\''))
		return (true);
	else if (quote == '\'' && check_in_quote_s(str, pos, '"'))
		return (true);
	else if (quote == '\'' && !check_in_quote_s(str, pos, '\''))
		return (true);
	else
		return (false);
}


void expand_new(t_tokenlist *tokenlist, size_t pos, char *str_token, char *start, char *name,
		char *env_value)
{
	int offset; 
	char *new_str;
	t_token *n_token;

	n_token = NULL;
	if (env_value && name) 
		offset = ft_strlen(name)+1;
	else if (!env_value && !name)
	{
		env_value = "";
		offset = 1;
	}
	else if (!env_value && name)
	{
		env_value = "";
		offset = ft_strlen(name)+1;
	}
	else 
		offset = 1;
	new_str = exp_str_token(str_token, start, env_value, offset);
	n_token = new_token(tokenlist, new_str, ft_strlen(new_str)+1);
	if (!n_token)
		tokenlist_free(tokenlist);
	tokenlist_set(tokenlist, pos, n_token);
	(ft_safe_free((unsigned char **)&new_str));
}



// expand known var and otherwise delete and re-position all tokens
int	expand_var(t_tokenlist **tokenlist, int pos, t_msh_data *msh_data,
		t_token *check_token)
{
	char	*name;
	char	*env_value;
	char 	*start_var;
	int 	start_pos;

	name = NULL;
	env_value = NULL;
	start_var = ft_strchr(check_token->value, '$');
	while (start_var)
	{
		check_token = (*tokenlist)->tokens[pos];
		start_pos = start_var - check_token->value;
		name = refine_name_var(start_var, name, '$');
		if (name && (ft_strncmp(name, "?", 2) == 0))
			env_value = ft_itoa(msh_data->last_pipeline_return);
		else if (!env_value && ft_strncmp(check_token->value, "$", 2) == 0)
			return (ft_safe_free((unsigned char **)&name), tokenlist_delete(*tokenlist, pos), 0);
		else if (env_var_get_value(msh_data->env, name, &env_value) != success)
				dprintf(STDERR_FILENO, "Failed to malloc env\n");
		p_printf("\nNAME= %s \n", name);
		p_printf("ENV VAL= ==%s==\n", env_value);
		p_printf("START VAR = -%s-\n", start_var);
		if (check_in_quote_s(check_token->value, start_pos, '\''))
			start_var = ft_strchr(start_var +1, '$');
		else 
		{	
			p_printf("TOKEN VAL= %s\n", check_token->value);
			expand_new(*tokenlist, pos, check_token->value, start_var, name, env_value);
			(ft_safe_free((unsigned char **)&name), ft_safe_free((unsigned char **)&env_value));
			check_token = (*tokenlist)->tokens[pos];
			start_var = ft_strchr(check_token->value, '$');
		}
	}
	return (0);
}

//$"adecheri and '$USER' or $HOME "$

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
			expand_var(&tokenlist, i, msh_data, check_token);
		}
		else if (type == SINGLE_Q && (int)check_token->type == SINGLE_Q)
			rm_quotes(tokenlist, i, '\'');
		else if (type == DOUBLE_Q && (int)check_token->type == DOUBLE_Q)
			rm_quotes(tokenlist, i, '"');
		i++;
	}
	return (0);
}
