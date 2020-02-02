/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_thpool.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 10:56:56 by pacovali      #+#    #+#                 */
/*   Updated: 2019/10/28 10:56:56 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_THPOOL_H
# define __FT_THPOOL_H

# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

typedef struct		s_thread
{
	size_t			id;
	pthread_t		thread;
}					t_thread;

typedef struct		s_job
{
	void			(*fun)(void*);
	void			*data;
	struct s_job	*next;
}					t_job;

typedef struct		s_pool
{
	t_thread		**thr;
	t_job			*first;
	t_job			*last;
	pthread_cond_t	work_exist;
	pthread_cond_t	threads_idle;
	pthread_mutex_t	lock;
	size_t			total_threads;
	size_t			working_threads;
	size_t			stop;
}					t_pool;

t_pool				*tp_create(size_t threads_amount);
void				tp_destroy(t_pool *pool);
void				*tp_worker(void *args);
t_job				*tp_job_create(void (*fun)(void*), void *arg);
void				tp_job_destroy(t_job **job);
t_job				*tp_get_next_job(t_pool *pool);
size_t				tp_get_nbr_proc(void);
void				tp_wait_for_queue(t_pool *pool);
void				tp_exec_queue_add(t_pool *pool, void (*fun)(void*),
									void *arg);

#endif
