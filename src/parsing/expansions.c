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
//ft_safe_free((unsigned char **)&env_value)
int expand_unquoted(element *tokenlist, t_token *check_token, char *name, int pos, char *env_value)
{
    static int flag;
    int i;
    
    i = -1;
    if (flag > 0 || ft_strchr(check_token->value, '$'))
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
    (ft_safe_free((unsigned char **)&check_token));
    return (0);
}

void expand_quoted(element *tokenlist, char *name, size_t pos, char *env_value)
{
    int offset;
    t_token *check_token;

    check_token = tokenlist->element_list.tokens[pos];
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


int count_exp(char* str_token)
{
    int i; 
    int count;

    count = 0;
    i = 0;
    while(str_token[i])
    {
        if (str_token[i] == '$')
            count++;
        i++;
    }
    return (count);
}

//expand known var and otherwise delete and re-position all tokens
int expand_var(element **tokenlist, 
    int pos, 
    t_minishell_data **minishell_data, 
    t_token *check_token, 
    bool quoted)
{
    char *name;
    char *env_value;
    int count;

	name = NULL;
    count = count_exp(check_token->value);
    while (count > 0)
    {
        check_token = (*tokenlist)->element_list.tokens[pos];
        name = refine_name_var(check_token->value, name);
        if (name && ft_strncmp(name, "?", 1))
            printf("%d\n", (*minishell_data)->last_pipeline_return);
        // if (name && ft_isall_upper(name))
        //    return (token_name);
        env_value = env_var_get_value((*minishell_data)->env, name);
        e_printf("\nNAME= %s \n", name);
        if (quoted)
            expand_quoted(*tokenlist, name, pos, env_value);
        else if (!quoted && env_value)
            expand_unquoted(*tokenlist, check_token, name, pos, env_value);
        // else if (!quoted && !env_value)
        // {
        //     (*tokenlist)->pf_element_delete((*tokenlist), pos);
        //     ft_safe_free((unsigned char **)&name);
        //     index_lexer(tokenlist);
        //     return (1);
        // }
        
        (ft_safe_free((unsigned char **)&name), ft_safe_free((unsigned char **)&env_value));
        index_lexer(tokenlist);
        count--;
    }
    //index_lexer(tokenlist);
    return (0);
}
 
// check lexer on expansion and quotes
int	exp_lexer(
    element *tokenlist, 
    t_minishell_data **minishell_data,
    int type, 
    size_t i)
{
	t_token	*check_token;
    
	while (i < (size_t)tokenlist->element_list.total)
	{
		check_token = (t_token *)tokenlist->element_list.tokens[i];
		if (((int)check_token->type == PARAMETER && type == PARAMETER)
			|| ((int)check_token->type == DOUBLE_Q && type == DOUBLE_Q))
		{
			rm_quotes(tokenlist, i, '"');
			if (type == DOUBLE_Q && ft_strchr(check_token->value, '$') != NULL)
			{
				if (expand_var(&tokenlist, i, minishell_data, check_token, true))
					i--;
			}
			else if (type == PARAMETER)
			{
				if (expand_var(&tokenlist, i, minishell_data, check_token, false))
					i--;
			}
		}
		else if ((int)check_token->type == SINGLE_Q && type == SINGLE_Q)
			rm_quotes(tokenlist, i, '\'');
		i++;
	}
	return (0);
}
