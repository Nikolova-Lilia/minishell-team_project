#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
size_t  ft_strlen(const char *str);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
int     ft_strcmp(const char *s1, const char *s2);
int     ft_find_and_replace(t_list *env, t_env *node_to_check);
int     ft_export(int argc, char **argv, t_list **env);
int     ft_env(int argc, t_list *env);

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

void    ft_del_env_node(t_env *env_node)
{
    free(env_node->key);
    free(env_node->value);
    free(env_node);
}

int     ft_strcmp(const char *s1, const char *s2)
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

int     ft_find_and_replace(t_list *env, t_env *node_to_check)
{
    t_env *node_cont;

    if (!env)
        return (0);

    while (env != NULL)
    {
        node_cont = (t_env *)env->content;
        if (ft_strcmp(node_cont->key,node_to_check->key) == 0)
        {
            free(node_cont->value);
            node_cont->value = node_to_check->value;
            free(node_to_check->key);
            free(node_to_check);
            return (1);
        }
        env = env->next;
    }

    return (0);
}

int     ft_export(int argc, char **argv, t_list **env)
{
    t_env *env_node;
    t_list *list_node;

    if (argc == 1)
        return (ft_export_env_print(*env));
    if (argc == 2 && argv[1] != NULL && env != NULL)
    {
        env_node = ft_split_env(argv[1]);
        if (env_node != NULL)
        {
            if (ft_find_and_replace(*env, env_node) == 1)
                return (1);
            list_node = ft_lstnew((void *)env_node);
            if (list_node != NULL)
            {
                if (*env == NULL)
                    *env = list_node;
                else
                    ft_lstadd_back(env, list_node);
                return (1);
            }
            ft_del_env_node(env_node);
        }
    }    
    perror("export error");
    return (0);
}

int     ft_export_env_print(t_list *env)
{
    t_env *node_cont;

    if (!env)
        return (0);
    
    while (env != NULL)
    {
        node_cont = (t_env *)env->content;
        printf("declare -x %s=\"%s\"\n", node_cont->key, node_cont->value);
        env = env->next;
    }
    return (1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*p;
	size_t	lens1;
	size_t	lenset;
	size_t	start;
	size_t	end;

	if (!s1 || !set)
		return (NULL);
	lens1 = ft_strlen(s1);
	lenset = ft_strlen(set);
	start = 0;
	end = lens1;
	while (ft_strchr(set, s1[start]) && lens1 && lenset && start < lens1)
		start++;
	if (start == end)
		return ((char *)ft_calloc(1, sizeof(char)));
	while (ft_strchr(set, s1[end]) && lens1 && lenset)
		end--;
	if (lenset == 0)
		end--;
	p = (char *)malloc((end - start + 2) * sizeof(char));
	if (p != NULL)
		ft_strlcpy(p, &s1[start], end - start + 2);
	return (p);
}

int ft_env(int argc, t_list *env)
{
    if (argc == 1)
    {
        ft_env_exec(env);
        return (1);
    }
    perror("env: invalid option\n");
    return (0);
}

int ft_env_exec(t_list *env)
{
    t_env *node_cont;

    if (!env)
        return (0);

    while (env != NULL)
    {
        node_cont = (t_env *)env->content;
        printf("%s=%s\n", node_cont->key, node_cont->value);
        env = env->next;
    }
    return (1);
}

int     ft_unset(int argc, char **argv, t_list **env)
{
    

}

int ft_find_and_remove(t_list **env, char *key)
{
    t_env *node_cont;
    t_list *head;
    t_list *prev;

    prev = NULL;
    head = *env;
    if (!env)
        return (0);
    while (head != NULL)
    {
        node_cont = (t_env *)head->content;
        if (ft_strcmp(node_cont->key,key) == 0)
        {
            if (prev == NULL)
                *env = head->next;
            else
                prev->next = head->next;
            free(node_cont->value);
            free(node_cont->key);
            free(node_cont);
            free(head);
            return (1);
        }
        prev = head; 
        head = head->next;
    }
    return (0);
}

t_env   *ft_split_env(char *env)
{
    int i;
    char *key;
    char *value;
    char *trim;
    t_env *node;

    i = 0;
    while (env[i] != '=')
    {
        if (env[i] == '\0')
            return NULL;
        i++;
    }
    
    key = (char *)malloc(sizeof(char) * (i + 1));
    if (key == NULL)
        return NULL;

    ft_strlcpy(key, env, i);
    value = (char *)malloc(sizeof(char) * (strlen(env) - i));
    if (value == NULL)
        return NULL;

    ft_strlcpy(value, env + i + 1, ft_strlen(env) - i - 1);
    trim = ft_strtrim(value, "\"");
    free(value);
    value = trim;
    node = (t_env *)malloc(sizeof(t_env));
    if (node == NULL)
        return NULL;

    node->key = key;
    node->value = value;

    return node;
}

int	ft_atoi(const char *nptr)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while ((*nptr >= 9 && *nptr <= 13) || (*nptr == 32))
		nptr++;
	if (*nptr == '\0')
		return (0);
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			sign *= -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + *nptr - '0';
		nptr++;
	}
	return (result * sign);
}

int ft_digit_check(const char *str)
{
    int flag;

    flag = 0;
    while (*str)
    {
        if (*str >= 48 && *str <= 57)
            str++;
        else if (*str == 45)
        {
            flag++;
            str++;
            if (flag > 1)
                return (0);
        }
        else
            return (0);
    }
    return (1);
}

int ft_exit(int argc, char **argv, char **env)
{
    if (argc <= 2)
    {
        if (argc == 2)
        {
            if (ft_digit_check(argv[1]) == 1)
            {
                exit(ft_atoi(argv[1]));
                return (1);
            }

            perror("Bad exit number\n");
        }
        else
        {
            exit(0);
            return (1);
        }
    }
    return (0);
}

void    main(int argc, char **argv, char **env)
{
    (void **)argv;
    (void)argc;

    char **args = malloc(sizeof(char*) * 2);
    char *arg1 = "export";
    char *arg2 = "PATH=/usr/";

    args[0] = arg1;
    args[1] = arg2;

    t_list *head;
    int i;

    head = NULL;
    i = 0;

    while (env[i] != NULL)
    {
        t_env *env_node;
        t_list *list_node;
        
        env_node = ft_split_env(env[i]);
        if (env_node == NULL && head != NULL)
        {
            ft_lstclear(&head, ft_del_env_node);
        }
        
        list_node = ft_lstnew((void *)env_node);
        if (list_node == NULL)
        {
            free(env_node);
            
            if (head != NULL)
                ft_lstclear(&head, ft_del_env_node);
        }

        if (head == NULL)
            head = list_node;
        else
            ft_lstadd_back(&head, list_node);
    }

    ft_export(2, args, env);
    free(args);
}