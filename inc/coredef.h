/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coredef.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 01:22:46 by sgardner          #+#    #+#             */
/*   Updated: 2018/11/10 02:41:18 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREDEF_H
# define COREDEF_H

/*
** #############################################################################
** # CONFIGURATION - Not easily tuned--it is recommended to leave it alone!    #
** #############################################################################
*/

/*
** Core
*/

# define MEM_SIZE			4096
# define IDX_MOD			(MEM_SIZE / 8)
# define CHAMP_MAX_SIZE		(MEM_SIZE / 6)
# define MAX_PLAYERS		4
# define DUMP_LEN			32

/*
** Lives / Process Culling
*/

# define CYCLE_TO_DIE		1536
# define CYCLE_DELTA		50
# define NBR_LIVE			21
# define MAX_CHECKS			10

/*
** Registers
** Size options are ignored at this time, requiring a system with 16-bit shorts
**  and 32-bit ints. Also, little endianness, as the core is big endian.
*/

# define IND_SIZE			2
# define REG_SIZE			4
# define DIR_SIZE			REG_SIZE
# define REG_NUMBER			16

/*
** Op parameter types
*/

# define T_R				1
# define T_I				2
# define T_D				4

/*
** Champions
*/

# define PROG_NAME_LENGTH	128
# define COMMENT_LENGTH		2048
# define COREWAR_EXEC_MAGIC	0x00EA83F3

/*
** #############################################################################
** # UTILITY                                                                   #
** #############################################################################
*/

# define REV				ft_revbytes
# define ERR				error
# define MSG				notice
# define SYS_ERR			ERR(DEFAULT_ERR, strerror(errno))
# define CORE_POS(x)		(((x) + (MEM_SIZE << 3)) % MEM_SIZE)
# define REL_POS(x)			((x) % IDX_MOD)
# define IDX_POS(ar, pc, x)	((ar) + CORE_POS(((pc) - (ar)) + REL_POS(x)))
# define ABS_POS(ar, pc, x)	((ar) + CORE_POS(((pc) - (ar)) + (x)))

/*
** #############################################################################
** # LANG IDS                                                                  #
** #############################################################################
*/

/*
** Error Messages
*/

enum	e_errmsg
{
	DEFAULT_ERR,
	BAD_IO,
	CHAMP_TOO_LARGE,
	CHAMP_TOO_SMALL,
	INVALID_HEADER,
	NO_PLAYERS,
	SIZE_MISMATCH,
	TOO_MANY_CHAMPS,
	NERRMSGS
};

/*
** Standard Messages
*/

enum	e_notices
{
	ANNOUNCE_WINNER,
	ANNOUNCE_WINNER_TEAM,
	ANNOUNCE_LIVE,
	ANNOUNCE_LOSERS,
	CHAMP_COMMENT,
	LAUNCH_GUI,
	WAR_OVER,
	NNOTICES
};
#endif
