

#include "../../inc/libft.h"
#include "../../inc/minishell.h"

int	check_valid_key(char *key, t_token *node)
{
	int	i;

	i = 0;
	while (key[i] && ((ft_isalnum(key[i]) && !ft_isdigit(key[0]))
			|| key[i] == '_'))
		i++;
	if ((int)ft_strlen(key) != i)
		return (print_error(node, ": not a valid identifier\n", 1), 1);
	return (0);
}

void	unset_organizer(t_data *data, t_env *tmp)
{
	t_env	*prev;

	prev = data->env;
	if (tmp)
	{
		while (prev)
		{
			if (prev->next == tmp)
				break ;
			prev = prev->next;
		}
		if (prev)
			prev->next = tmp->next;
		else
			data->env = tmp->next;
	}
}

void	cmd_unset(t_data *data, t_token *node)
{
	t_env	*tmp;
	t_env	*prev;
	int		flag;

	flag = 0;
	while (node)
	{
		prev = data->env;
		if (check_valid_key(node->value, node))
		{
			node = node->next;
			flag = 1;
			continue ;
		}
		tmp = get_env(data, node->value);
		unset_organizer(data, tmp);
		node = node->next;
	}
	*get_exit_status() = flag;
}
