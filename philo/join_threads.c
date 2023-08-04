/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvagapov <vvagapov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 22:10:25 by vvagapov          #+#    #+#             */
/*   Updated: 2023/08/04 21:14:04 by vvagapov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_return_value	join_philosopher_threads(t_party *party)
{
	unsigned int	i;

	i = 0;
	while (i < party->number_of_philosophers)
	{
		if (pthread_join(party->philosophers[i].thread, NULL) != SUCCESS)
			return (JOIN_FAIL);
		i++;
	}
	return (SUCCESS);
}

t_return_value	join_monitoring_thread(t_party *party)
{
	if (pthread_join(party->monitoring_thread, NULL) != SUCCESS)
		return (JOIN_FAIL);
	return (SUCCESS);
}
