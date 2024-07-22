#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef enum e_token_type
{
    TOKEN,
    WORD,
    ASSIGNMENT_WORD,
    I0_NUMBER,
    AND_IF,
    OR_IF,
    LESS,
    GREAT,
    DLESS,
    DGREAT,
    PIPE,
    LPARA,
    RPARA
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *str;
} t_token;

typedef struct s_list
{
    void *content;
    t_list *next;
} t_list;

typedef struct s_env
{
    char *key;
    char *value;
} t_env;

int ft_echo(t_token *args, int size, t_list **env);
int ft_cd(t_token *args, int size, t_list **env);
int ft_pwd(void);
int ft_env(t_list **env);
// TODO: function to evaluate environment variables
int ft_find_closing_brace(t_token *tokens, int size);
int ft_and_if(t_token *tokens, int size, int i, t_list **env);
int ft_or_if(t_token *tokens, int size, int i, t_list **env);
int ft_pipe_exec(t_token *tokens, int size, int fd, int* pipe, t_list **env);
int ft_pipe(t_token *tokens, int size, int i, t_list **env);
int ft_great(t_token *tokens, int size, int i, t_list **env);
int ft_dgreat(t_token *tokens, int size, int i, t_list **env);
int ft_less(t_token *tokens, int size, int i, t_list **env);
int ft_dless(t_token *tokens, int size, int i, t_list **env);
int ft_check_operators(t_token *tokens, int size, t_list **env);
int ft_check_joins(t_token *tokens, int size, t_list **env);
int ft_run_shell(t_token *tokens, int size, t_list **env);
int ft_run_command(t_token *tokens, int size, t_list **env);
int ft_exec(t_token *tokens, int size, t_list **env);
int ft_strcmp(const char *s1, const char *s2);

char *ft_join_tokens(t_token *tokens, int size);
char *ft_collect_input(char *end);
int ft_check_n(char *str);
size_t ft_strlen(const char *str);
void ft_putstr_fd(char *s, int fd);
char *ft_strjoin(char *str1, char *str2);
void ft_strcpy(char *dest, char *src);
void ft_add_env(char* strtod, t_list **env);
void ft_remove_env(char* key, t_list **env);
static size_t	ft_strcount(char const *s, char c);
static size_t	ft_array_sorting(char const *s, char c);
static char	**ft_dealloc(char **str, int i);
static int	ft_check_null(size_t len, char const *s);
char	**ft_split(char const *s, char c);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstdelone(t_list *lst, void (*del)(void *));
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstnew(void *content);
void ft_del_env(t_env *env);
t_env *ft_find_env(char *key, t_list **env);

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list) * 1);
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}

t_list	*ft_lstlast(t_list *lst)
{
	while (lst != NULL && lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new;
}

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (!lst)
		return ;
	while (*lst != NULL)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		(*lst) = temp;
	}
}

static size_t	ft_strcount(char const *s, char c)
{
	size_t	n;
	int		count;

	count = 0;
	n = 0;
	while (*s != '\0')
	{
		if (*s == c)
		{
			if (count > 0)
			{
				n++;
				count = 0;
			}
		}
		else
			count++;
		s++;
	}
	if (count > 0)
		n++;
	return (n);
}

static size_t	ft_array_sorting(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (*s != '\0')
	{
		if (*s == c)
			return (len);
		else
			len++;
		s++;
	}
	return (len);
}

static char	**ft_dealloc(char **str, int i)
{
	int	f;

	f = 0;
	while (f < i)
		free(str[f++]);
	free(str);
	return (NULL);
}

static int	ft_check_null(size_t len, char const *s)
{
	if (s[len] == '\0')
		return (0);
	else
		return (1);
}

char	**ft_split(char const *s, char c)
{
	char		**str;
	int			i;
	int			len;

	i = 0;
	if (!s)
		return (NULL);
	str = (char **)malloc((ft_strcount(s, c) + 1) * sizeof(char *));
	if (!str)
		return (NULL);
	while (*s != '\0')
	{
		if (*s != c)
		{
			len = ft_array_sorting(s, c);
			str[i++] = ft_substr(s, 0, len);
			if (str[i - 1] == NULL)
				return (ft_dealloc(str, i));
			s += len + ft_check_null(len, s);
		}
		else
			s++;
	}
	str[i] = NULL;
	return (str);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	if (!dst || !src)
		return (0);
	if (!size)
		return (ft_strlen(src));
	i = 0;
	while (src[i] != '\0' && (i < (size - 1)))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	while (src[i] != '\0')
	{
		i++;
	}
	return (i);
}

