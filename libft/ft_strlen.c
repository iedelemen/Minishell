
#include <unistd.h>

size_t	ft_strlen(const char *a)
{
	int	x;

	x = 0;
	while (a[x])
		x++;
	return (x);
}
