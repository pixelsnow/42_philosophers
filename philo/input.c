/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lclerc <lclerc@hive.student.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 20:16:58 by lclerc            #+#    #+#             */
/*   Updated: 2023/07/23 17:10:18 by lclerc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	return (((unsigned long long))result);
}

static t_return_value	validate_digit(char *string)
{
	int	value;
	int	i;

	value = ascii_to_positive_int(string);
	if (value = -42)
		return (ERROR);
}

static t_return_value	argument_number_check(argc)
{
	if (argc == 5 || argc == 6)
		return (SUCCESS);
	printf("Improper amount of argument\n");
	printf("Usage: ./philo number_of_philosophers time_to_die (millisecond, ms) \
	time_to_eat (ms) time_to_sleep(ms) \
	[number_of_times_each_philosopher_must_eat]\n");
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
