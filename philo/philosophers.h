#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>

# define EXPECT_ARG_COUNT 5
# define EXPECT_ARG_COUNT_WITH_MEALS 6
# define MAX_AMOUNT_PHILO_ALLOWED 200


typedef struct s_party	t_party;

typedef struct s_philosopher
{
	pthread_t 			thread;
	pthread_mutex_t		*fork_own;
	pthread_mutex_t		*fork_borrowed;
	int					meal_count; // change to meals_left maybe? that would eliminate the need to reach for party->number_of_meals_needed every time. idk
	unsigned int		index;
	unsigned long long	time_last_ate;
	pthread_mutex_t		meal_update; // protects meal_count and time_last_ate
	t_party				*party;
}						t_philosopher;

typedef struct s_party
{
	unsigned int		number_of_philosophers;
	unsigned long long	time_to_die;
	unsigned long long	time_to_eat;
	unsigned long long	time_to_sleep;
	unsigned long long 	party_start_time;
	int					number_of_meals;
	t_philosopher		*philosophers;
	pthread_t 			monitoring_thread;
	pthread_mutex_t		*forks;
	pthread_mutex_t		guard; // INITIALIZED
	pthread_mutex_t		printing; // TODO include everywhere
	pthread_mutex_t		dying; // someone_dead
	pthread_mutex_t		reporting_enough_meals; // number_of_philosophers_fed
	unsigned int		someone_dead;
	unsigned int		number_of_philosophers_fed;
}						t_party;


typedef enum e_return_value
{
	SUCCESS,
	ERROR,
	ARG_COUNT_ERROR,
	ARG_NOT_NUMERIC,
	TOO_MANY_PHILOS,
	MALLOC_FAIL,
	MUTEX_FAIL,
	THREAD_FAIL,
	JOIN_FAIL,
	SINGLE_PHILO_CASE,
	LIFE_GOES_ON,
	SOMEONE_DIED,
	EVERYONE_IS_FED,
}						t_return_value;

/**
 * Contained in input.c 
 */
t_return_value		parse_args(t_party *party, int argc, char **argv);

/**
 * Contained in utils.c 
 */
unsigned long long	get_current_time(void);
void				custom_usleep(unsigned long long duration, t_party *party);
void				print_whats_happening(t_philosopher *philosopher, char *event);

/**
 * Contained in party_preparation
 */
t_return_value		prepare_party(t_party *party);

/**
 * Contained in start_threads.c
 */
t_return_value		start_philosopher(t_party	*party, unsigned int i);
t_return_value		start_monitoring(t_party	*party);

/**
 * Contained in philo_routine.c
 */
void				*philosopher_routine(void *philosopher_data);

/**
 *	Contained in monitoring_routine.c
 */
void				*monitoring_routine(void *party_data);


void				clean_up(t_party	*party); 

#endif
