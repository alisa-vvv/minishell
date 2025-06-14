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
    size_t size; // in mem
    size_t total;
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
    REDIRECT_OUT,
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

int elementTotal(element *e);
int elementResize(element *e, int size);
int elementPushBack(element *e, void *token);
int elementSet(element *e, int index, void *token);
void *elementGet(element *e, int index);
int elementDelete(element *e, int index);
int elementFree(element *e);
void element_init(element *e, int size);

int         default_lexer(char *input_line);
int         token_count(char *str);
t_token *   new_token(char *str, int len);
int         fill_tokenlist(element **tokenlist, char *str);
void	    ft_safefree(void *ptr);
void	    ft_free_arr(void **array);
size_t	    ft_strlcpy(char *dst, const char *src, size_t size);


#endif
