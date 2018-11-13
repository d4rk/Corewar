/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_lldi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/27 23:54:54 by sgardner          #+#    #+#             */
/*   Updated: 2018/11/04 19:41:21 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

t_bool	op_lldi(t_core *core, t_proc *p, t_instr *instr)
{
	t_byte	*src;
	int32_t	*dst;
	int32_t	off;

	dst = (int32_t *)instr->args[2];
	if (instr->atypes[0] & T_I)
	{
		off = read_core(core, instr->args[0], IND_SIZE, FALSE);
		src = IDX_POS(core->arena, p->pc, off);
		off = read_core(core, src, DIR_SIZE, FALSE);
	}
	else
		off = read_arg(core, p, 0);
	off += read_arg(core, p, 1);
	*dst = read_core(core, ABS_POS(core->arena, p->pc, off), REG_SIZE, FALSE);
	return (!*dst);
}
