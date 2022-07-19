/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 16:03:38 by lumenthi          #+#    #+#             */
/*   Updated: 2020/02/15 14:12:37 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

size_t		hex_len(size_t nb) {
	size_t count = 0;
	if (nb == 0)
		return 1;
	while (nb) {
		nb >>= 4;
		count++;
	}
	return count;
}

static void	display_value(unsigned long value, int bits, int empty) {
	size_t	len;
	size_t	display_len = bits / 4;

	if (empty) {
		while (display_len) {
			ft_putchar(' ');
			display_len--;
		}
		return;
	}
	len = 0;
	len = hex_len(value);
	while (len < display_len) {
		ft_putchar('0');
		len++;
	}
	ft_puthex(0, 0, value);
}

int			display_line(uint8_t *address, uint32_t *i, t_found_section parsed_sections) {
	// TODO CHECK IF I IS NOT AFTER THE FILE SIZE
	uint32_t end = parsed_sections.size;
	uint32_t offset = parsed_sections.offset;
	uint32_t lim = (*i + 16) > end ? end : *i + 16;

	display_value(offset + *i, parsed_sections.arch, 0);
	ft_putchar(' ');
	while (*i < lim) {
		display_value(address[*i], 8, 0);
		ft_putchar(' ');
		(*i)++;
	}
	ft_putchar('\n');
	return *i < end;
}

// OTOOL.C
void		display_section(void *file, char *section, t_found_section *parsed_sections) {
	while (parsed_sections) {
		if (ft_strcmp(section, parsed_sections->sectname) == 0) {
			uint32_t i = 0;
			uint8_t *address = file + parsed_sections->offset;
			ft_putchar('(');
			ft_putstr(section);
			ft_putchar(')');
			ft_putendl(" section");
			while (display_line(address, &i, *parsed_sections));
		}
		parsed_sections = parsed_sections->next;
	}
	return ;
}

static char	get_matching(t_found_section *section, int section_number) {
	char letter ='S';
	int i = 1;

	while (section && i < section_number) {
		section = section->next;
		i++;
	}
	if (section) {
		if (!ft_strcmp(section->sectname, "__text"))
			letter = 'T';
		else if (!ft_strcmp(section->sectname, "__bss"))
			letter = 'B';
		else if (!ft_strcmp(section->sectname, "__data"))
			letter = 'D';
	}
	return letter;
}

static char	get_letter(t_parsed_symbol symbol, t_found_section *section) {
	char	letter = 'S';
	int		type;

	// ft_puthex(1, 0, symbol.n_type & N_TYPE);
	// ft_putchar(' ');
	if ((type = (symbol.n_type & N_TYPE)) == N_SECT) // THEN SYMBOL IS DEFINED IN A SECTION
		letter = get_matching(section, symbol.n_sect);
	else if (type == N_UNDF) { // UNDEFINED
		if (symbol.n_value) // COMMON
			letter = 'C';
		else
			letter = 'U';
	}
	else {
		if (type == N_PBUD) // PRIVATE EXTERNAL
			letter = 'U';
		else if (type == N_INDR) // INDIRECT
			letter = 'I';
		else if (type == N_ABS) // ABSOLUTE
			letter = 'A';
		else
			letter = '?';
	}
	if (letter != '?' && !(symbol.n_type & N_EXT)) {
		letter += 32;
	}
	return(letter);
}

void		display_symbols(t_parsed_symbol *symbol, t_found_section *section) {
	char letter;
	while (symbol) {
		// PRINT VALUE, NEED LEADING ZEROES (64 bits = 16hex) 32 bits ????
		letter = get_letter(*symbol, section);
		if (letter == 'U' || letter == 'u' || letter == 'i' || letter == 'I')
			display_value(symbol->n_value, symbol->arch, 1);
		else
			display_value(symbol->n_value, symbol->arch, 0);
		ft_putchar(' ');
		ft_putchar(letter);
		ft_putchar(' ');
		ft_putstr(symbol->name);
		if (letter == 'I' || letter == 'i')
			ft_putstr(" (indirect for bad string index)");
		ft_putchar('\n');
		symbol = symbol->next;
	}
}
