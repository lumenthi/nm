/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 12:01:16 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/27 16:46:39 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

uint16_t	swap_uint16(uint16_t nb, int swap)
{
	if (!swap)
		return nb;
	nb = (nb << 8) | (nb >> 8);
	return (nb);
}

uint32_t	swap_uint32(uint32_t nb, int swap)
{
	if (!swap)
		return nb;
	nb = ((nb & 0x000000FF) << 24 |
				(nb & 0x0000FF00) << 8 |
				(nb & 0x00FF0000) >> 8 |
				(nb & 0xFF000000) >> 24);
	return (nb);
}
