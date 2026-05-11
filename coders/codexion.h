/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:12:46 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/11 18:01:01 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

# define POLICY_FIFO 0
# define POLICY_EDF 1

// priority queue node
typedef struct s_pq_node
{
	int		coder_id;
	long	priority;
}	t_pq_node;

// binary min-heap
typedef struct s_heap
{
	t_pq_node	*data;
	int			size;
	int			capacity;
}	t_heap;

// forward declaration
typedef struct s_sim	t_sim;

// dongle
typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				taken;
	long			release_time;
	t_heap			wait_queue;
}	t_dongle;

// coder
typedef struct s_coder
{
	int			id;
	pthread_t	thread;
	int			left_dongle;
	int			right_dongle;
	long		last_compile_start;
	int			compile_count;
	long		seq_num;
	t_sim		*sim;
}	t_coder;

// simulation
struct	s_sim
{
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	long			dongle_cooldown;
	int				scheduler;
	long			start_ms;
	int				stopped;
	long			seq_counter;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	log_mutex;
	pthread_t		monitor_thread;
	t_coder			*coders;
	t_dongle		*dongles;

};

// parsing.c
int			parse_args(int argc, char **argv, t_sim *sim);

// init.c
int			init_sim(t_sim *sim);

// init_dongles.c
int			init_dongles(t_sim *sim);
void		destroy_all_dongles(t_sim *sim);

// coder.c
void		*coder_routine(void *arg);

// dongle.c
int			acquire_dongle(t_dongle *dongle, t_coder *coder);
void		release_dongle(t_dongle *dongle, t_sim *sim);

// monitor.c
void		*monitor_routine(void *arg);

// heap.c
int			heap_init(t_heap *h, int capacity);
void		heap_push(t_heap *h, t_pq_node node);
t_pq_node	heap_pop(t_heap *h);
t_pq_node	heap_peek(t_heap *h);
int			heap_is_empty(t_heap *h);
void		heap_free(t_heap *h);

// time_utils.c
long		get_current_ms(void);
long		get_timestamp_ms(long start_ms);
void		precise_sleep(long duration_ms, t_sim *sim);

// log.c
void		log_msg(t_sim *sim, int coder_id, char *msg);
void		log_burnout(t_sim *sim, int coder_id);

// cleanup.c
void		cleanup_sim(t_sim *sim);

// sim_utils.c
int			simulation_stopped(t_sim *sim);
void		set_sim_stopped(t_sim *sim);
long		get_coder_deadline(t_coder *coder);
long		next_seq_num(t_sim *sim);

#endif