#include <stdio.h>
#include <unistd.h>

/*
        ◦ pwd with no options

        The ‘pwd,’ which stands for “print working directory.” It prints
        the path of the working directory,
        starting from the root. pwd is shell
        built-in command(pwd) or an actual binary(/bin/pwd).

        This command doesn’t have any arguments or options,
        accepts flags (ignore for the assignment).


        The getcwd() function copies an absolute pathname of the current
        working directory to the array pointed to by buf, which is of
        length size.

        If the length of the absolute pathname of the current working
        directory, including the terminating null byte, exceeds size
        bytes, NULL is returned, and errno is set to ERANGE; an
        application should check for this error, and allocate a larger
        buffer if necessary.
    */

int ft_pwd(void);
static size_t ft_strlen(const char *str); //libft function
static void ft_putstr_fd(char *s, int fd); //libft fuunction

int main(void)
{
    ft_pwd();
    printf("ok\n");
}

//write a check to see if the space is enough, if no include realloc(maybe not necessary for the scope of this project)
// https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-getcwd-get-path-name-working-directory
int ft_pwd(void)
{
    char cwd[256];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror("getcwd() error");
    else
    {
        ft_putstr_fd(cwd, 1);
        ft_putstr_fd("\n", 1);
    }
}

static size_t ft_strlen(const char *str)
{
    unsigned int i;

    i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return ((size_t)i);
}

static void ft_putstr_fd(char *s, int fd)
{
    if (!s)
        return;
    write(fd, s, ft_strlen(s));
}