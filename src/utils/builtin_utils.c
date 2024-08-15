

#include "../../inc/libft.h"
#include "../../inc/minishell.h"
#include <unistd.h>

int	builtins(char *path)
{
	if (!is_echo(path, "echo"))
		return (1);
	if (ft_strcmp(path, "cd") == 0)
		return (1);
	if (ft_strcmp(path, "pwd") == 0)
		return (1);
	if (ft_strcmp(path, "export") == 0)
		return (1);
	if (ft_strcmp(path, "unset") == 0)
		return (1);
	if (ft_strcmp(path, "env") == 0)
		return (1);
	if (ft_strcmp(path, "exit") == 0)
		return (1);
	return (0);
}

int	is_path(char *str)
{
	return ((str[0] == '.' && str[1] == '/') || str[0] == '/');
}

char	*check_path(char **path, t_token *node)
{
	int		i;
	char	*tmp;

	tmp = NULL;
	i = 0;
	while (path[i])
	{
		tmp = ft_str_arr_join((char *[]){path[i], "/", node->value}, 3);
		if (access(tmp, F_OK) == 0)
		{
			if (access(tmp, X_OK) == 0)
				return (tmp);
			else
			{
				print_error(node, ": Permission denied\n", 126);
				return (NULL);
			}
		}
		i++;
		garbage_collecter(tmp);
	}
	return (print_error(node, ": command not found\n", 127), NULL);
}

int	my_builtin(t_data *data, char **args)
{
	t_token	*node;

	if (args)
	{
		node = args_to_token(args);
		if (!ft_strcmp(args[0], "cd"))
			return (cmd_cd(data, node->next, NULL, NULL), 1);
		else if (!ft_strcmp(args[0], "pwd"))
			return (cmd_pwd(), 1);
		else if (!is_echo(args[0], "echo"))
			return (cmd_echo(node->next), 1);
		else if (!ft_strcmp(args[0], "env"))
			return (cmd_env(data, node), 1);
		else if (!ft_strcmp(args[0], "export"))
			return (cmd_export(data, node->next), 1);
		else if (!ft_strcmp(args[0], "unset"))
			return (cmd_unset(data, node->next), 1);
		else if (!ft_strcmp(args[0], "exit"))
			return (cmd_exit(node->next), 1);
	}
	return (0);
}

void	pars_single_cmd(t_data *data)
{
	if (is_args(data->token))
	{
		while (data->token && *data->token->value == '\0'
			&& data->token->type == WORD)
			data->token = data->token->next;
		if (!data->token)
			return ;
		if (!ft_strcmp(data->token->value, "cd"))
			cmd_cd(data, data->token->next, NULL, NULL);
		else if (!ft_strcmp(data->token->value, "pwd"))
			cmd_pwd();
		else if (!is_echo(data->token->value, "echo"))
			cmd_echo(data->token->next);
		else if (!ft_strcmp(data->token->value, "env"))
			cmd_env(data, data->token);
		else if (!ft_strcmp(data->token->value, "export"))
			cmd_export(data, data->token->next);
		else if (!ft_strcmp(data->token->value, "unset"))
			cmd_unset(data, data->token->next);
		else if (!ft_strcmp(data->token->value, "exit"))
			cmd_exit(data->token->next);
		else
			use_execve(data);
	}
}
