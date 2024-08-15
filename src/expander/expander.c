
#include "../../inc/libft.h"
#include "../../inc/minishell.h"

t_token	*args_to_token(char **args)
{
	t_token	*token;
	int		i;

	i = -1;
	token = NULL;
	while (args[++i])
		add_token_back(&token, new_token(args[i], WORD));
	return (token);
}

void	join_value(t_token *node)
{
	char	*new;

	new = ft_strjoin(node->prev->value, node->value);
	add_garbage_c(new);
	node->prev->value = new;
	node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	node->prev->flag = node->flag;
}

void	expander_utils(t_data *data, t_token *node, char *tmp, int i)
{
	t_env	*env;
	char	*str;
	char	*env_key;
	char	*prev;

	env_key = NULL;
	str = NULL;
	env_key = ft_substr(tmp + 1, 0, i - 1);
	add_garbage_c(env_key);
	prev = ft_substr(node->value, 0, tmp - node->value);
	add_garbage_c(prev);
	env = get_env(data, env_key);
	if (env && env->value)
		str = ft_str_arr_join((char *[]){prev, env->value, tmp + i}, 3);
	else if (*env_key == '?')
		str = ft_str_arr_join((char *[]){prev, exit_status(), tmp + 2}, 3);
	else
		str = ft_str_arr_join((char *[]){prev, tmp + i}, 2);
	if (str == NULL)
		str = "";
	garbage_collecter(node->value);
	node->value = ft_strdup(str);
	add_garbage_c(node->value);
	garbage_collecter(str);
}

void	select_expend_value(t_data *data, t_token *node)
{
	char	*tmp;
	int		i;

	tmp = ft_strchr(node->value, '$');
	while (tmp)
	{
		i = 1;
		while (tmp[i] && (ft_isalnum(tmp[i]) || tmp[i] == '?' || tmp[i] == '_'))
		{
			i++;
			if (ft_isdigit(tmp[1]))
				break ;
		}
		if (i == 1)
		{
			tmp++;
			tmp = ft_strchr(tmp, '$');
			continue ;
		}
		expander_utils(data, node, tmp, i);
		tmp = ft_strchr(node->value, '$');
	}
}

void	expander(t_data *data)
{
	t_token	*node;
	int		hedir;

	hedir = 0;
	node = data->token;
	while (node)
	{
		if (node->type == HEREDOC)
		{
			node = node->next;
			hedir = 1;
			continue ;
		}
		if ((node->type == WORD || node->type == DOUBLE_QUOTE) && !hedir)
			select_expend_value(data, node);
		if (node->prev && node->prev->flag)
			join_value(node);
		if (!node->flag)
			hedir = 0;
		node = node->next;
	}
}
