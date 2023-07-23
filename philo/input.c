#include "philosophers.h"

int	ft_atoi(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while ((*str == 32) || ((*str >= 9) && (*str <= 13)))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '+' || *str == '-')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	return ((int)(result * sign));
}
t_return_value	validate_digit(char *string)
{

}

t_return_value	argument_number_check(argc)
{
	if (argc == 4 || argc == 5)
		return (SUCCESS);
	printf("Improper amount of argument\n");
	printf("Usage: ./philo number_of_philosophers time_to_die (millisecond, ms) \
	time_to_eat (ms) time_to_sleep(ms) \
	[number_of_times_each_philosopher_must_eat]\n");
	return (ERROR);
}

t_return_value	parse_args(t_party *party, int argc, char **argv)
{
	int	i;

	i = 1;
	if (argument_number_check(argc) == SUCCESS)
	{
		while (i < argc)
		{
			if (validate_digit(argv[i]) != SUCCESS && argv[i] != '\0')
			{
				printf("Argument must be positive number\n");
				return (ERROR);
			}
		i++;
		}
		return (SUCCESS);
	}
	return(ERROR);
}
