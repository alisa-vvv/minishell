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
char *exp_str_token(char *str_token, char *name, char *value)
{
    char *new_str;
    char *start; 
    char *leftover;
    char *temp_left;
    int offset; 

    if (!value || !name)
        value = "";
    start = ft_strchr(str_token, '$');
    offset = ft_strlen(name) + 1;
    temp_left = malloc(sizeof(char) * ft_strlen(start) - offset);
    if (!temp_left)
        return (NULL);
    ft_strlcpy(temp_left, start + offset, ft_strlen(start)- offset + 1);
    leftover = ft_strjoin(value, temp_left);
    start[0] = '\0';
    new_str = ft_strjoin(str_token, leftover);
    if (!new_str || !leftover)
        return ((ft_safe_free((unsigned char **)&leftover), ft_safe_free((unsigned char **)&temp_left), ft_safe_free((unsigned char **)&new_str)), NULL);
    (ft_safe_free((unsigned char **)&leftover), ft_safe_free((unsigned char **)&temp_left), ft_safe_free((unsigned char **)&str_token));
    return (new_str);
}

//get name of env var from token_name
char *refine_name_var(char *token_name, char *result)
{
    char *start;
    int i;

    i = 0;
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
    return (result);
}

//expand known var and otherwise delete and re-position all tokens
int expand_var(element **tokenlist, int pos, t_minishell_data **minishell_data, bool quoted)
{
    t_token *check_token;
    check_token = (t_token *)(*tokenlist)->element_list.tokens[pos];
    char *name;

	name = NULL;
    name = refine_name_var(check_token->value, name);
    if (ft_strncmp(name, "?", 2))
        printf("%d\n", (*minishell_data)->last_pipeline_return);
    e_printf("\nNAME= %s \n", name);
    if (quoted || env_var_get_value((*minishell_data)->env, name))
    {
        e_printf("VALUE= %s \n", env_var_get_value((*minishell_data)->env, name));
        check_token->value = exp_str_token(check_token->value, name, env_var_get_value((*minishell_data)->env, name));
    }
    else if (!quoted && !env_var_get_value((*minishell_data)->env, name))
    {
        (*tokenlist)->pf_element_delete((*tokenlist), pos);
        index_lexer(tokenlist);
        return (1);
    }
    free(name);
    return (0);
}
 
// check lexer on expansion and quotes
int	exp_lexer(element *tokenlist, t_minishell_data **minishell_data, int type)
{
	size_t	i;
	t_token	*check_token;

	i = 0;
	while (i < (size_t)tokenlist->element_list.total)
	{
		check_token = (t_token *)tokenlist->element_list.tokens[i];
		// e_printf("TYPE = %d \n", (int)check_token->type);
		if (((int)check_token->type == PARAMETER && type == PARAMETER)
			|| ((int)check_token->type == DOUBLE_Q && type == DOUBLE_Q))
		{
			rm_quotes(tokenlist, i, '"');
			if (type == DOUBLE_Q && ft_strchr(check_token->value, '$') != NULL)
			{
				if (expand_var(&tokenlist, i, minishell_data, true))
					i--;
			}
			else if (type == PARAMETER)
			{
				if (expand_var(&tokenlist, i, minishell_data, false))
					i--;
			}
		}
		else if ((int)check_token->type == SINGLE_Q && type == SINGLE_Q)
			rm_quotes(tokenlist, i, '\'');
		i++;
	}
	return (0);
}
