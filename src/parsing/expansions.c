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
// look for return value in msh struct when accessing $?


int lpos_in_str(const char *str, char symbol)
{
    int i;
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
int expand_unquoted(t_tokenlist *tokenlist, t_token *check_token, char *name, int pos, char *env_value)
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
        tokenlist_set(tokenlist, pos, new_token(tokenlist, new_str, ft_strlen(new_str) + 1));
        (ft_safe_free((unsigned char **)&new_str));
    }
    else 
    {
        tokenlist_set(tokenlist, pos, new_token(tokenlist, env_value, ft_strlen(env_value)+ 1));
       // ft_safe_free((unsigned char **)&env_value);
    }
    check_token = tokenlist->tokens[pos];
    if (!check_token)
        tokenlist_delete(tokenlist, pos);
    (ft_safe_free((unsigned char **)&check_token));
    return (0);
}

void expand_quoted(t_tokenlist *tokenlist, char *name, size_t pos, char *env_value)
{
    int offset;
    t_token *check_token;

    char quote;
    int start;
    check_token = tokenlist->tokens[pos];
    start = lpos_in_str(check_token->value, '$');
    quote = symbol_in_quote(check_token->value, '$');
    p_printf("QUOTE TYPE =%c\n", quote);
    if ((quote == '"' && !check_in_quote_s(check_token->value, start, '\'')) || (check_in_quote_s(check_token->value, start, '"') && !check_in_quote_s(check_token->value, start, '\'')))
    {
        offset = 0;
        if (!env_value || !name)
            env_value = "";
        offset = ft_strlen(name)+1;
        char *new_str;
        t_token *n_token; 
        n_token = NULL;
        new_str = exp_str_token(check_token->value, env_value, offset);
        n_token = new_token(tokenlist, new_str, ft_strlen(new_str)+1);
        if (!n_token)
            tokenlist_free(tokenlist);
        tokenlist_set(tokenlist, pos, n_token);
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
int expand_var(t_tokenlist **tokenlist, 
    int pos, 
    t_msh_data *msh_data, 
    t_token *check_token, 
    bool quoted)
{
    char *name;
    char *env_value;
    int count;

	name = NULL;
	env_value = NULL;
    count = count_symbols(check_token->value, '$');
    while (count > 0)
    {
        check_token = (*tokenlist)->tokens[pos];
        name = refine_name_var(check_token->value, name, '$');
        if (name && ft_strncmp(name, "?", 2))
            return(printf("%d\n", msh_data->last_pipeline_return));
        if (env_var_get_value(msh_data->env, name, &env_value) != success)
			dprintf(STDERR_FILENO, "Failed to malloc env\n");
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
 
// check_in_quote(check_token->value, lpos_in_str(check_token->value, '$')
// check lexer on expansion and quotes
int	exp_lexer(
    t_tokenlist *tokenlist, 
    t_msh_data *msh_data,
    int type, 
    size_t i)
{
	t_token	*check_token;
	while (i < (size_t)tokenlist->total)
	{
		check_token = (t_token *)tokenlist->tokens[i];
        if (!check_token)
            return (1);
        if (type == PARAMETER && (check_token->type == PARAMETER || check_token->type == DOUBLE_Q || check_token->type == SINGLE_Q))
        {
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