void ft_add_env(char* str, t_list **env)
{
    // PERSON=LILIA
    t_env *node;
    int i;
    char *key;
    char *value;
    
    i = 0;
    while (str[i] != '=')
        i++;

    node = malloc(sizeof(t_env));
    key = malloc(i * sizeof(char));
    value = malloc(ft_strlen(str + i + 1) * sizeof(char));
    ft_strlcpy(key, str, i);
    ft_strcpy(value, str + i + 1);
    node->key = key;
    node->value = value;

    ft_lstadd_back(env, node);
}

void ft_del_env(t_env *env)
{
    free(env->key);
    free(env->value);
    free(env);
}

t_env *ft_find_env(char *key, t_list **env)
{
    t_list *temp;
    t_env *tempenv;

    temp = *env;

    while (temp)
    {
        tempenv = (t_env *)temp->content;
        if (ft_strcmp(tempenv->key, key) == 0)
            return temp;
    }

    return NULL;
}

void ft_remove_env(char* key, t_list **env)
{
    t_list *temp;
    
    temp = ft_find_env(key, env);

    if (temp)
        ft_lstdelone(temp, ft_del_env);
}

int ft_strcmp(const char *s1, const char *s2)
{
    size_t i;

    i = 0;
    while (s1[i] != 0 && s2[i] != 0 && s1[i] == s2[i])
    {
        i++;
    }
    if (s1[i] == s2[i])
        return (0);
    else
        return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int ft_echo(t_token *args, int size, t_list **env)
{
    int i = 1;
    int flag = 0;
    int t = ft_check_n(args[i].str);
    if (t == 1)
    {
        flag = 1;
        i++;
    }
    while (i < size)
    {
        ft_putstr_fd(args[i].str, 1);
        if (i + 1 < size)
            ft_putstr_fd(" ", 1);
        i++;
    }
    if (flag == 0)
        ft_putstr_fd("\n", 1);

    return 1;
}

int ft_cd(t_token *tokens, int size, t_list **env)
{
    static char olddir[256];
    char curdir[256];

    if (ft_strcmp(tokens[1].str, "-") == 0 && !olddir[0])
    {
        ft_putstr_fd("bash: cd: No such file or directory\n", 1);
        return 0;
    }

    if (getcwd(curdir, sizeof(curdir)) == NULL)
    {
        perror("getcwd() error");
        return 0;
    }

    if (ft_strcmp(tokens[1].str, "-") == 0)
        chdir(olddir);
    else if (size == 1)
        chdir(getenv("HOME"));
    else
        chdir(tokens[1].str);

    ft_strcpy(olddir, curdir);
    return (1);
}

int ft_pwd(void)
{
    char cwd[256];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        return 0;
    }
    else
    {
        ft_putstr_fd(cwd, 1);
        ft_putstr_fd("\n", 1);
    }

    return (1);
}

int ft_check_n(char *str)
{
    if (str[0] == '-' && str[1] == 'n')
        return (1);

    return (0);
}

int ft_env(t_list **env)
{
    t_list *temp;

    temp = *env;
    while (temp)
    {
        ft_putstr_fd(((t_env *)temp->content)->key, 1);
        write(1, "\n", 1);
        temp = temp->next;
    }
    return (1);
}

size_t ft_strlen(const char *str)
{
    unsigned int i;

    i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return ((size_t)i);
}

char *ft_strjoin(char *str1, char *str2)
{
    if (!str1 || !str2)
        return (NULL);

    int len1 = ft_strlen(str1);
    int len2 = ft_strlen(str2);

    char *strjoin = malloc(len1 + len2 + 1);
    if (!strjoin)
    	return (NULL);
    ft_strcpy(strjoin, str1);
    ft_strcpy(strjoin + len1, str2);
    return strjoin;
}

void ft_strcpy(char *dest, char *src)
{
    while (*src)
    {
        *dest++ = *src++;
    }
    *dest = '\0';
}

void ft_putstr_fd(char *s, int fd)
{
    if (!s)
        return;
    write(fd, s, ft_strlen(s));
}

int ft_find_closing_brace(t_token *tokens, int size)
{
	int i;
	int c;

	i = 0;
	c = 0;
	while (i < size)
	{
		if (tokens[i].type == LPARA)
			c++;
		if (tokens[i].type == RPARA)
			c--;

		if (c == 0)
			return (i);
	}
}

