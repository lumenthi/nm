/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 11:50:22 by lumenthi          #+#    #+#             */
/*   Updated: 2020/02/15 14:07:33 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

// FOR DEBUG (printf), remove after
#include <stdio.h>
//

# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <unistd.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/swap.h>
# include <mach-o/fat.h>
# include <ar.h>

# include "libft.h"

typedef struct					s_found_section {
		char					sectname[16];
		char					segname[16];
		unsigned long			offset;
		unsigned long			size;
		int						arch;
		struct s_found_section	*next;
}								t_found_section;

typedef struct					s_parsed_symbol {
		char					*name;
		unsigned char			n_type;
		unsigned char			n_sect;
		unsigned long			n_value;
		int						arch;
		struct s_parsed_symbol	*next;
}								t_parsed_symbol;

// SECTIONS_STRUCT.C
void		load_section(t_found_section **head, void *section, int architecture);
void		print_sections(t_found_section *head);
void		delete_sections(t_found_section **head);

// SYMBOLS_STRUCT.C
void		push_symbol(t_parsed_symbol **head, t_parsed_symbol *new);
void		delete_symbols(t_parsed_symbol **symbol);
void		symbols_sort_alpha(t_parsed_symbol **head);

// DISPLAY.C
void		display_symbols(t_parsed_symbol *symbol, t_found_section *section);
void		display_section(void *file, char *section, t_found_section *parsed_sections);

// MACHO.C
void		handle_macho(void *file, t_parsed_symbol **parsed_symbols,
				t_found_section **parsed_sections, int architecture);

// ARCHIVE.C
void		handle_archive(char *arg, void *file, int cmd);

// FAT.C
void		handle_fat(char *arg, void *file, int cmd);

// SWAP.C
uint16_t	swap_uint16(uint16_t nb);
uint32_t	swap_uint32(uint32_t nb);

// SEGMENT.C
void		handle_segment64(int swap, void *cmd, t_found_section **parsed_sections);
void		handle_segment32(int swap, void *cmd, t_found_section **parsed_sections);

// SYMBOL.C
void	load_symbols64(t_parsed_symbol **parsed_symbols, struct nlist *symtab,
			char *strtab, struct symtab_command symcmd, int swap);
void	load_symbols32(t_parsed_symbol **parsed_symbols, struct nlist *symtab,
			char *strtab, struct symtab_command symcmd, int swap);

// TOOLS.C
void	*map_file(char *path, size_t *size);
int		iterate_commands(char *arg, struct mach_header *header, int cmd);
															// 1 = NM, 2 = OTOOL

#endif
