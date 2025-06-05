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

void push_token(t_token *token, t_token **tlist)
{
    t_token *last;
    if (!tlist)
    {
        *tlist = token;
        return ;
    }
    while ((*tlist)->next)
        last = (*tlist)->next;
    last = token; 
}

t_token *new_token(char *str)
{
    t_token *token;
    token = ft_calloc(1, sizeof(t_token));
    if (!token)
        return (MALLOC_ERR);
    token->value = ft_strdup(str);
    token->type = match(str);
    return (token);
}

int default_lexer(char *str)
{
    int i;
    int j;
    i = 0;
    j = 0;
    //intoken = 0; 
    char *input_line; 
    input_line = readline("minishell");
    if (!input_line)
        return (1);
    while (!is_EOS(input_line, i))
    {
        while (ft_isspace(input_line[i]))
            i++;
        while (ft_isprint(input_line[i]) && !ft_isspace(input_line))
        {
            j++;
        }
        
        
    }
    return (0);
}

int match_token(char *str_read)
{
    int i;
    i = 0; 
    if (str_read[i] == '"')
        return DOUBLE_Q_OPEN;
    else if (str_read[i] == '"')

}


// t_token *regular_expression(char *str)
// {
//     int i;
//     i = 0; 
//     while(isspace(str[i]))
//         i++;
//     if 
// }