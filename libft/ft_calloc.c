

#include "../inc/libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t c, size_t s)
{
	char	*ptr;

	ptr = malloc(c * s);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, s * c);
	return (ptr);
}
