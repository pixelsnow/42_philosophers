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
	t_party				*party;
}						t_philosopher;

typedef struct s_party
{
	unsigned int		number_of_philosophers;
	unsigned long long	time_to_die;
	unsigned long long	time_to_eat;
	unsigned long long	time_to_sleep;
	int					number_of_meals_needed;
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

t_return_value    parse_args(t_party *party, int argc, char **argv);

int					parse_args_old(t_party	*party, int ac, char **av); // old
int					quit_gracefully(t_party	*party); // useless now
unsigned long long	get_current_time(void);

int					prepare_party(t_party *party);
void				start_philosopher(t_party	*party, unsigned int i);
void				start_monitoring(t_party	*party);
void				*philosopher_routine(void *philosopher_data);
void				*monitoring_routine(void *party_data);

void				clean_up(t_party	*party);

#endif
