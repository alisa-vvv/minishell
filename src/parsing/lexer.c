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

// int is_EOS(char *str, int i)
// {
//     if (i == ft_strlen(str))
//         return (1);
//     return (0);
// }

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

char *trim_string(char *str)
{
    int i;
    int j;
    i = 0;
    j = 0;
    while (str[j])
    {
        if (i == 0 && (skip_blanks(str + j) > 1))
            j += skip_blanks(str + j);
        else if (skip_blanks(str + j) > 1)
            j += skip_blanks(str + j + 1);
        str[i] = str[j];
        i++;
        j++;
    }
    
    str[i] = '\0';
    return (str);
}

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
    tokencount = 0;

    while (*str)
    {
        if (ft_isspace(*str))
            str++;
        else
        {
            tokencount++;
            while (!ft_isspace(*str) && *str)
            {
                str++;
            }
        }
    }
    return (tokencount);
}

//pushes tokens in the elementlist from the back, immediately indexing 
int fill_tokenlist(element **tokenlist, char *str)
{
    size_t len;
    t_token *token;

    while (*str)
    {
        len = 0;
        while (*str && ft_isspace(*str))
            str++;
        while (*str && !ft_isspace(*str))
        {
            len++;
            str++;
        }
        if (len > 0)
        {
            token = new_token(str - len, len + 1);
            if (!token)
                return ((*tokenlist)->pfElementFree(*tokenlist), MALLOC_ERR);
            token->pos = (*tokenlist)->elementList.total;
            (*tokenlist)->pfElementAdd(*tokenlist, token);
        }
    }
    return (0);
}

//default option to put input as token into dyn array elementlist
int default_lexer(char *input_line)
{
    int token_c;
    token_c = 0;
    if (!input_line)
        return (1);
    if (check_d_quote_in(input_line) || check_s_quote_in(input_line, check_quote_c(input_line)))
        return (1);
    token_c = token_count(input_line);
    if (!token_c)
        return (write(1, "Failed to count tokens\n", 23));
    TOKEN_INIT(token_list, token_c);
    fill_tokenlist(&token_list, input_line);
    if (!token_list.elementList.tokens)
        return (write(1, "Failed to init tokenlist\n", 25));
    check_lexer(&token_list);
    
    size_t i = 0;
    t_token *token_test;

    while (i < token_list.elementList.total)
    {
        token_test = (t_token *)token_list.elementList.tokens[i];
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

