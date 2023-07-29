

#include "philosophers.h"

/*
 * Converts a string containing an ASCII representation of a positive integer
 * to an unsigned long long value. If value is invalid, return 0.
 */
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
		return (0);
	return ((unsigned long long)result);
}

/**
 * @brief Store and validate an argument for the party configuration.
 *
 * @param party The party struct where the validated value will be stored.
 * @param string The input string containing the argument value.
 * @param argument The index of the argument (1-based).
 * @param argc The total number of command-line arguments.
 * @return SUCCESS if the argument is valid and successfully stored,
	otherwise ERROR.
 */
static t_return_value	store_arg_if_validated(t_party *party, char *string,
		int argument)
{
	int	validated_value;

	validated_value = ascii_to_positive_int(string);
	if (validated_value == 0)
		return (ARG_NOT_NUMERIC);
	else if (argument == 1)
	{
		if (validated_value > MAX_AMOUNT_PHILO_ALLOWED)
			return (TOO_MANY_PHILOS);
		party->number_of_philosophers = validated_value;
	}
	else if (argument == 2)
		party->time_to_die = validated_value * 1000;
	else if (argument == 3)
		party->time_to_eat = validated_value * 1000;
	else if (argument == 4)
		party->time_to_sleep = validated_value * 1000;
	else if (argument == 5)
		party->number_of_meals = (int)validated_value;
	// I don't like that the below is here because it repeats it 4 times
	return (SUCCESS);
}

static void	print_philo_usage(void)
{
	printf("Usage: ./philo\tnumber_of_philosophers\t(max 200 philos)\n\t\ttime_to_die\t\
		(millisecond, ms)\n\t\ttime_to_eat\t\t\t(ms)\n\t\ttime_to_sleep\t\
		(ms)\n\t\t[number_of_times_each_philosopher_must_eat]\n");
}

static t_return_value	argument_number_check(int argc)
{
	if (argc == EXPECT_ARG_COUNT || argc == EXPECT_ARG_COUNT_WITH_MEALS)
		return (SUCCESS);
	printf("Improper amount of arguments. Expected %d or %d arguments\n",
		EXPECT_ARG_COUNT - 1, EXPECT_ARG_COUNT_WITH_MEALS - 1);
	print_philo_usage();
	return (ARG_COUNT_ERROR);
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
	t_return_value	ret_val;
	int				index;

	index = 1;
	ret_val = argument_number_check(argc);
	if (ret_val == ARG_COUNT_ERROR)
		return (ARG_COUNT_ERROR);
	if (argc == 5)
		party->number_of_meals = -1;
	while (index < argc)
	{
		ret_val = store_arg_if_validated(party, argv[index], index);
		if (ret_val != SUCCESS)
		{
			if (ret_val == TOO_MANY_PHILOS)
				printf("Recommended philosopher amount should be less or equal to \
200\n\n");
			else if (ret_val == ARG_NOT_NUMERIC)
				printf("Arguments must be only positive numbers (0 considered not \
valid)\n\n");
			print_philo_usage();
			return (ret_val);
		}
		index++;
	}
	return (SUCCESS);
}
