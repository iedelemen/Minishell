

#include "../../inc/libft.h"
#include "../../inc/minishell.h"

int	check_pipes(t_token *node)
{
	while (node)
	{
		if (node->type == PIPE)
		{
			if (!node->prev || !node->next)
				return (print_error(NULL, SYNTAX_PIPE, 2), 1);
			if (node->next && node->next->type == PIPE)
				return (print_error(NULL, SYNTAX_PIPE, 2), 1);
		}
		node = node->next;
	}
	return (0);
}

int	check_rdir(t_token *node)
{
	while (node)
	{
		if (rdir(node) == 2)
		{
			if (!node->next)
				return (print_error(NULL, SYNTAX_LINE, 2), 1);
			if (rdir(node->next) || node->next->type == PIPE)
				return (print_error(NULL, SYNTAX_LINE, 2), 1);
			return (0);
		}
		else if (rdir(node) == 1)
		{
			if (!node->next)
				return (print_error(NULL, SYNTAX_LINE, 2), 1);
			else if ((rdir(node->next) || node->next->type == PIPE))
			{
				print_error(NULL, "syntax error near unexpected token `", 2);
				ft_putstr_fd(node->next->value, 2);
				return (ft_putendl_fd("\'", 2), 1);
			}
		}
		node = node->next;
	}
	return (0);
}

int	cmd_check(t_token *node)
{
	if (check_pipes(node))
		return (1);
	if (check_rdir(node))
		return (1);
	return (0);
}
