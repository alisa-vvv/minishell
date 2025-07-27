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

char *exp_str_token(char *str_token, char *name, char *value)
{
    char *new_str;
    char *start; 
    char *leftover;

    start = NULL;
    leftover = NULL;
    if (!value)
        value = "";
    start = ft_strchr(str_token, "$");
    *leftover = start + ft_strlen(name);
    leftover = ft_strjoin(value, leftover);
    start[0] = '\0';
    new_str = ft_strjoin(str_token, leftover);

    return (new_str);
}


//get name of env var
char *refine_name_var(char *token_name, char *result)
{
    char *start;
    int i;

    i = 0;
    start = ft_strchr(token_name, '$');
    // e_printf("\nSTART = %s\n", start);
    result = ft_strdup(start + 1);
   // e_printf("\nRESULT = %s$\n", result);
    while (result[i])
    {
        if (result[i] == '"' || result[i] == 32 || (result[i] >= 9 && result[i] <= 13))
            break;
        i++;
    }
    result[i] = '\0';
    //free(start);
    return (result);
}

//expand known var and otherwise delete and re-position all tokens
int expand_var(element **tokenlist, int pos, t_minishell_data **minishell_data, bool quoted)
{
    t_token *check_token;
    check_token = (t_token *)(*tokenlist)->element_list.tokens[pos];
    char *name;

    name = refine_name_var(check_token->value, name);
    e_printf("\nNAME= %s \n", name);
    if (env_var_get_value((*minishell_data)->env, name))
    {
        e_printf("VALUE= %s \n", env_var_get_value((*minishell_data)->env, name));
        check_token->value = exp_str_token(check_token->value, name, env_var_get_value((*minishell_data)->env, name));
    }
    else 
    {
        (*tokenlist)->pf_element_delete((*tokenlist), pos);
        index_lexer(tokenlist);
        return (1);
    }
    free(name);
    return (0);
}
 