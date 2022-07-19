/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 21:13:16 by lumenthi          #+#    #+#             */
/*   Updated: 2020/02/12 15:02:43 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
 * This is the symbol table entry structure for 32-bit architectures.
 */
/*struct nlist {*/
	/*union {*/
		/*char			*n_name;	[> for use when in-core <]*/
		/*long			n_strx;		[> index into the string table <]*/
	/*}					n_un;		[> union name <]*/
	/*unsigned char		n_type;		[> type flag, see below <]*/
	/*unsigned char		n_sect;		[> section number or NO_SECT <]*/
	/*short				n_desc;		[> see <mach-o/stab.h> <]*/
	/*unsigned long		n_value;	[> value of this symbol (or stab offset) <]*/
/*};*/
/*
 * This is the symbol table entry structure for 64-bit architectures.
 */
/*struct nlist_64 {*/
	/*union {*/
		/*uint32_t		n_strx;		[> index into the string table <]*/
	/*}					n_un;		[> union name <]*/
	/*uint8_t			n_type;		[> type flag, see below <]*/
	/*uint8_t			n_sect;		[> section number or NO_SECT <]*/
	/*uint16_t			n_desc;		[> see <mach-o/stab.h> <]*/
	/*uint64_t			n_value;	[> value of this symbol (or stab offset) <]*/
/*};*/

// MASKS FOR n_type;

/*#define	N_STAB	0xe0  [> if any of these bits set, a symbolic debugging entry <]*/
/*#define	N_PEXT	0x10  [> private external symbol bit <]*/
/*#define	N_TYPE	0x0e  [> mask for the type bits <]*/
/*#define	N_EXT	0x01  [> external symbol bit, set for external symbols <]*/

// Values for N_TYPE bits of the n_type field

/*#define	N_UNDF	0x0		[> undefined, n_sect == NO_SECT <]*/
/*#define	N_ABS	0x2		[> absolute, n_sect == NO_SECT <]*/
/*#define	N_SECT	0xe		[> defined in section number n_sect <]*/
/*#define	N_PBUD	0xc		[> prebound undefined (defined in a dylib) <]*/
/*#define	N_INDR	0xa		[> indirect <]*/

// SYMTAB COMMAND

/*struct symtab_command {*/
	/*unsigned long	cmd;			[> LC_SYMTAB <]*/
	/*unsigned long	cmdsize;		[> sizeof(struct symtab_command) <]*/
	/*unsigned long	symoff;			[> symbol table offset <]*/
	/*unsigned long	nsyms;			[> number of symbol table entries <]*/
	/*unsigned long	stroff;			[> string table offset <]*/
	/*unsigned long	strsize;		[> string table size in bytes <]*/
/*};*/

void	load_symbols32(t_parsed_symbol **parsed_symbols, struct nlist *symtab, char *strtab, struct symtab_command symcmd, int swap) {
	struct nlist	*tmp;
	tmp = (struct nlist *)symtab;
	if (swap) {
		symcmd.nsyms = swap_uint32(symcmd.nsyms);
		symcmd.strsize = swap_uint32(symcmd.strsize);
	}
	while (symcmd.nsyms) {
		struct nlist tmp_values = *tmp;
		if (swap) {
			tmp_values.n_value = swap_uint32(tmp_values.n_value);
			tmp_values.n_un.n_strx = swap_uint32(tmp_values.n_un.n_strx);
		}
		// SKIP DEBUG SYMBOLS
		if (!(tmp_values.n_type & N_STAB)) {
			// CHECK IF GOOD STRING INDEX
			t_parsed_symbol *new = (t_parsed_symbol *)malloc(sizeof(t_parsed_symbol));
			if (!new)
				ft_putstr_fd("error: nm: malloc failed\n", STDERR_FILENO);
			else {
				if (tmp_values.n_un.n_strx > symcmd.strsize)
					new->name = "bad string index";
				else
					new->name = strtab + tmp_values.n_un.n_strx;
				new->n_type = tmp_values.n_type;
				new->n_sect = tmp_values.n_sect;
				new->n_value = tmp_values.n_value;
				new->arch = 32;
				new->next = NULL;
				// PUSH TO PARSED SYMBOLS LIST
				push_symbol(parsed_symbols, new);
			}
		}
		tmp += 1;
		symcmd.nsyms--;
	}
}

void	load_symbols64(t_parsed_symbol **parsed_symbols, struct nlist *symtab, char *strtab, struct symtab_command symcmd, int swap) {
	struct nlist_64		*tmp64;
	tmp64 = (struct nlist_64 *)symtab;
	if (swap) {
		symcmd.nsyms = swap_uint32(symcmd.nsyms);
		symcmd.strsize = swap_uint32(symcmd.strsize);
	}
	while (symcmd.nsyms) {
		struct nlist_64 tmp_values = *tmp64;
		if (swap) {
			tmp_values.n_value = swap_uint32(tmp_values.n_value);
			tmp_values.n_un.n_strx = swap_uint32(tmp_values.n_un.n_strx);
		}
		// SKIP DEBUG SYMBOLS
		if (!(tmp_values.n_type & N_STAB)) {
			// CHECK IF GOOD STRING INDEX
			t_parsed_symbol *new = (t_parsed_symbol *)malloc(sizeof(t_parsed_symbol));
			if (!new)
				ft_putstr_fd("error: nm: malloc failed\n", STDERR_FILENO);
			else {
				if (tmp_values.n_un.n_strx > symcmd.strsize)
					new->name = "bad string index";
				else
					new->name = strtab + tmp_values.n_un.n_strx;
				new->n_type = tmp_values.n_type;
				new->n_sect = tmp_values.n_sect;
				new->n_value = tmp_values.n_value;
				new->arch = 64;
				new->next = NULL;
				// PUSH TO PARSED SYMBOLS LIST
				push_symbol(parsed_symbols, new);
			}
		}
		tmp64 += 1;
		symcmd.nsyms--;
	}
}
