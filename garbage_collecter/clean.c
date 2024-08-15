
#include "../inc/minishell.h"
#include <stdlib.h>

void	gc_free(void)
{
	t_malloc	*tmp;
	t_malloc	*garbage_c;

	garbage_c = get_garbage_c()->next;
	while (garbage_c)
	{
		tmp = garbage_c;
		garbage_c = garbage_c->next;
		free(tmp->ptr);
		free(tmp);
	}
}

void	garbage_collecter(void *ptr)
{
	t_malloc	*garbage_c;
	t_malloc	*tmp;

	if (!ptr)
		return ;
	garbage_c = get_garbage_c();
	while (garbage_c)
	{
		if (garbage_c->ptr == ptr)
		{
			tmp->next = garbage_c->next;
			free(garbage_c);
			free(ptr);
			ptr = NULL;
			return ;
		}
		tmp = garbage_c;
		garbage_c = garbage_c->next;
	}
}

void	free_token(t_data *data)
{
	t_token	*token;
	t_token	*tmp;

	token = data->token;
	while (token)
	{
		tmp = token;
		token = token->next;
		garbage_collecter(tmp->value);
		garbage_collecter(tmp);
	}
	data->token = NULL;
}
