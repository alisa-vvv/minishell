/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   lexer.c                                             :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/03 18:53:46 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/03 18:53:56 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int is_EOS(char *str, int i)
{
    if (i == ft_strlen(str))
        return (1);
    return (0);
}

int skip_blanks(char *str)
{
    int count;
    count = 0;
    while (ft_isspace(*str))
    {
        count++;
        str++;
    }
    return (count);
}

// trims input to have one space between tokens to ease validation
// unless the input is quoted
char *trim_str_space(char *str)
{
    int i;
    int j;
    i = 0;
    j = 0;
    while (str[j])
    {
        if (!check_in_quote(str, j) && ((i == 0 && skip_blanks(str + j) > 1) || is_EOS(str, j + skip_blanks(str + j))))
            j += skip_blanks(str + j);
        else if (!check_in_quote(str, j) && (skip_blanks(str + j) > 1))
            j += skip_blanks(str + j + 1);
        else if (!check_in_quote(str, j) && ft_isspace(str[j]))
            j += 1;
        str[i] = str[j];
        i++;
        j++;
    }
    str[i] = '\0';
    return (str);
}

//make a new token in the array
t_token *new_token(char *str, int len)
{
    if (!*str)
        return (NULL);
    t_token *token;
    token = malloc(1 * sizeof(t_token));
    if (!token)
        return (MALLOC_ERR);
    token->value = malloc((len + 1) * sizeof(char));
    token->value = ft_strlcpy(token->value, str, len);
    if (!token->value)
        return (MALLOC_ERR);
    token->type = match(token->value);
    return (token);
}

//counts args to size up elementlist
int token_count(char *str)
{
    int tokencount;
    int i;

    i = 0;
    tokencount = 0;

    while (str[i])
    {
        if (!check_in_quote(str, i) && ft_isspace(str[i]))
            i++;
        else
        {
            tokencount++;
            while (check_in_quote(str, i) || (!check_in_quote(str, i) && !ft_isspace(str[i]) && str[i]))
                i++;
        }
    }
    return (tokencount);
}

//pushes tokens in the elementlist from the back, immediately indexing 
int fill_tokenlist(element **tokenlist, char *str)
{
    int i; 
    size_t len;
    t_token *token;

    i = 0;
    while (str[i])
    {
        len = 0;
        while (!check_in_quote(str, i) && str[i] && ft_isspace(str[i]))
            i++;
        while (check_in_quote(str, i) || (str[i] && !ft_isspace(str[i])))
        {
            len++;
            i++;
        }
        if (len > 0)
        {
            token = new_token(str + i - len, len + 1);
            if (!token)
                return ((*tokenlist)->pf_element_free(*tokenlist), MALLOC_ERR);
            token->pos = (*tokenlist)->element_list.total;
            (*tokenlist)->pf_element_add(*tokenlist, token);
        }
    }
    return (0);
}

//default option to put trimmed input in tokenlist
int default_lexer(char *input_line)
{
    int token_c;
    token_c = 0;
    if (!input_line)
        return (1);
    input_line = trim_str_space(input_line);
    val_inputline(input_line);
    
    token_c = token_count(input_line);
    if (!token_c)
        return (write(1, "Failed to count tokens\n", 23));
    TOKEN_INIT(token_list, token_c);
    fill_tokenlist(&token_list, input_line);
    if (!token_list.element_list.tokens)
        return (write(1, "Failed to init tokenlist\n", 25));

    size_t i = 0;
    t_token *token_test;

    while (i < token_list.element_list.total)
    {
        token_test = (t_token *)token_list.element_list.tokens[i];
        printf("%s\n", token_test->value);
        i++;
    }
    return (0);
}

//set a value to the token so we can expand on those later
int match_token(char *str_token)
{
    if (str_token[0] == 34)
        return DOUBLE_Q_OPEN;
    else if (str_token[0] == 39)
        return (SINGLE_Q_OPEN);
    else if (str_token[0] == '(' || str_token[0] == '{')
        return (OPEN_BRACKET);
    else if (str_token[0] == '-')
        return (HYPHEN);
    else if ((str_token[0] == '>') || (str_token[0] == '<') || (str_token[0] == '$') || (str_token[0] == '|') || (str_token[0] == '.') || (str_token[0] == '!'))
        return (NON_TERMINAL);
    else if (ft_isdigit(str_token[0]))
        return (NUMBER);
    else if (ft_isalpha(str_token[0]))
        return (STRING);
    else 
        return (UNKNOWN);
}

