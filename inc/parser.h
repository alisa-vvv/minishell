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

# define MALLOC_ERR "Error! malloc()"

typedef enum	e_token_type
/*
* REALLY important to remove token types we don't need.
* there's logic tied to them that is still active, and it messes up certain edge cases
* @alisa
*/
{
	COMMAND,
	STRING,
	NUMBER,
	OPERATOR,
	PARAMETER,
	NON_TERMINAL,

	SINGLE_Q,
	DOUBLE_Q,

	QUESTION_MARK,
	
	PIPE,
	REDIRECT_IN,
	HEREDOC,
	HEREDOC_DEL,
	REDIRECT_OUT,
	REDIRECT_OUT_APP,

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
	char		*value;
	bool		command;
	size_t		pos;
}			t_token;


int 		all_num_alph(const char *str);
int			default_lexer(char *input_line, t_msh_data *msh_data);
bool 		char_is_quote(char c);
bool 		str_is_quote(const char *str, char symbol);
int			token_count(const char *str, int i);
t_token *	new_token(element *tokenlist, char *str, int len);
int			set_len(const char *str, int i);
bool		char_is_red(char c);
bool 		str_contains_red(char *str);
bool		token_is_redirect(t_token *check_token);
char 		symbol_in_quote(char *str, char symbol);
int			check_in_quote_s(const char *str, int pos, char quote);
int 		move_o_unquoted(const char *str, int i);
int			move_over_quote(const char *str, int pos);
int			fill_tokenlist(element *tokenlist, char *str);
const char* enum_to_str(t_token_type symbols);
int 		clean_lexer(element *tokenlist, size_t i);
int			merge_tokens(element *tokenlist, int pos1, int pos2);
int 		contract_list(element *tokenlist, int i);
int			check_lexer(element *tokenlist, t_msh_data *msh_data);
int			fill_comm_list(t_exec_data *exec_data, element *tokenlist, size_t pos, int pos_red);
int			index_lexer(element **tokenlist);
t_token		*lookahead(element *tokenlist, size_t index);
t_token		*lookbehind(element *tokenlist, size_t index);
int			exp_lexer(element *tokenlist, t_msh_data *msh_data,
				int type, size_t i);
int			skip_blanks(char *str, int pos);
char		*trim_str_space(char *str);
int			match_token(char *str_token);
int			check_pipe_redirect(char *str, char symbol);
int			check_in_quote(const char *str, int pos);
int			single_token(element *tokenlist);
int			val_inputline(char *str);
int			val_redir(element *tokenlist, size_t i);
int			find_symbol(element *tokenlist, int pos, char symbol);
int			match_nonterminal(char *str_token);
int			match_string(char *str_token);
int			expand_var(element **tokenlist, int pos, t_msh_data *msh_data, t_token *check_token, bool quoted);
int			rm_quotes(element *tokenlist, int pos, char symbol);
int			add_arg_to_list(t_exec_data *comm_list, element *tokenlist, int *i, size_t pos, int pos_red);
int			add_redirect(t_exec_data* execdata, element *tokenlist, int pos, int pos_red);
int			set_heredoc(t_exec_data* execdata, element *tokenlist, int pos, int pos_red);
void		set_pipe_cm(element *tokenlist, size_t i);
void 		set_command(t_exec_data *comm_list, element *tokenlist, int pos, t_token *check_token, int *i);
int			set_exec_def(t_exec_data *execdata, element *tokenlist, size_t pos);
int 		count_symbols(char* str_token, char symbol);
int 		count_args(element *tokenlist, int pos, int total);
int			count_lists(element *tokenlist);
int			count_next_cm(element *tokenlist, int pos);
char *		prep_leftover(char *str_token, char symbol,  int offset);
char *		exp_str_token(char *str_token, char *value, int offset);
char *		refine_name_var(char *token_name, char *result, char symbol);
t_builtin_name set_builtins(t_token_type tokentype);
int			find_token_type(element *tokenlist, size_t pos, int pos_red, t_token_type type);
int			fill_comm_list(t_exec_data *execdata, element *tokenlist, size_t pos, int pos_red);
int			make_cm_list(element *tokenlist, t_exec_data *comm_list, size_t pos, int pos_red);
int			pass_comm(element *tokenlist, t_msh_data *msh_data, int i, int pos);
int			convert_data(element *tokenlist, t_msh_data *msh_data, int i, size_t pos, int pos_red);

#endif
