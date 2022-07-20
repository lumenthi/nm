/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 11:33:39 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/20 12:43:49 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	display_symbols(t_section *symbols)
{
	t_section *tmp = symbols;

	while (tmp) {
		printf("FULLNAME: %s\nNAME: %d\nTYPE: %d\nFLAGS: 0x%x\nARCH: %d\n",\
			tmp->sym_name, tmp->sh_name, tmp->sh_type, tmp->sh_flags, tmp->arch);
		printf("==========================\n");
		tmp = tmp->next;
	}
}
