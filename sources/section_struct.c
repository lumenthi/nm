/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   section_struct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/26 12:05:08 by lumenthi          #+#    #+#             */
/*   Updated: 2020/02/15 14:07:16 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

// DEBUG FUNCTION
void		print_sections(t_found_section *head) {
	int i = 1;
	while (head) {
		ft_putstr("index: ");
		ft_putnbr(i);
		ft_putstr(", name: ");
		ft_putendl(head->sectname);
		head = head->next;
		i++;
	}
}

static void	append(t_found_section **head, t_found_section *new) {
	t_found_section *tmp = *head;
	if (*head == NULL)
		*head = new;
	else {
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void		load_section(t_found_section **head, void *section, int architecture) {
	struct section_64	*section64 = NULL;
	struct section		*section32 = NULL;
	t_found_section		*new = NULL;

	new = (t_found_section *)malloc(sizeof(t_found_section));
	if (!new) {
		ft_putstr_fd("error: nm: malloc failed\n", STDERR_FILENO);
		return ;
	}
	if (architecture == 64) {
		section64 = (struct section_64 *)section;
		ft_strcpy(new->sectname, section64->sectname);
		ft_strcpy(new->segname, section64->segname);
		new->size = section64->size;
		new->offset = section64->offset;
		new->arch = 64;
	}
	else {
		section32 = (struct section *)section;
		ft_strcpy(new->sectname, section32->sectname);
		ft_strcpy(new->segname, section32->segname);
		new->size = section32->size;
		new->offset = section32->offset;
		new->arch = 32;
	}
	new->next = NULL;
	append(head, new);
}

void		delete_sections(t_found_section **head) {
	t_found_section *current = *head;
	t_found_section *next;
	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
	*head = NULL;
}
