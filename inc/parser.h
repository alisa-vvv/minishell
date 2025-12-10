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

// make enums lowercase
// check the exec for enums in struct (build-in --> to pass which enum and biuld-in)

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

typedef enum	e_token_type
{
	COMMAND,
	STRING,
	//NUMBER,
	OPERATOR,
	PARAMETER,
	WHITESPACE,
	NON_TERMINAL,

	SINGLE_Q,
	DOUBLE_Q,

	PIPE,
	RED_IN,
	HEREDOC,
	HEREDOC_DEL,
	RED_OUT,
	RED_APP,

	CD,
	ECHO,
	ENV,
	EXIT,
	EXPORT,
	PWD,
	UNSET,

} t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	bool			command;
	size_t			pos;
}			t_token;

typedef struct s_exp_data
{
	char 	*name;
	char	*env_value;
	char 	*start_var;
	int 	start_pos;
}	t_exp_data;

typedef struct s_pos
{
	size_t i;
	int pos; 
	int red; 
}	t_pos;

typedef struct s_quote
{
	char quote;
	char o_quote;
	int count; 
	int o_count; 
}	t_quote;



int 		all_num_alph(const char *str);
int			default_lexer(char *input_line, t_msh_data *msh_data);
bool 		char_is_quote(char c);
bool 		str_is_quote(const char *str, char symbol);
int			token_count(const char *str, int i, int tokencount);
t_token *	new_token(t_tokenlist *tokenlist, char *str, int len);
int	prep_token(t_tokenlist *tokenlist, char *str, int i, int len);
int			set_len(const char *str, int i);
bool		char_is_red(char c);
bool 		str_contains_red(char *str);
bool		tok_is_red(t_token *check_token);
char 		symbol_in_quote(char *str, char symbol);
int			validate_redirect(const char *str, int i);
int			check_in_quote_s(const char *str, int pos, char quote);
int 		move_o_unquoted(const char *str, int i);
int			move_over_quote(const char *str, int pos);
int			fill_tokenlist(t_tokenlist *tokenlist, char *str);
const char* enum_to_str(t_token_type symbols);
int 		clean_lexer(t_tokenlist *tokenlist, size_t i);
int			merge_tokens(t_tokenlist *tokenlist, int pos1, int pos2);
int 		contract_list(t_tokenlist *tokenlist, int i);
int			check_lexer(t_tokenlist *tokenlist, t_msh_data *msh_data);
int			fill_comm_list(t_exec_data *exec_data, t_tokenlist *tokenlist, t_pos *xpos);
int 		prep_execdata(t_tokenlist *tlist, t_msh_data *msh_data);
int			index_lexer(t_tokenlist **tokenlist);
t_token		*looknxt(t_tokenlist *tokenlist, size_t index);
t_token		*lookbehind(t_tokenlist *tokenlist, size_t index);
int			exp_lexer(t_tokenlist *tokenlist, t_msh_data *msh_data,
				int type, size_t i);
int			skip_blanks(char *str, int pos);
char		*trim_str_space(char *str);
int			match_token(char *str_token);
int			check_pipe_redirect(char *str, char symbol);
int			check_in_quote(const char *str, int pos);
int			single_token(t_tokenlist *tokenlist);
int 		check_spcases(t_tokenlist *tokenlist);
int			val_inputline(char *str);
int			val_redir(t_tokenlist *tokenlist, size_t i, t_token *ctok);
int			find_symbol(t_tokenlist *tokenlist, int pos, char symbol);
int			match_nonterminal(char *str_token);
int			match_string(char *str_token);
int			expand_var(t_tokenlist **tokenlist, int pos, t_msh_data *msh_data, t_token *check_token);
int			rm_quotes(t_tokenlist *tokenlist, int pos, char symbol);
int			add_arg_to_list(t_exec_data *comm_list, t_tokenlist *tokenlist, t_pos* xpos, int *i);
int			add_redirect(t_exec_data* execdata, t_tokenlist *tokenlist, t_pos *xpos);
int			set_heredoc(t_exec_data* execdata, t_tokenlist *tokenlist, t_pos *xpos);
void		set_pipe_cm(t_tokenlist *tokenlist, size_t i);
void 		set_command(t_exec_data *comm_list, t_tokenlist *tokenlist, t_pos *xpos, int *i);
int			set_exec_def(t_exec_data *execdata, t_tokenlist *tokenlist, size_t pos);
int			token_is_cm(t_tokenlist *tokenlist, int pos, t_token *c_token);
int 		count_occ(const char* str, char symbol, bool inside);
int 		count_args(t_tokenlist *tokenlist, int pos, int total);
int			count_lists(t_tokenlist *tokenlist);
int			count_next_cm(t_tokenlist *tokenlist, int pos);
char *		prep_leftover(char *str_token, char *start,  int offset);
char *		exp_str_token(char *str_token, char *start, char *value, int offset);
int			refine_name(char *token_name, char **result, char symbol);
t_builtin_name set_builtins(t_token *check_token);
int			find_type(t_tokenlist *tokenlist, size_t pos, int red, t_token_type type);
int			fill_comm_list(t_exec_data *execdata, t_tokenlist *tokenlist, t_pos *xpos);
int			make_cm_list(t_tokenlist *tokenlist, t_exec_data *comm_list, t_pos *xpos);
int			pass_comm(t_tokenlist *tokenlist, t_msh_data *msh_data, t_pos *xpos);
int			convert_data(t_tokenlist *tokenlist, t_msh_data *msh_data, t_pos *epos);

#endif
