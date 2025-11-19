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

//var expansion: $sign, $@sign 
//path expansion
//--> make expansion first, remove quotes later
// expand $EMPTY to nothing
// look for return value in minishell struct when accessing $?


int lpos_in_str(const char *str, char symbol)
{
    int i;
    i = -1;
    i = ft_strlen(str)-1;
    while (i > 0)
    {
        if (str[i] == symbol)
            return (i);
        i--;
    }
    return (i);
}


//expands unquoted var to the relevant environment value, keeping the rest of the string intact or replacing empty strings if env value is not present
int expand_unquoted(element *tokenlist, t_token *check_token, char *name, int pos, char *env_value)
{
    static int flag;
    int i;
    
    i = -1;
    if (!env_value)
        env_value = "";
    if (flag >= 0 || ft_strchr(check_token->value, '$'))
    {
        char *new_str;
        new_str = exp_str_token(check_token->value, env_value, ft_strlen(name) +1);
        tokenlist->pf_element_set(tokenlist, pos, new_token(new_str, ft_strlen(new_str) + 1));
        (ft_safe_free((unsigned char **)&new_str));
    }
    else 
    {
        tokenlist->pf_element_set(tokenlist, pos, new_token(env_value, ft_strlen(env_value)+ 1));
       // ft_safe_free((unsigned char **)&env_value);
    }
    check_token = tokenlist->element_list.tokens[pos];
    if (!check_token)
        tokenlist->pf_element_delete(tokenlist, pos);
    (ft_safe_free((unsigned char **)&check_token));
    return (0);
}

void expand_quoted(element *tokenlist, char *name, size_t pos, char *env_value)
{
    int offset;
    t_token *check_token;

    int start;
    check_token = tokenlist->element_list.tokens[pos];
    start = lpos_in_str(check_token->value, '$');
    if (!check_in_quote(check_token->value, start))
    {
        offset = 0;
        if (!env_value || !name)
            env_value = "";
        offset = ft_strlen(name) + 1;
        char *new_str;
        new_str = exp_str_token(check_token->value, env_value, ft_strlen(name) +1);
        tokenlist->pf_element_set(tokenlist, pos, new_token(new_str, ft_strlen(new_str) + 1));
        (ft_safe_free((unsigned char **)&new_str), ft_safe_free((unsigned char **)&check_token));
        // check_token->value = exp_str_token(check_token->value, env_value, offset);
    }
}


int count_symbols(char* str_token, char symbol)
{
    int i; 
    int count;

    count = 0;
    i = 0;
    while(str_token[i])
    {
        if (str_token[i] == symbol)
            count++;
        i++;
    }
    return (count);
}

//expand known var and otherwise delete and re-position all tokens
int expand_var(element **tokenlist, 
    int pos, 
    t_minishell_data *minishell_data, 
    t_token *check_token, 
    bool quoted)
{
    char *name;
    char *env_value;
    int count;

	name = NULL;
    count = count_symbols(check_token->value, '$');
    while (count > 0)
    {
        check_token = (*tokenlist)->element_list.tokens[pos];
        name = refine_name_var(check_token->value, name);
        if (name && ft_strncmp(name, "?", 2))
            printf("%d\n", minishell_data->last_pipeline_return);
        env_value = env_var_get_value(minishell_data->env, name);
        e_printf("\nNAME= %s \n", name);
        if (quoted)
            expand_quoted(*tokenlist, name, pos, env_value);
        else if (!quoted)
            expand_unquoted(*tokenlist, check_token, name, pos, env_value);
        (ft_safe_free((unsigned char **)&name), ft_safe_free((unsigned char **)&env_value));
        index_lexer(tokenlist);
        count--;
    }
    return (0);
}
 
// check lexer on expansion and quotes
int	exp_lexer(
    element *tokenlist, 
    t_minishell_data *minishell_data,
    int type, 
    size_t i)
{
	t_token	*check_token;
    
	while (i < (size_t)tokenlist->element_list.total)
	{
		check_token = (t_token *)tokenlist->element_list.tokens[i];
        if (!check_token)
            return (1);
		if ((int)check_token->type == SINGLE_Q && type == SINGLE_Q)
            rm_quotes(tokenlist, i, '\'');
        if ((int)check_token->type == DOUBLE_Q && type == DOUBLE_Q)
			rm_quotes(tokenlist, i, '"');
        if (type == PARAMETER && (check_token->type == PARAMETER || check_token->type == DOUBLE_Q || check_token->type == SINGLE_Q))
        {
            if (check_token->type == DOUBLE_Q || check_token->type == SINGLE_Q)
                expand_var(&tokenlist, i, minishell_data, check_token, true);
            expand_var(&tokenlist, i, minishell_data, check_token, false);
        }
        else if (i > 0 && lookbehind(tokenlist, i) && lookbehind(tokenlist, i)->type == OPERATOR)
            merge_tokens(tokenlist, i -1, i);
		i++;
	}
	return (0);
}
