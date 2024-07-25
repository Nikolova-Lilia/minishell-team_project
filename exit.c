
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

int	ft_atoi(const char *nptr) //libft function
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