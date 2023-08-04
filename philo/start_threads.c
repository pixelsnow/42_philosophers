/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvagapov <vvagapov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 22:13:02 by vvagapov          #+#    #+#             */
/*   Updated: 2023/08/04 21:12:36 by vvagapov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_return_value	start_philosopher(t_party	*party, unsigned int i)
{
	party->philosophers[i].index = i;
	if (pthread_create(&(party->philosophers[i].thread), NULL, \
		philosopher_routine, (void *)&(party->philosophers[i])) != 0)
		return (THREAD_FAIL);
	return (SUCCESS);
}

t_return_value	start_philosophers(t_party *party)
{
	unsigned int	i;

	i = 0;
	while (i < party->number_of_philosophers)
	{
		if (start_philosopher(party, i) == THREAD_FAIL)
			return (THREAD_FAIL);
		i++;
	}
	return (SUCCESS);
}

t_return_value	start_monitoring(t_party	*party)
{
	if (pthread_create(&(party->monitoring_thread), NULL, \
		monitoring_routine, (void *)party) != 0)
		return (THREAD_FAIL);
	return (SUCCESS);
}
