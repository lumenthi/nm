/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 11:33:39 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/21 15:04:28 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int		sym_cmp(char *s1b, char *s2b)
{
	int i;
	int special1 = 0;
	int special2 = 0;
	char s1_c;
	char s2_c;
	char *s1 = s1b;
	char *s2 = s2b;

	i = 0;
	if (!s1 || !s2)
		return (0);
	/* must ignore '_' char like real nm */
	while (*s1 == '_') {
		s1++;
		special1 = 1;
	}
	while (*s2 == '_') {
		s2++;
		special2 = 1;
	}
	while (s1[i] || s2[i])
	{
		s1_c = ft_tolower(s1[i]);
		s2_c = ft_tolower(s2[i]);
		if (s1_c < s2_c)
			return (s1_c - s2_c);
		if (s1_c > s2_c)
			return (s1_c - s2_c);
		i++;
	}
	/* When same name, nm put the __symbol first */
	if (special1)
		return -1;
	else if (special2)
		return 1;
	return (0);
}

void	sort_symbols(t_symbol **head) {
	t_symbol *current;
	t_symbol *next;
	t_symbol *prev;

	int sorted = 0;
	if (*head == NULL || (*head)->next == NULL)
		return;
	while (!sorted) {
		prev = NULL;
		current = *head;
		next = current->next;
		sorted = 1;
		while (next) {
			if (sym_cmp(current->sym_name, next->sym_name) > 0) {
				if (prev)
					prev->next = next;
				else
					*head = next;
				current->next = next->next;
				next->next = current;
				sorted = 0;
				prev = next;
				current = prev->next;
				next = current->next;
			}
			else {
				prev = current;
				current = next;
				next = next->next;
			}
		}
	}
}

void	t_symbols_display(t_symbol *symbols)
{
	t_symbol *tmp = symbols;

	while (tmp) {
		printf("FULLNAME: %s\nINFO: 0x%x\nVALUE: 0x%lx\nSIZE: 0x%lx\n",\
			tmp->sym_name, tmp->st_info, tmp->st_value, tmp->st_size);
		printf("==========================\n");
		tmp = tmp->next;
	}
}
