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
# include <readline/readline.h>
# include <readline/history.h>

// typedef struct sVectorList
// {
//     void **elements;
//     size_t size; (size in memory)
//     size_t count; (number elements)
// } sVectorList;


enum 
{
    COMMAND, 
    STRING, 
    NUMBER,
    FILE,
    DIR,
    BLANK,
    
    SINGLE_Q_OPEN,
    SINGLE_Q_CLOSED,
    DOUBLE_Q_OPEN,
    DOUBLE_Q_CLOSED,

    DOUBLE_DOT,
    DOT,
    DASH,
    PIPE,
    OPEN_BRACKET,
    CLOSED_BRACKET,
    DOLLAR_SIGN,
    FORW_SLASH,
    BACKW_SLASH,
    QUESTION_MARK,
    EXCLAM_MARK
}

typedef struct s_token
{
    int type;
    char *value;
    void *next;
} t_token;



#endif
