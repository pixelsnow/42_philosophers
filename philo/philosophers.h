/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvagapov <vvagapov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 17:40:02 by lclerc            #+#    #+#             */
/*   Updated: 2023/07/22 20:07:32 by vvagapov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_party	t_party;

typedef struct s_philosopher
{
	int					meal_count;
	pthread_t 			philo_thread;
	unsigned long long	time_last_ate;
}						t_philosopher;

typedef struct s_party
{
	unsigned int		number_of_philosophers;
	unsigned long long	time_to_die;
	unsigned long long	time_to_eat;
	unsigned long long	time_to_sleep;
	unsigned int		number_of_times_each_philosopher_must_eat;
	t_philosopher		*philosophers;
	pthread_mutex_t		*forks;
}						t_party;

typedef enum e_return_value
{
	SUCCESS,
	ERROR,
}						t_return_value;

#endif