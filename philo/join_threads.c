/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvagapov <vvagapov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 22:10:25 by vvagapov          #+#    #+#             */
/*   Updated: 2023/08/03 04:27:08 by vvagapov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_return_value	join_philosopher_threads(t_party *party, unsigned int last_index)
{
	unsigned int	i;

	i = 0;
	while (i < last_index)
	{
		if (pthread_join(party->philosophers[i].thread, NULL) != SUCCESS)
		{
			//printf("Failed to join philosopher thread %u\n", i + 1);
			return (JOIN_FAIL);
		}
		//printf("Joined philosopher thread %u\n", i + 1);
		i++;
	}
	return (SUCCESS);
}

t_return_value	join_monitoring_thread(t_party *party)
{
	if (pthread_join(party->monitoring_thread, NULL) != SUCCESS)
	{
		//printf("Failed to join monitoring thread\n");
		return (JOIN_FAIL);
	}
	//printf("Joined monitoring thread\n");
	return (SUCCESS);
}
