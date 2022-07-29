/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 11:33:39 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/29 16:37:27 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

/* HELL sorting functions to match NM output */

static int		sym_cmp(t_symbol *s1b, t_symbol *s2b, t_info infos)
{
	int i;
	int j;
	char s1_c;
	char s2_c;
	char *s1 = s1b->sym_name;
	char *s2 = s2b->sym_name;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (0);
	/* must ignore '_' char like real nm */
	while (*s1 == '_' || *s1 == '.')
		s1++;
	while (*s2 == '_' || *s2 == '.')
		s2++;
	while (s1[i] || s2[j]) {
		while (s1[i] && (s1[i] == '_' || s1[i] == '@'))
			i++;
		while (s2[j] && (s2[j] == '_' || s2[j] == '@'))
			j++;
		s1_c = ft_tolower(s1[i]);
		s2_c = ft_tolower(s2[j]);
		if (s1_c < s2_c)
			return (s1_c - s2_c);
		if (s1_c > s2_c)
			return (s1_c - s2_c);
		i++;
		j++;
	}
	(void)infos;
	/* This is horrible but it matches the nm algo */
	if (s1b->st_value == s2b->st_value)
		return ft_strcmp(s1b->sym_name, s2b->sym_name);
	return ft_strcmp(s2b->sym_name, s1b->sym_name);
}

void	sort_symbols(t_symbol **head, t_info infos)
{
	t_symbol *current;
	t_symbol *next;
	t_symbol *prev;
	int value = 0;
	int sorted = 0;

	if (infos.args & 0x01)
		return;

	if (*head == NULL || (*head)->next == NULL)
		return;
	while (!sorted) {
		prev = NULL;
		current = *head;
		next = current->next;
		sorted = 1;
		while (next) {
			value = infos.args & 0x02 ?
				sym_cmp(next, current, infos):
				sym_cmp(current, next, infos);
			if (value == 0)
				value = current->st_value - next->st_value;
			if (value > 0) {
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

void	append_symbol(t_symbol **head, t_symbol *new)
{
	t_symbol *tmp = *head;
	if (*head == NULL)
		*head = new;
	else {
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	free_symbols(t_symbol **symbols)
{
	t_symbol *current = *symbols;
	t_symbol *next;
	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
	*symbols = NULL;
}
