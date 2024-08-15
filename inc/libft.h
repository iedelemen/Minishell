
#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>

int		ft_isalnum(int x);
int		ft_isalpha(int x);
int		ft_isascii(int x);
int		ft_isdigit(int x);
int		ft_isprint(int x);
int		ft_isspace(int c);
int		ft_tolower(int c);

int		ft_atoi(const char *str);
int		ft_strncmp(char *dest, char *src, size_t len);
int		ft_strcmp(const char *str1, const char *str2);

char	*ft_itoa(int nbr);
char	*ft_strdup(const char *s1);
char	*ft_strchr(const char *s, int c);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);

size_t	ft_strlen(const char *a);
size_t	ft_strlcpy(char *dest, char *src, size_t size);
size_t	ft_strlcat(char *dest, const char *src, size_t size);

void	ft_bzero(void *s, size_t n);

void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	*ft_calloc(size_t c, size_t s);
void	*ft_memset(void *dest, int c, size_t len);

#endif
