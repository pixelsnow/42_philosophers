/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvagapov <vvagapov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 19:40:19 by vvagapov          #+#    #+#             */
/*   Updated: 2023/07/22 20:10:18 by vvagapov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdlib.h>

int	parse_args(t_party	*party, int ac, char **av)
{
	party->number_of_philosophers = atoi(av[1]);
	party->time_to_die = atoi(av[2]);
	party->time_to_eat = atoi(av[3]);
	party->time_to_sleep = atoi(av[4]);
	if (ac > 5)
		party->number_of_times_each_philosopher_must_eat = atoi(av[5]);
	return (0);
}

void	monitor_party(t_party	*party)
{
	(void) party;
}

int	main(int ac, char **av)
{
	t_party	party;
	
	if (parse_args(&party, ac, av))
	{
		return (ERROR);
	}
	monitor_party(&party);
	return(SUCCESS);
}
