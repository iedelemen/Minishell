

#include "../../inc/libft.h"
#include "../../inc/minishell.h"
#include <stdio.h>

int	is_echo(char *str1, char *str2)
{
	int	i;

	i = 0;
	if (!str1 || !str2)
		return (1);
	i = 0;
	while (ft_tolower(str1[i]) && str2[i] && str1[i] == str2[i])
		i++;
	return (str1[i] - str2[i]);
}

int	check_flags(t_token *node)
{
	int	i;

	i = 1;
	if ((node->type == WORD && !node->value[0]))
		return (2);
	if (!node->value[i])
		return (0);
	while (node->value[i])
	{
		if (node->value[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	cmd_echo(t_token *node)
{
	int	check;

	check = 0;
	while (node && ((is_args(node) && *node->value == '-')
			|| (node->type == WORD && !node->value[0])))
	{
		if (!check_flags(node))
			break ;
		check = check_flags(node);
		node = node->next;
	}
	while (node)
	{
		if (is_args(node))
			printf("%s", node->value);
		else
			return ;
		if (node->next && (is_args(node->next)))
			printf(" ");
		node = node->next;
	}
	if (check != 1)
		printf("\n");
	*get_exit_status() = 0;
}
