/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 11:37:14 by lumenthi          #+#    #+#             */
/*   Updated: 2020/02/14 12:43:35 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
 * The name field (16 bytes) contains the name of the file.
	If the name is either longer than 16 bytes, or contains a space character, the actual name should be written directly after the header line and the name field should contain the string #1/ followed by the length.
	To keep the archive entries aligned to 4 byte boundaries, length of the name that follows the #1/ is rounded to 4 bytes and the name that follows the header is padded with null bytes.
*/

/*
The __.SYMDEF and .__SORTED SYMDEF archive members contain an array of ranlib data structures, preceded by the length (in bytes) (a long integer, 4 bytes) of the number of items in the array. The array is followed by a string table of null-terminated strings, which are preceded by the length in bytes of the entire string table (again, a 4-byte long integer).
*/

/*#define	ARMAG		"!<arch>\n"	[> ar "magic number" <]*/
/*#define	SARMAG		8			[> strlen(ARMAG); <]*/

/*#define	AR_EFMT1	"#1/"		[> extended format #1 <]*/

/*struct ar_hdr {*/
	/*char ar_name[16];			[> name <]*/
	/*char ar_date[12];			[> modification time <]*/
	/*char ar_uid[6];			[> user id <]*/
	/*char ar_gid[6];			[> group id <]*/
	/*char ar_mode[8];			[> octal file permissions <]*/
	/*char ar_size[10];			[> size in bytes <]*/
	/*#define	ARFMAG			"`\n"*/
	/*char ar_fmag[2];			[> consistency check <]*/
/*};*/

/*struct	ranlib {*/
		/*union {*/
			/*uint32_t				ran_strx;	[> string table index of <]*/
			/*#ifndef __LP64__*/
				/*char				*ran_name;	[> symbol defined by <]*/
			/*#endif*/
		/*}							ran_un;*/
		/*uint32_t					ran_off;	[> library member at this offset <]*/
/*};*/

size_t	name_len(void *ar_hdr) {
	size_t size = 0;
	size = ft_atoi((char *)ar_hdr + ft_strlen(AR_EFMT1));
	return size;
}

void	handle_archive(char *arg, void *file, int cmd) {
	struct ar_hdr *header_ar = (struct ar_hdr *)(file + SARMAG);
	void *symdef = (void *)(header_ar + 1) + name_len(header_ar);

	uint32_t total_size = *(uint32_t *)symdef; // == uint32_t, long but not negative (for operations purpose)
	uint32_t nranlibs = total_size / sizeof(struct ranlib);
	struct ranlib* ranlib = (struct ranlib*)(symdef + sizeof(uint32_t));
	// useless ?
	// uint32_t strtab_len = *(uint32_t *)(symdef + sizeof(uint32_t) + total_size);
	// char *strtab = (char *)(symdef + sizeof(uint32_t) + total_size + sizeof(uint32_t));
	uint32_t i = 0;
	while (i < nranlibs) {
		if (i != 0)
			ft_putchar('\n');
		struct ar_hdr *s_ar_header = (struct ar_hdr *)
										(file + ranlib->ran_off);
		void *ar_object = (void *)(s_ar_header + 1) + name_len(s_ar_header);
		// ft_putendl(s_ar_header->ar_name);
		ft_putstr(arg);
		ft_putchar('(');
		ft_putstr((char *)(s_ar_header + 1));
		ft_putstr("):\n");
		iterate_commands(arg, ar_object, cmd);
		ranlib += 1;
		i++;
	}
}
