#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/*
typedef enum e_token_type //structure i used to test
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

typedef struct s_token //structure i used to test it
{
    t_token_type type;
    char *str;
} t_token;

typedef struct s_list //structure i used to test it
{
    void *content;
    t_list *next;
} t_list;

int ft_strcmp(const char *s1, const char *s2); //libft function
void ft_strcpy(char *dest, char *src); //libft function
void ft_putstr_fd(char *s, int fd); //libft function
*/

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
/*
int ft_strcmp(const char *s1, const char *s2) //libft function
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

void ft_strcpy(char *dest, char *src) //libft function
{
    while (*src)
    {
        *dest++ = *src++;
    }
    *dest = '\0';
}

void ft_putstr_fd(char *s, int fd) //libft function
{
    if (!s)
        return;
    write(fd, s, ft_strlen(s));
}
*/