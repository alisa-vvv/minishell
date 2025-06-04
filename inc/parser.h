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

enum 
{
    COMMAND, 
    STRING, 
    NUMBER,
    FILE,
    DIR,
    
    SINGLE_Q_OPEN,
    SINGLE_Q_CLOSED,
    DOUBLE_Q_OPEN,
    DOUBLE_Q_CLOSED,

    DASH,
    PIPE,
    OPEN_BRACKET,
    CLOSED_BRACKET,
    DOLLAR_SIGN,
    FORW_SLASH,
    BACKW_SLASH

}

typedef struct s_token
{
    int type;
    char *str;
} t_token;

#endif
