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

# define MALLOC_ERR "Error! malloc()"
# define TOKEN_INIT(el, size) element el;\
    element_init(&el, size)

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
} t_token;

typedef struct s_element element;

struct s_element
{
    t_elementlist elementList;
    int (*pfElementTotal)(element *);
    int (*pfElementResize)(element *, int);
    int (*pfElementAdd)(element *, void *);
    int (*pfElementSet)(element *, int, void *);
    void *(*pfElementGet)(element *, int);
    int (*pfElementDelete)(element *, int);
    int (*pfElementFree)(element *);
};

enum 
{
    COMMAND, 
    STRING, 
    NUMBER,
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
};

int         default_lexer(char *input_line);
int         token_count(char *str);
t_token *   new_token(char *str, int len);
int         fill_tokenlist(element *tokenlist, char *str);
void	    ft_safefree(void *ptr);
void	    ft_free_arr(void **array);
size_t	    ft_strlcpy(char *dst, const char *src, size_t size);


#endif