int ft_and_if(t_token *tokens, int size, int i, t_list **env)
{
	int left;
	
	left = ft_exec(tokens, i, env);
	if (left == 1)
	{
		int right;
		
		right = ft_exec(tokens + i + 1, size - (i + 1), env);
		return (left && right);
	}

	return (0);
}

int ft_or_if(t_token *tokens, int size, int i, t_list **env)
{
	int left;
	
	left = ft_exec(tokens, i, env);
	if (left == 0)
	{
		int right;
		
		right = ft_exec(tokens + i + 1, size - (i + 1), env);
		return (left || right);
	}

	return (1);
}

int ft_pipe_exec(t_token *tokens, int size, int fd, int* pipe, t_list **env)
{
	int fd_copy;
	
	fd_copy = dup(fd);
	close(pipe[0]);
	dup2(pipe[1], fd);
	ft_exec(tokens, size, env);
	close(pipe[1]);
	dup2(fd_copy, fd);
	close(fd_copy);
}

int ft_pipe(t_token *tokens, int size, int i, t_list **env)
{
	int outcopy;
	int fd[2];
	int id;

	if (pipe(fd) == -1)
		return (1);

	id = fork();
	if (id == -1)
		return (2);

	if (id == 0)
		ft_pipe_exec(tokens, i, 1, fd, env);
	else
    {
        int status;

		ft_pipe_exec(tokens + i + 1, size - (i + 1), 0, fd, env);
        wait(&status);
        return status != 0;
    }

	return (1);
}

int ft_great(t_token *tokens, int size, int i, t_list **env)
{
	int outcopy;
	int fd;
	
	outcopy = dup(1);
	// todo: what if it's not a file
	fd = open(tokens[i + 1].str, O_CREAT | O_WRONLY);
	dup2(fd, 1);
	ft_exec(tokens, i, env);
	close(fd);
	dup2(outcopy, 1);
	close(outcopy);

    return (1);
}

int ft_dgreat(t_token *tokens, int size, int i, t_list **env)
{
	int outcopy;
	int fd;
	
	outcopy = dup(1);
	fd = open(tokens[i + 1].str, O_CREAT | O_WRONLY | O_APPEND);
	dup2(fd, 1);
	ft_exec(tokens, i, env);
	close(fd);
	dup2(outcopy, 1);
	close(outcopy);

    return (1);
}

int ft_less(t_token *tokens, int size, int i, t_list **env)
{
	int outcopy;
	int fd;
	
	outcopy = dup(1);
	fd = open(tokens[0].str, O_RDONLY);
	dup2(fd, 1);
	ft_exec(tokens + i + 1, size - (i + 1), env);
	close(fd);
	dup2(outcopy, 1);
	close(outcopy);

    return (1);
}

char *ft_join_tokens(t_token *tokens, int size)
{
	int i;
	char *end;

	end = (char *)malloc(1 * sizeof(char));
	if (!end)
		return (NULL);
	end[0] = '\0';
	i = 0;
	while (i < size)
	{
		char *new_end = ft_strjoin(end, tokens[i].str);
		free(end);
		end = new_end;
		i++;
	}

	return (end);
}

char *ft_collect_input(char *end)
{
	char *input;

	input = malloc(1 * sizeof(char));
	input[0] = '\0';

	char *line;
	// todo: read line

	while (ft_strcmp(line, end) != 0)
	{
		char *new_input;

		new_input = ft_strjoin(input, line);
		free(input);
		input = new_input;
		// todo: read line again
	}

	return (input);
}

int ft_dless(t_token *tokens, int size, int i, t_list **env)
{
	int fd;
	char *end;
	char *input;

	end = ft_join_tokens(tokens + i + 1, size - (i + 1));
	input = ft_collect_input(end);
	
	// todo: what if not a file? redirect to stdin?
	fd = open(tokens[0].str, O_CREAT | O_WRONLY);
	write(fd, input, ft_strlen(input));

	return (1);
}

int ft_check_operators(t_token *tokens, int size, t_list **env)
{
    int i = 0;

    while (i < size)
    {
        if (tokens[i].type == LPARA)
        {
            i = ft_find_closing_brace(tokens + i, size - i);
            if (i == size - 1)
                return (ft_exec(tokens + 1, size - 2, env));
        }
        else if (tokens[i].type == AND_IF)
			return (ft_and_if(tokens, size, i, env));
    	else if (tokens[i].type == OR_IF)
			return (ft_or_if(tokens, size, i, env));

        i++;
    }

	return (-1);
}

