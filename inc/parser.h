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

# define TOKEN_INIT(el) element el;\
    element_init(&el)

typedef struct s_elementlist
{
    void **tokens;
    size_t size; //(size in memory)
    size_t total; //(number elements)
} t_elementlist;

typedef struct s_token
{
    int type;
    char *value;
    void *next;
} t_token;

typedef struct s_element element;
struct s_element
{
    t_elementlist elementList;
    int (*pfElementTotal)(element *);
    int (*pfElementResize)(element *, int);
    int (*pfElementAdd)(element *, void *);
    int (*pfElementSet)(element *, int, void *);
    void (*pfElementGet)(element *, int);
    int *(*pfElementDelete)(element *, int);
    int (*pfElementFree)(element *);
};

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

#endif
