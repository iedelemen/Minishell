
#include "../../inc/libft.h"
#include "../../inc/minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>

int	permission_check(t_token *node)
{
	if (rdir(node) == 1)
	{
		if (access(node->next->value, F_OK) == 0)
		{
			if (access(node->next->value, W_OK) != 0)
			{
				print_error(node->next, ": Permission denied\n", 1);
				return (1);
			}
		}
	}
	return (0);
}

void	handle_heredoc(char *delimiter, t_data *data)
{
	char	*line;
	int		fd;
	t_token	*node;

	fd = open("/tmp/.heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		ft_exit(strerror(errno));
	g_signal = HERE_SIG;
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		node = new_token(line, WORD);
		select_expend_value(data, node);
		write(fd, node->value, ft_strlen(node->value));
		write(fd, "\n", 1);
		add_garbage_c(line);
	}
	close(fd);
}

void	exec_heredoc(t_token *node, t_data *data)
{
	while (node)
	{
		if (node->type == HEREDOC)
			handle_heredoc(node->next->value, data);
		node = node->next;
	}
}

void	rdir_dup(int fd, int std, t_token *node)
{
	if (fd == -1)
	{
		print_error(node, ": No such file or directory\n", 1);
		ft_exit(NULL);
	}
	dup2(fd, std);
	close(fd);
}

void	handle_redirection(t_token *node, t_data *data)
{
	if (data->here)
		exec_heredoc(node, data);
	g_signal = EXEC_SIG;
	while (node && (rdir(node) || is_args(node)))
	{
		if (permission_check(node))
			ft_exit(NULL);
		if (node->type == REDIRECT_OUT)
			rdir_dup(open(node->next->value, O_WRONLY | O_CREAT | O_TRUNC,
					0644), STDOUT_FILENO, node->next);
		else if (node->type == APPEND)
			rdir_dup(open(node->next->value, O_WRONLY | O_CREAT | O_APPEND,
					0644), STDOUT_FILENO, node->next);
		else if (node->type == REDIRECT_IN)
			rdir_dup(open(node->next->value, O_RDONLY), STDIN_FILENO,
				node->next);
		else if (node->type == HEREDOC)
			rdir_dup(open("/tmp/.heredoc_tmp", O_RDONLY), STDIN_FILENO,
				node->next);
		node = node->next;
	}
}
