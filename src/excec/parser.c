

#include "../../inc/minishell.h"
#include <string.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <unistd.h>

void	pars_multiple_cmd(t_data *data)
{
	t_token	*node;

	node = data->token;
	while (node)
	{
		if (rdir(node) == 2)
			data->here = 1;
		if (node->type == PIPE)
			data->pipes = 1;
		node = node->next;
	}
	pars_multiple(data);
}

int	how_much_cmd(t_data *data)
{
	t_token	*node;

	node = data->token;
	if ((is_args(node) && !node->next))
		return (1);
	else
	{
		while (node)
		{
			if (node->type == PIPE || rdir(node))
				return (0);
			node = node->next;
		}
		return (1);
	}
	return (0);
}

void	parser(t_data *data)
{
	if (cmd_check(data->token))
		return ;
	if (!data->token)
		return ;
	if (how_much_cmd(data) == 1)
		pars_single_cmd(data);
	else
		pars_multiple_cmd(data);
}

void	pars_multiple(t_data *data)
{
	t_token	*node;
	char	**env;
	int		pipfd[2];

	env = env_to_char(data);
	node = data->token;
	while (node)
	{
		if (is_args(node) || rdir(node))
		{
			if (data->pipes)
				if (pipe(pipfd) == -1)
					ft_exit(strerror(errno));
			data->pid = fork();
			if (data->pid == 0)
				handle_child_process(data, node, pipfd, env);
			else
				node = handle_parent_process(data, pipfd, node);
		}
		if (node)
			node = node->next;
	}
	while (wait(NULL) > 0)
		;
	close_and_zero(data, pipfd);
}
