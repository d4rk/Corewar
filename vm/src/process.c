/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 00:30:56 by sgardner          #+#    #+#             */
/*   Updated: 2019/01/25 01:53:18 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include <stdlib.h>

/*
** If live has been called at least NBR_LIVE times since the last check, or
**  MAX_CHECKS checks have happened, CYCLE_TO_DIE is reduced by CYCLE_DELTA.
*/

static void	schedule_culling(t_core *core, t_cull *cull)
{
	int	i;

	if (++cull->checks >= MAX_CHECKS || cull->nbr_lives >= NBR_LIVE)
	{
		cull->checks = 0;
		cull->ctd = (cull->ctd > CYCLE_DELTA) ? cull->ctd - CYCLE_DELTA : 0;
		i = core->nplayers;
		while (i--)
			core->champions[i].plives = 0;
		cull->plives = 0;
	}
	cull->nbr_lives = 0;
	cull->ccycle = core->cycle + cull->ctd;
}

/*
** Frees up unused memory in the process pool.
*/

static void	drain_pool(t_procpool *pool)
{
	pool->maxsize /= 2;
	if (!(pool->procs = realloc(pool->procs, sizeof(t_proc) * pool->maxsize)))
		SYS_ERR;
}

/*
** Shifts all living processes to a contiguous memory section.
*/

static void	defrag_pool(t_procpool *pool, t_uint start, t_uint killed)
{
	t_proc	*procs;
	t_uint	end;

	end = start + 1;
	procs = pool->procs;
	while (end < pool->size)
	{
		while (start < pool->size && procs[start].pc)
			++start;
		if (end < start)
			end = start + 1;
		while (end < pool->size && !procs[end].pc)
			++end;
		while (end < pool->size && procs[end].pc && !procs[start].pc)
		{
			procs[start++] = procs[end];
			procs[end++].pc = NULL;
		}
	}
	pool->size -= killed;
	if (pool->size > 32 && pool->size <= (pool->maxsize / 3))
		drain_pool(pool);
}

/*
** Kills processes that have not yet called the live instruction, and schedules
**  the next culling.
*/

void		cull_processes(t_core *core, t_cull *cull)
{
	t_proc	*p;
	t_uint	start;
	t_uint	killed;
	t_uint	i;

	start = 0;
	killed = 0;
	i = core->procpool.size;
	while (i--)
	{
		p = &core->procpool.procs[i];
		if (core->cycle - p->lcycle >= cull->ctd)
		{
			start = i;
			p->pc = NULL;
			++killed;
		}
	}
	if (killed)
		defrag_pool(&core->procpool, start, killed);
	schedule_culling(core, cull);
}

/*
** Spawns a new process and adds it to the "front" of the execution list.
** Instruction to execute starts with a NOP to begin execution on next cycle.
** If process is a fork of another, all data (except for the PC and pid) is
**  copied to the child process from the parent process.
**
** WARNING: Resizing the process pool may cause the parent process to shift to
**  a new memory address.
*/

t_proc		*fork_process(t_core *core, t_proc *p, t_byte *fpc)
{
	static t_uint	lpid;
	t_procpool		*pool;
	t_proc			*clone;

	pool = &core->procpool;
	if (!pool->procs || pool->size == pool->maxsize)
	{
		clone = pool->procs;
		pool->maxsize = (pool->maxsize) ? pool->maxsize << 1 : 32;
		pool->procs = realloc(pool->procs, sizeof(t_proc) * pool->maxsize);
		if (!pool->procs)
			SYS_ERR;
		ft_memset(pool->procs + pool->size, 0,
			sizeof(t_proc) * (pool->maxsize - pool->size));
		if (p)
			p = pool->procs + (p - clone);
	}
	clone = &pool->procs[pool->size++];
	if (p)
		ft_memcpy(clone, p, sizeof(t_proc));
	clone->pc = fpc;
	clone->pid = ++lpid;
	clone->instr.op = NOP;
	return (clone);
}
