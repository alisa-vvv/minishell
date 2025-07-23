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

# include "debug.h"
# include "libft.h"
# include "minishell.h"
# include "minishell_env.h"
# include "vec_lib.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>

# define MALLOC_ERR "Error! malloc()"

typedef struct s_token
{
	int		type;
	char	*value;
	size_t	pos;

}			t_token;

enum
{
	COMMAND,
	STRING,
	NUMBER,
	OPERATOR,
	PARAMETER,
	FILENAME,
	NON_TERMINAL,

	SINGLE_Q,
	DOUBLE_Q,
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

	FORW_SLASH,
	BACKW_SLASH,
	QUESTION_MARK,
	EXCLAM_MARK,

	ECHO,
	CAT,
	CD,
	ENV,
	PWD,
	UNSET,
	EXPORT,

	UNKNOWN

};

int			default_lexer(char *input_line, t_minishell_data *minishell_data);
int			token_count(char *str);
t_token		*new_token(char *str, int len);
int			fill_tokenlist(element *tokenlist, char *str);
void		ft_safefree(void *ptr);
char		*ft_strchr(const char *s, int c);
void		ft_free_arr(void **array);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			index_lexer(element **tokenlist);
int			check_lexer(element *tokenlist, t_minishell_data **minishell_data, int type);
int			skip_blanks(char *str, int pos);
char		*trim_str_space(char *str);
int			match_token(char *str_token);
int			check_pipe(char *str);
int			check_in_quote(char *str, int pos);
int			val_inputline(char *str);
int			find_symbol(element *tokenlist, int pos, char symbol);
int			count_symbol(element *tokenlist, int pos, char symbol);
int			match_nonterminal(char *str_token);
int			match_string(char *str_token);
int			expand_var(element **tokenlist, int pos,
				t_minishell_data **minishell_data, bool quoted);
int			rm_quotes(element *tokenlist, int pos, char symbol);

#endif
