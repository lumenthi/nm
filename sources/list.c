/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 11:33:39 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/20 19:15:59 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	display_symbols(t_symbol *symbols)
{
	t_symbol *tmp = symbols;

	while (tmp) {
		printf("FULLNAME: %s\nINFO: 0x%x\nVALUE: 0x%lx\nSIZE: 0x%lx\n",\
			tmp->sym_name, tmp->st_info, tmp->st_value, tmp->st_size);
		printf("==========================\n");
		tmp = tmp->next;
	}
}
