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

//expand the var when token is in quotes 
char *exp_str_token(char *str_token, char *value, int offset)
{
    char *new_str;
    char *start; 
    char *leftover;
    char *temp_left;

    // start = NULL;
    // leftover = NULL;
    // new_str = NULL;
    start = ft_strchr(str_token, '$');
    temp_left = malloc(sizeof(char) * ft_strlen(start) - offset);
    if (!temp_left)
        return (NULL);
    ft_strlcpy(temp_left, start + offset, ft_strlen(start)- offset + 1);
    leftover = ft_strjoin(value, temp_left);
    ft_safe_free((unsigned char **)&temp_left);
    start[0] = '\0';
    new_str = ft_strjoin(str_token, leftover);
    if (!new_str || !leftover)
        return ((ft_safe_free((unsigned char **)&leftover), ft_safe_free((unsigned char **)&value), ft_safe_free((unsigned char **)&new_str)), NULL);
    (ft_safe_free((unsigned char **)&leftover), ft_safe_free((unsigned char **)&value), ft_safe_free((unsigned char **)&str_token));
    return (new_str);
}

//get name of env var from token_name
char *refine_name_var(char *token_name, char *result, bool quoted)
{
    char *start;
    int i;

    i = 0;
    start = NULL;
    start = ft_strchr(token_name, '$');
    // e_printf("\nSTART = %s\n", start);
    result = ft_strdup(start + 1);
    if (!result)
        return(NULL);
   // e_printf("\nRESULT = %s$\n", result);
    while (result[i])
    {
        if (result[i] == '"' || result[i] == ' ' || (result[i] >= '\t' && result[i] <= '\r'))
            break;
        i++;
    }
    result[i] = '\0';
    if (!quoted)
        ft_safe_free((unsigned char **)&start);
    return (result);
}


void expand_unquoted(element *tokenlist, t_token *check_token, int pos, char *env_value)
{
    tokenlist->pf_element_set(tokenlist, pos, new_token(env_value, ft_strlen(env_value)+ 1));
    ft_safe_free((unsigned char **)&check_token);
    ft_safe_free((unsigned char **)&env_value);
    index_lexer(&tokenlist);
}

void expand_quoted(t_token *check_token, char *name, char *env_value)
{
    int offset; 

    if (!env_value || !name)
        env_value = "";
    offset = ft_strlen(name) + 1;
    check_token->value = exp_str_token(check_token->value, env_value, offset);
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

	name = NULL;
    name = refine_name_var(check_token->value, name, quoted); 
    if (ft_strncmp(name, "?", 2))
        printf("%d\n", (*minishell_data)->last_pipeline_return);
    env_value = env_var_get_value((*minishell_data)->env, name);
    e_printf("\nNAME= %s \n", name);
    if (quoted)
        expand_quoted(check_token, name, env_value);
    else if (!quoted && env_value)
        expand_unquoted(*tokenlist, check_token, pos, env_value);
    else if (!quoted && !env_value)
    {
        (*tokenlist)->pf_element_delete((*tokenlist), pos);
        ft_safe_free((unsigned char **)&name);
        index_lexer(tokenlist);
        return (1);
    }
    (ft_safe_free((unsigned char **)&name));
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
