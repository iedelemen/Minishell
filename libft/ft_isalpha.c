
int	ft_isalpha(int x)
{
	if ((x <= 'Z' && x >= 'A') || (x <= 'z' && x >= 'a'))
		return (1);
	return (0);
}
