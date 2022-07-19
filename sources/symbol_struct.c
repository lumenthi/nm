/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/26 15:53:10 by lumenthi          #+#    #+#             */
/*   Updated: 2020/02/12 15:02:48 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	symbols_sort_alpha(t_parsed_symbol **head) {
	t_parsed_symbol *current;
	t_parsed_symbol *next;
	t_parsed_symbol *prev;

	int sorted = 0;
	if (*head == NULL || (*head)->next == NULL)
		return;
	while (!sorted) {
		prev = NULL;
		current = *head;
		next = current->next;
		sorted = 1;
		while (next) {
			if (ft_strcmp(current->name, next->name) > 0) {
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

void	delete_symbols(t_parsed_symbol **head) {
	t_parsed_symbol *current = *head;
	t_parsed_symbol *next;
	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
	*head = NULL;
}

void	push_symbol(t_parsed_symbol **head, t_parsed_symbol *new) {
	new->next = *head;
	*head = new;
}
