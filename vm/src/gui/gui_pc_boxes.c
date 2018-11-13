/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui_pc_boxes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 00:41:15 by asarandi          #+#    #+#             */
/*   Updated: 2018/11/09 02:09:41 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"

int	gui_mark_pc(t_gui *g, int i, int j)
{
	int i_dst;
	int j_dst;
	int j_save;
	int k;
	int *ptr;

	i = GUI_BLOCK_ARENA_X_POS + (i * GUI_BLOCK_ROW_HEIGHT) - 1;
	i_dst = i + GUI_BLOCK_HEIGHT + 2;
	j = (GUI_BLOCK_ARENA_Y_POS + (j * GUI_BLOCK_COL_WIDTH)) - 1;
	j_dst = j + GUI_BLOCK_WIDTH + 2;
	j_save = j;
	while (i < i_dst)
	{
		j = j_save;
		while (j < j_dst)
		{
			k = (i * g->img_size) + (j * g->img_bpp / CHAR_BIT);
			ptr = (int *)&g->img_data[k];
			*ptr = GUI_PC_BOX_COLOR;
			j++;
		}
		i++;
	}
	return (0);
}

int	gui_pc_boxes(t_gui *g)
{
	t_proc	*p;
	int		idx;

	p = g->core->processes;
	g->nprocesses = 0;
	(void)ft_memset(g->pc_box, 0, MEM_SIZE);
	while (p)
	{
		g->nprocesses++;
		idx = (p->pc - g->core->arena) % MEM_SIZE;
		if (g->pc_box[idx] == 0)
		{
			g->pc_box[idx] = 1;
			(void)gui_mark_pc(g, idx / GBNC, idx % GBNC);
		}
		p = p->next;
	}
	return (0);
}
