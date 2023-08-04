/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvagapov <vvagapov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 22:12:20 by vvagapov          #+#    #+#             */
/*   Updated: 2023/08/04 22:12:30 by vvagapov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (result > MAX_TIME_ALLOWED)
			return (0);
		str++;
	}
	if (*str != '\0')
		return (0);
	return ((unsigned long long)result);
}

static t_return_value	store_arg_if_validated(t_party *party, char *string,
		int argument)
{
	unsigned long long	validated_value;

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

t_return_value	parse_args(t_party *party, int argc, char **argv)
{
	t_return_value	ret_val;
	int				i;

	i = 1;
	ret_val = argument_number_check(argc);
	if (ret_val == ARG_COUNT_ERROR)
		return (ARG_COUNT_ERROR);
	if (argc == 5)
		party->number_of_meals = -1;
	while (i < argc)
	{
		ret_val = store_arg_if_validated(party, argv[i], i);
		if (ret_val != SUCCESS)
		{
			if (ret_val == TOO_MANY_PHILOS)
				printf("Recommended philosopher amount should be less or equal to \
%d\n\n", MAX_AMOUNT_PHILO_ALLOWED);
			else if (ret_val == ARG_NOT_NUMERIC)
				printf("Arguments must be only positive numbers (0 considered not \
valid), max time allowed is %ld\n\n", MAX_TIME_ALLOWED);
			print_philo_usage();
			return (ret_val);
		}
		i++;
	}
	return (SUCCESS);
}
