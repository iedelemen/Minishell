
int	ft_isalnum(int x)
{
	if ((x <= 'z' && x >= 'a') || (x <= 'Z' && x >= 'A'))
		return (1);
	else if (x <= '9' && x >= '0')
		return (1);
	return (0);
}
