

#include "philosophers.h"

static unsigned long long	ascii_to_positive_int(const char *str)
{
	unsigned long long	result;

	result = 0;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	if (*str != '\0')
		return (-42);
	return ((unsigned long long)result);
}

static t_return_value	store_arg_if_validated(t_party *party, char *string,
		int argument)
{
	int	validated_value;

	validated_value = ascii_to_positive_int(string);
	if (validated_value == -42)
		return (ERROR);
	else if (argument == 1)
		party->number_of_philosophers = validated_value;
	else if (argument == 2)
		party->time_to_die = validated_value;
	else if (argument == 3)
		party->time_to_eat = validated_value;
	else if (argument == 4)
		party->time_to_sleep = validated_value;
	else if (argument == 5)
		party->number_of_meals_needed = (int)validated_value;
	return (SUCCESS);
}

static void print_philo_usage(void)
{
	printf("Usage: ./philo\tnumber_of_philosophers\n\t\ttime_to_die\t(millisecond, ms)\n\t\ttime_to_eat\t(ms)\n\t\ttime_to_sleep\t(ms)\n\t\t[number_of_times_each_philosopher_must_eat]\n");
}

static t_return_value	argument_number_check(argc)
{
	if (argc == 5 || argc == 6)
		return (SUCCESS);
	printf("Improper amount of argument\n");
	print_philo_usage();
	return (ERROR);
}

/**


	* @brief Check if number of arguments is correct and are positive numerical values
 * 
 * @param party Struct organizing the party
 * @param argc
 * @param argv 
 * @return	SUCCESS or ERROR
 */
t_return_value	parse_args(t_party *party, int argc, char **argv)
{
	int	index;

	index = 1;
	if (argument_number_check(argc) == ERROR)
		return (ERROR);
	while (index < argc)
	{
		if (store_arg_if_validated(party, argv[index], index) == ERROR &&
			argv[index] != '\0')
		{
			printf("Argument must be positive number\n");
			print_philo_usage();
			return (ERROR);
		}
		index++;
	}
	return (SUCCESS);
}
