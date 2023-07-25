#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>

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
	pthread_mutex_t		guard;
	pthread_mutex_t		printing; // TODO include everywhere
	pthread_mutex_t		dying; // someone_dead
	pthread_mutex_t		reporting_enough_meals; // number_of_philosophers_fed
	pthread_mutex_t		party_going_on; // holds main thread
	unsigned int		someone_dead;
	unsigned int		number_of_philosophers_fed;
}						t_party;

typedef enum e_return_value
{
	SUCCESS,
	ERROR,
}						t_return_value;

/**
 * Contained in input.c 
 */
t_return_value		parse_args(t_party *party, int argc, char **argv);

/**
 * Contained in utils.c 
 */
// int					parse_args_old(t_party	*party, int ac, char **av); // old
int					quit_gracefully(t_party	*party); // useless now
unsigned long long	get_current_time(void);
void				print_whats_happening(t_philosopher *philosopher, char *event);

/**
 * Contained in party_preparation
 */
int					prepare_party(t_party *party);

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


void				clean_up(t_party	*party); // Am I blind or where is that function? : ) 

#endif
