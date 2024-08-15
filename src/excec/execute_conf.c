

#include "../../inc/libft.h"
#include "../../inc/minishell.h"
#include <string.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <unistd.h>

void	configure_fd(t_data *data, t_token *node, int in_out[2], int pipfd[2])
{
	if (data->in_fd != 0)
	{
		if (!in_out[0])
		{
			dup2(data->in_fd, 0);
			close(data->in_fd);
		}
		close(pipfd[0]);
	}
	if (node && node->type == PIPE)
	{
		if (!in_out[1])
		{
			dup2(pipfd[1], 1);
			close(pipfd[1]);
		}
		close(pipfd[0]);
	}
}

void	setup_pipes(t_data *data, t_token *node, int pipfd[2])
{
	int	in_out[2];

	in_out[0] = 0;
	in_out[1] = 0;
	while (node && node->type != PIPE)
	{
		if (node->type == APPEND || node->type == REDIRECT_OUT)
			in_out[1] = 1;
		else if (node->type == REDIRECT_IN || node->type == HEREDOC)
			in_out[0] = 1;
		node = node->next;
	}
	configure_fd(data, node, in_out, pipfd);
}

char	**args_and_path(t_data *data, t_token *node, char **path)
{
	char	**args;

	args = NULL;
	if (is_args(node))
	{
		args = get_command_args(node);
		*path = finding_path(data, node);
		while (node && is_args(node))
			node = node->next;
		handle_redirection(node, data);
	}
	else if (node && rdir(node))
	{
		handle_redirection(node, data);
		while (node)
		{
			if (is_args(node) && node->prev && !rdir(node->prev))
				break ;
			node = node->next;
		}
		args = get_command_args(node);
		*path = finding_path(data, node);
	}
	return (args);
}

void	handle_child_process(t_data *data, t_token *node, int pipfd[2],
		char **env)
{
	char	**args;
	char	*path;

	path = NULL;
	args = NULL;
	args = args_and_path(data, node, &path);
	signal_base();
	if (data->pipes)
		setup_pipes(data, node, pipfd);
	if (my_builtin(data, args))
		ft_exit(NULL);
	if (path)
		if (execve(path, args, env) == -1)
			ft_putendl_fd(strerror(errno), 2);
	ft_exit(NULL);
}

t_token	*handle_parent_process(t_data *data, int pipfd[2], t_token *node)
{
	g_signal = EXEC_SIG;
	if (data->in_fd != 0)
		close(data->in_fd);
	if (data->here)
	{
		waitpid(data->pid, get_exit_status(), 0);
		data->here = 0;
	}
	signal_base();
	while (node && node->type != PIPE)
		node = node->next;
	if (!(node && node->type == PIPE))
		waitpid(data->pid, get_exit_status(), 0);
	else
	{
		data->in_fd = pipfd[0];
		close(pipfd[1]);
	}
	update_exit_status();
	return (node);
}
