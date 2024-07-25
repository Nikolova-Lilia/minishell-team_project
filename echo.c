#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/* created only for testing purposes

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

*/

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

int ft_check_n(char *str)
{
    if (str[0] == '-' && str[1] == 'n')
        return (1);

    return (0);
}

void ft_putstr_fd(char *s, int fd) //libft fuction
{
    if (!s)
        return;
    write(fd, s, ft_strlen(s));
}