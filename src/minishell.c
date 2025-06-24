/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   minishell.c                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/31 15:31:56 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/24 18:09:08 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <unistd.h>

char	**clone_environment(
	int *env_var_count,
	int *env_mem
)
{
	// SET A MAX VALUE FOR ALLOC SIZE
	char		**environment;
	int			i;
	int			len;
	int			alloc_size;

	alloc_size = 128;
	environment = ft_calloc(alloc_size, sizeof(char *));
	if (!environment)
		return (NULL);
	i = -1;
	while(__environ[++i])
	{
		if (i == alloc_size)
		{
			alloc_size += alloc_size;
			free_2d_arr((void **) environment);
			environment = ft_calloc(alloc_size, sizeof(char *));
			if (!environment)
				return (NULL);
			i = 0;
		}
		environment[i] = ft_strdup(__environ[i]);
		if (!environment[i])
		{
			free_2d_arr((void **) environment);
			return (NULL);
		}
	}
	*env_var_count = i;
	*env_mem = alloc_size;
	return (environment);
}

int	main(void)
{
	char				*read_line;
	int					is_open = true;
	t_minishell_data	minishell_data;

	int	TEST_len = 4;
	minishell_data.environment = clone_environment(&minishell_data.env_var_count, &minishell_data.env_mem);
	if (!minishell_data.environment)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (EXIT_FAILURE);
	}
	while (is_open != 0)
	{
		read_line = readline("minishell ");
		if (!read_line)
			printf("what happens?\n");
		else if (strcmp(read_line, "clear") == 0)
			rl_clear_history();
		else if (strcmp(read_line, "exit") == 0)
			is_open = false;
		else if (strcmp(read_line, "executor") == 0)
			executor(test_get_dummy_exec_data(TEST_len), TEST_len, &minishell_data);
		else
			printf("We entered: %s\n", read_line);
		add_history(read_line);
	}
	free_2d_arr((void **) minishell_data.environment);
	return (0);
}
