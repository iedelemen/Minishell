
#include "../../inc/libft.h"
#include "../../inc/minishell.h"
#include <sys/stat.h>
#include <unistd.h>

int	is_directory(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == -1)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

char	*handle_executable_file(t_token *node, char *path)
{
	char	*tmp;

	if (is_directory(path))
	{
		print_error(node, ": is a directory\n", 126);
		return (NULL);
	}
	else if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
		{
			tmp = ft_strdup(path);
			add_garbage_c(tmp);
			return (tmp);
		}
		print_error(node, ": Permission denied\n", 126);
		return (NULL);
	}
	print_error(node, ": No such file or directory\n", 127);
	return (NULL);
}

char	*finding_path(t_data *data, t_token *node)
{
	t_env	*env;
	char	**path;
	int		i;

	i = -1;
	if (!node)
		return (NULL);
	if (!(*node->value))
		return (print_error(data->token, ": command not found\n", 127), NULL);
	if (builtins(node->value))
		return (NULL);
	if (is_path(node->value))
		return (handle_executable_file(node, node->value));
	env = get_env(data, "PATH");
	if (!env)
		return ((print_error(node, ": command not found\n", 127)), NULL);
	path = ft_split(env->value, ':');
	add_garbage_c(path);
	while (path[++i])
		add_garbage_c(path[i]);
	return (check_path(path, node));
}

char	**env_to_char(t_data *data)
{
	t_env	*node;
	char	**env;
	int		i;

	i = 0;
	node = data->env;
	while (node)
	{
		node = node->next;
		i++;
	}
	env = gc_malloc(sizeof(char *) * (i + 1));
	i = -1;
	node = data->env;
	while (++i, node->next)
	{
		if (node->value)
			env[i] = ft_str_arr_join((char *[]){node->key, "=", node->value},
					3);
		else
			env[i] = ft_str_arr_join((char *[]){node->key, "="}, 2);
		node = node->next;
	}
	env[i] = NULL;
	return (env);
}

char	**get_command_args(t_token *node)
{
	t_token	*tmp;
	char	**args;
	int		i;

	i = 0;
	tmp = node;
	while (tmp && (is_args(tmp) || rdir(tmp) == 1))
	{
		if (is_args(tmp) && (!tmp->prev || (tmp->prev && !rdir(tmp->prev))))
			i++;
		tmp = tmp->next;
	}
	args = gc_malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (node && (is_args(node) || rdir(node) == 1))
	{
		if (is_args(node) && (!node->prev || (node->prev && !rdir(node->prev))))
		{
			args[i] = node->value;
			i++;
		}
		node = node->next;
	}
	args[i] = NULL;
	return (args);
}
