

#ifndef MINISHELL_H
# define MINISHELL_H

# define SYNTAX_LINE "syntax error near unexpected token `newline'\n"
# define SYNTAX_PIPE "syntax error near unexpected token `|'\n"

# define PROMT_SIG 0
# define HERE_SIG 1
# define EXEC_SIG 2

extern int				g_signal;

typedef struct s_malloc
{
	void				*ptr;
	struct s_malloc		*next;
}						t_malloc;

enum					e_token_type
{
	WORD,
	PIPE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
};

typedef struct s_token
{
	enum e_token_type	type;
	char				*value;
	struct s_token		*prev;
	struct s_token		*next;
	int					flag;
}						t_token;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_data
{
	t_env				*env;
	t_token				*token;
	char				*cmd;
	int					syntax;
	int					in_fd;
	int					pid;
	int					pipes;
	int					here;
}						t_data;

t_malloc				*get_garbage_c(void);
void					*gc_malloc(unsigned int size);
void					add_garbage_c(void *ptr);

void					gc_free(void);
void					garbage_collecter(void *ptr);
void					free_token(t_data *data);

void					ft_exit(char *str);
void					print_error(t_token *node, char *str, int exit_status);
void					close_and_zero(t_data *data, int pipfd[2]);
char					*exit_status(void);

void					initilazor(t_data *data);
void					create_env(char **envp, t_data *data);
t_env					*get_env(t_data *data, char *key);

char					*ft_str_arr_join(char **str_list,
							unsigned int str_count);
int						rdir(t_token *node);
int						is_args(t_token *node);
int						skip_space(char *str, int i);
int						ft_validchar(int c);

void					lexer(t_data *data);

t_token					*last_token(t_token *node);
t_token					*new_token(char *value, enum e_token_type type);
void					add_token_back(t_token **node, t_token *new);

void					expander(t_data *data);
void					select_expend_value(t_data *data, t_token *node);

void					parser(t_data *data);
t_token					*args_to_token(char **args);

char					*handle_executable_file(t_token *node, char *path);
char					*finding_path(t_data *data, t_token *node);
char					**env_to_char(t_data *data);
char					**get_command_args(t_token *node);

int						builtins(char *path);
int						is_path(char *str);
char					*check_path(char **path, t_token *node);
int						my_builtin(t_data *data, char **args);
void					pars_single_cmd(t_data *data);

void					handle_redirection(t_token *node, t_data *data);

int						cmd_check(t_token *node);
void					update_oldpwd(t_data *data, char *pwd);
void					cmd_cd(t_data *data, t_token *node, char *path,
							char *pwd);
void					cmd_echo(t_token *node);
void					cmd_env(t_data *data, t_token *node);
void					cmd_exit(t_token *node);
int						*get_exit_status(void);
void					update_exit_status(void);
void					cmd_export(t_data *data, t_token *node);
t_env					*get_pwd(t_data *data);
void					cmd_pwd(void);
int						check_valid_key(char *key, t_token *node);
void					cmd_unset(t_data *data, t_token *node);

void					use_execve(t_data *data);

void					signal_base(void);
void					handle_sigint(int sig);
int						is_echo(char *str1, char *str2);
void					pars_multiple(t_data *data);

t_token					*handle_parent_process(t_data *data, int pipfd[2],
							t_token *node);
void					handle_child_process(t_data *data, t_token *node,
							int pipfd[2], char **env);
#endif
