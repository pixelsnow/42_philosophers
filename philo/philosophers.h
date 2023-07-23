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
	int					dead;
	int					meal_count;
	unsigned long long	time_last_ate;
	t_party				*party;
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
	pthread_mutex_t		guard;
}						t_party;

typedef enum e_return_value
{
	SUCCESS,
	ERROR,
}						t_return_value;

#endif