int ft_check_joins(t_token *tokens, int size, t_list **env)
{
	int i;

    i = 0;
    while (i < size)
    {
        if (tokens[i].type == PIPE)
			return (ft_pipe(tokens, size, i, env));
        if (tokens[i].type == GREAT)
			return (ft_great(tokens, size, i, env));
        if (tokens[i].type == DGREAT)
			return (ft_dgreat(tokens, size, i, env));
        if (tokens[i].type == LESS)
			return (ft_less(tokens, size, i, env));
        if (tokens[i].type == DLESS)
			return (ft_dless(tokens, size, i, env));

        i++;
    }

	return (-1);
}

int ft_run_shell(t_token *tokens, int size, t_list **env)
{
    char **args;
	int a;
	int id;
	
	args = malloc(size + 1 * sizeof(char*));
    a = 1;
    while (a < size)
        args[a] = tokens[a].str;
    args[size] = NULL;

    args[0] = ft_strjoin("/bin/", tokens[0].str);
    id = fork();
    if (id == 0)
        execve(args[0], args, NULL);
    else
    {
        int status;
		
        free(args[0]);
        free(args);
        wait(&status);
        return status != 0;
    }
}

int ft_run_command(t_token *tokens, int size, t_list **env)
{
    if (ft_strcmp(tokens[0].str, "echo") == 0)
        return ft_echo(tokens, size, env);

    if (ft_strcmp(tokens[0].str, "cd") == 0)
        return ft_cd(tokens, size, env);

    if (ft_strcmp(tokens[0].str, "pwd") == 0)
        return ft_pwd();

    if (ft_strcmp(tokens[0].str, "env") == 0)
        return ft_env(env);

	return ft_run_shell(tokens, size, env);
}

int ft_exec(t_token *tokens, int size, t_list **env)
{
	int result;

	result = ft_check_operators(tokens, size, env);
	if (result != -1)
		return result;

	result = ft_check_joins(tokens, size, env);
	if (result != -1)
		return result;

	result = ft_run_command(tokens, size, env);
	if (result != -1)
		return result;

    return 0;
}

int main(int argc, char **argv, t_list **env)
{
    t_token token1;
    token1.type = WORD;
    token1.str = "env";
    t_token token2;
    token2.type = WORD;
    token2.str = "..";
    t_token token3;
    token3.type = AND_IF;
    t_token token4;
    token4.type = WORD;
    token4.str = "pwd";
    t_token token5;
    token5.type = AND_IF;
    t_token token6;
    token6.type = WORD;
    token6.str = "cd";
    t_token token7;
    token7.type = WORD;
    token7.str = "-";
    t_token token8;
    token8.type = AND_IF;
    t_token token9;
    token9.type = WORD;
    token9.str = "pwd";
    t_token token10;
    token10.type = AND_IF;
    t_token token11;
    token11.type = WORD;
    token11.str = "cd";
    t_token token12;
    token12.type = WORD;
    token12.str = "-";
    t_token token13;
    token13.type = AND_IF;
    t_token token14;
    token14.type = WORD;
    token14.str = "pwd";

    t_token tokens[] = { token1 }; //, token2, token3, token4, token5, token6, token7, token8, token9, token10, token11, token12, token13, token14 };

    ft_exec(tokens, 1, env);
    return (0);
}

// int main(void)
// {
//     t_token token1;
//     token1.type = WORD;
//     token1.str = "echo";
//     t_token token_n;
//     token_n.type = WORD;
//     token_n.str = "-n";
//     t_token token2;
//     token2.type = WORD;
//     token2.str = "hello ";
//     t_token token3;
//     token3.type = AND_IF;
//     t_token token4;
//     token4.type = WORD;
//     token4.str = "echo";
//     t_token token5;
//     token5.type = WORD;
//     token5.str = "world";
//     t_token token6;
//     token6.type = AND_IF;
//     t_token token7;
//     token7.type = WORD;
//     token7.str = "ls";
//     t_token token8;
//     token8.type = GREAT;
//     t_token token9;
//     token9.type = WORD;
//     token9.str = "file.txt";
//     t_token token10;
//     token10.type = AND_IF;
//     t_token token11;
//     token11.type = WORD;
//     token11.str = "ls";
//     t_token token12;
//     token12.type = PIPE;
//     t_token token13;
//     token13.type = WORD;
//     token13.str = "wc";

//     t_token tokens[] = {token1, token_n, token2, token3, token4, token5, token6, token7, token8, token9, token10, token11, token12, token13};

//     ft_exec(tokens, 14);
//     return (0);
// }
