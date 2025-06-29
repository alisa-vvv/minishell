/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser.h                                            :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/04 13:25:59 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/04 13:26:01 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "vec_lib.h"

# define MALLOC_ERR "Error! malloc()"

typedef struct s_token
{
    int type;
    char *value;
    size_t pos;
    
} t_token;

enum 
{
    COMMAND, 
    STRING, 
    NUMBER,
    VARIABLE,
    PARAMETER,
    FILENAME,
    UNKNOWN,
    NON_TERMINAL,
    
    SINGLE_Q_OPEN,
    SINGLE_Q_CLOSED,
    DOUBLE_Q_OPEN,
    DOUBLE_Q_CLOSED,
    OPEN_BRACKET,
    CLOSED_BRACKET,

    DOUBLE_DOT,
    DOT,
    HYPHEN,
    PIPE,
    REDIRECT_OUT,        // match_string(str_token);
    REDIRECT_OUT_APP,
    REDIRECT_IN,
    HEREDOC, 
 
    DOLLAR_SIGN,
    FORW_SLASH,
    QUESTION_MARK,
    EXCLAM_MARK,

    ECHO,      
    CAT,
    CD,
    ENV,
    PWD,
    UNSET,
    EXPORT

};



int         default_lexer(char *input_line);
int         token_count(char *str);
t_token *   new_token(char *str, int len);
int         fill_tokenlist(element **tokenlist, char *str);
void	    ft_safefree(void *ptr);
void	    ft_free_arr(void **array);
size_t	    ft_strlcpy(char *dst, const char *src, size_t size);


#endif
