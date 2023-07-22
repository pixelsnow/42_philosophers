/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lclerc <lclerc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 17:40:02 by lclerc            #+#    #+#             */
/*   Updated: 2023/07/22 17:57:06 by lclerc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_shared_data	t_shared_data;

typedef struct s_philo
{
	int					fork_own;
	int					fork_borrowed;
	int					meal_count;
	pthread_t 			philo_thread;
	unsigned long long	time_last_ate;
	
}						t_philo;


typedef enum e_return_value
{
	ERROR,
	SUCCESS,
}						t_return_value;

