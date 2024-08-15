

#include "../../inc/libft.h"
#include "../../inc/minishell.h"
#include <stdio.h>

char	*creat_key_value_len(t_token *node, char **value, int *len)
{
	char	*key;

	key = NULL;
	*value = NULL;
	*len = (ft_strchr(node->value, '=') - node->value) * (ft_strchr(node->value,
				'=') != 0);
	if (*len == 0)
		key = ft_strdup(node->value);
	else
	{
		key = ft_substr(node->value, 0, *len);
		*value = ft_substr(node->value, *len + 1, ft_strlen(node->value));
		add_garbage_c(*value);
	}
	add_garbage_c(key);
	return (key);
}

void	print_export(t_data *data)
{
	t_env	*env;

	env = data->env;
	while (env && env->key)
	{
		printf("declare -x %s", env->key);
		if (env->value)
			printf("=\"%s\"\n", env->value);
		else
			printf("\n");
		env = env->next;
	}
}

void	env_organize(char *key, char *value, t_data *data)
{
	t_env	*env;

	env = get_env(data, key);
	if (env && value)
	{
		env->value = ft_strdup(value);
		add_garbage_c(env->value);
	}
	else if (!env && (value || key))
	{
		env = gc_malloc(sizeof(t_env));
		env->key = key;
		env->value = value;
		env->next = data->env;
		data->env = env;
	}
}

void	cmd_export(t_data *data, t_token *node)
{
	char	*key;
	char	*value;
	int		len;
	int		flag;

	flag = 0;
	if (node && node->value)
	{
		while (node && node->value)
		{
			key = creat_key_value_len(node, &value, &len);
			if (check_valid_key(key, node))
			{
				node = node->next;
				flag = 1;
				continue ;
			}
			env_organize(key, value, data);
			node = node->next;
		}
	}
	else
		print_export(data);
	*get_exit_status() = flag;
}
