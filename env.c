#include <stdio.h>
#include <stdlib.h>

typedef struct s_list
{
    void *content;
    struct s_list *next;
} t_list;

typedef struct s_env
{
    char *key;
    char *value;
} t_env;

int ft_env(int argc, t_list *env);
int ft_env_exec(t_list *env);

/*
t_env   *ft_split_env(char *env); //testing only
void	ft_lstclear(t_list **lst, void (*del)(void *)); //testing only
void	ft_lstdelone(t_list *lst, void (*del)(void *)); //testing only
void    ft_del_env_node(void *env_node); //testing only
t_list	*ft_lstnew(void *content); //testing only
void	ft_lstadd_back(t_list **lst, t_list *new); //testing only
size_t ft_strlen(const char *str);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strchr(const char *s, int c); //testing only
void	*ft_calloc(size_t nmemb, size_t size);
t_list	*ft_lstlast(t_list *lst);
*/

int ft_env(int argc, t_list *env)
{
    if (argc == 1)
    {
        ft_env_exec(env);
        return (1);
    }
    printf("env: invalid option\n");
    //perror("env: invalid option\n");
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

//below helper functions for checking purpuses only
/*
t_env   *ft_split_env(char *env) //testing only
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

    ft_strlcpy(key, env, i + 1);
    value = (char *)malloc(sizeof(char) * (ft_strlen(env) - i));
    if (value == NULL)
        return NULL;

    ft_strlcpy(value, env + i + 1, ft_strlen(env) - i);
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
void	ft_lstclear(t_list **lst, void (*del)(void *)) //testing only
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
void	ft_lstdelone(t_list *lst, void (*del)(void *)) //testing only
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
}
void    ft_del_env_node(void *env_node) //testing only
{
    t_env *node;
    node = (t_env *)env_node;
    free(node->key);
    free(node->value);
    free(node);
}
t_list	*ft_lstnew(void *content) //testing only
{
	t_list	*node;

	node = malloc(sizeof(t_list) * 1);
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
void	ft_lstadd_back(t_list **lst, t_list *new) //testing only
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
size_t ft_strlen(const char *str) //testing only
{
    unsigned int i;

    i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return ((size_t)i);
}
size_t	ft_strlcpy(char *dst, const char *src, size_t size) //testing only
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
char	*ft_strchr(const char *s, int c) //testing only
{
	unsigned char	uc;

	uc = (unsigned char)c;
	while (*s != '\0')
	{
		if (*s == uc)
			return ((char *)s);
		s++;
	}
	if (uc == '\0')
		return ((char *)s);
	return (NULL);
}
void	*ft_calloc(size_t nmemb, size_t size) //testing only
{
	size_t	i;
	size_t	total;
	char	*p;
	char	*q;
	size_t	size_max;

	size_max = (size_t) - 1;
	if (size && size_max / size < nmemb)
		return (NULL);
	total = nmemb * size;
	p = malloc(total);
	q = p;
	if (p != NULL)
	{
		i = 0;
		while (i < total)
		{
			*p++ = 0;
			i++;
		}
	}
	return ((void *)q);
}
t_list	*ft_lstlast(t_list *lst) //testing only
{
	while (lst != NULL && lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}
void    main(int argc, char **argv, char **env) //testing only
{
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
        i++;
    }
    ft_env(argc, head);
}
*/