#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_party	t_party;

typedef struct s_philosopher
{
	pthread_t 			thread;
	pthread_mutex_t		*fork_own;
	pthread_mutex_t		*fork_borrowed;
	int					meal_count; // chenge to meals_left?
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
	pthread_t 			monitoring_thread; // TODO
	pthread_mutex_t		*forks;
	pthread_mutex_t		guard;
	pthread_mutex_t		printing; // TODO include everywhere
	pthread_mutex_t		dying;
	pthread_mutex_t		reporting_enough_meals;
	pthread_mutex_t		party_going_on;
	int					someone_dead;
	unsigned int		number_of_philosophers_fed;
}						t_party;

typedef enum e_return_value
{
	SUCCESS,
	ERROR,
}						t_return_value;

t_return_value    parse_args(t_party *party, int argc, char **argv);

#endif
