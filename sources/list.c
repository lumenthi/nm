/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 11:33:39 by lumenthi          #+#    #+#             */
/*   Updated: 2022/07/28 11:33:12 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

static int		sym_cmp(char *s1b, char *s2b)
{
	int i;
	int j;
	char s1_c;
	char s2_c;
	char *s1 = s1b;
	char *s2 = s2b;

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
	return (ft_strcmp(s1b, s2b));
}

void	sort_symbols(t_symbol **head)
{
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
