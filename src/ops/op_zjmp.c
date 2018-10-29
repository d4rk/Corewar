/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_zjmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/27 20:24:30 by sgardner          #+#    #+#             */
/*   Updated: 2018/10/29 04:19:50 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

t_bool	op_zjmp(t_core *core, t_proc *p)
{
	t_instr	*instr;
	t_uint	off;

	instr = &p->instr;
	if (p->carry)
	{
		off = read_core(core, instr->epc, DIR_SIZE, TRUE);
		instr->epc = IDX_POS(core->arena, p->pc, off);
	}
	else
		instr->epc = ABS_POS(core->arena, instr->epc, IND_SIZE);
	return (p->carry);
}
