/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 16:15:21 by lumenthi          #+#    #+#             */
/*   Updated: 2020/02/12 15:02:28 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*struct segment_command {	[> for 32-bit architectures <]*/
	/*unsigned long	cmd;		[> LC_SEGMENT <]*/
	/*unsigned long	cmdsize;	[> includes sizeof section structs <]*/
	/*char		segname[16];	[> segment name <]*/
	/*unsigned long	vmaddr;		[> memory address of this segment <]*/
	/*unsigned long	vmsize;		[> memory size of this segment <]*/
	/*unsigned long	fileoff;	[> file offset of this segment <]*/
	/*unsigned long	filesize;	[> amount to map from the file <]*/
	/*vm_prot_t	maxprot;	[> maximum VM protection <]*/
	/*vm_prot_t	initprot;	[> initial VM protection <]*/
	/*unsigned long	nsects;		[> number of sections in segment <]*/
	/*unsigned long	flags;		[> flags <]*/
/*};*/

/*
 * The 64-bit segment load command indicates that a part of this file is to be
 * mapped into a 64-bit task's address space.  If the 64-bit segment has
 * sections then section_64 structures directly follow the 64-bit segment
 * command and their size is reflected in cmdsize.
 */
/*struct segment_command_64 {	[> for 64-bit architectures <]*/
	/*uint32_t	cmd;		[> LC_SEGMENT_64 <]*/
	/*uint32_t	cmdsize;	[> includes sizeof section_64 structs <]*/
	/*char		segname[16];	[> segment name <]*/
	/*uint64_t	vmaddr;		[> memory address of this segment <]*/
	/*uint64_t	vmsize;		[> memory size of this segment <]*/
	/*uint64_t	fileoff;	[> file offset of this segment <]*/
	/*uint64_t	filesize;	[> amount to map from the file <]*/
	/*vm_prot_t	maxprot;	[> maximum VM protection <]*/
	/*vm_prot_t	initprot;	[> initial VM protection <]*/
	/*uint32_t	nsects;		[> number of sections in segment <]*/
	/*uint32_t	flags;		[> flags <]*/
/*};*/

/*struct section {		[> for 32-bit architectures <]*/
	/*char		sectname[16];	[> name of this section <]*/
	/*char		segname[16];	[> segment this section goes in <]*/
	/*unsigned long	addr;		[> memory address of this section <]*/
	/*unsigned long	size;		[> size in bytes of this section <]*/
	/*unsigned long	offset;		[> file offset of this section <]*/
	/*unsigned long	align;		[> section alignment (power of 2) <]*/
	/*unsigned long	reloff;		[> file offset of relocation entries <]*/
	/*unsigned long	nreloc;		[> number of relocation entries <]*/
	/*unsigned long	flags;		[> flags (section type and attributes)<]*/
	/*unsigned long	reserved1;	[> reserved <]*/
	/*unsigned long	reserved2;	[> reserved <]*/
/*};*/

/*struct section_64 { [> for 64-bit architectures <]*/
	/*char		sectname[16];	[> name of this section <]*/
	/*char		segname[16];	[> segment this section goes in <]*/
	/*uint64_t	addr;		[> memory address of this section <]*/
	/*uint64_t	size;		[> size in bytes of this section <]*/
	/*uint32_t	offset;		[> file offset of this section <]*/
	/*uint32_t	align;		[> section alignment (power of 2) <]*/
	/*uint32_t	reloff;		[> file offset of relocation entries <]*/
	/*uint32_t	nreloc;		[> number of relocation entries <]*/
	/*uint32_t	flags;		[> flags (section type and attributes)<]*/
	/*uint32_t	reserved1;	[> reserved (for offset or index) <]*/
	/*uint32_t	reserved2;	[> reserved (for count or sizeof) <]*/
	/*uint32_t	reserved3;	[> reserved <]*/
/*};*/

void	handle_segment32(int swap, void *cmd, t_found_section **parsed_sections) {
	struct segment_command *segment = (struct segment_command *)cmd;
	struct section *section = (struct section *)(segment + 1);
	uint32_t nsects = swap ? swap_uint32(segment->nsects) : segment->nsects;
	// SECTIONS ITERATE
	while (nsects) {
		// ALLOCATE AND APPEND IN FOUND_SECTION
		load_section(parsed_sections, section, 32);
		section += 1;
		nsects--;
	}}

void	handle_segment64(int swap, void *cmd, t_found_section **parsed_sections) {
	struct segment_command_64 *segment64 = (struct segment_command_64 *)cmd;
	struct section_64 *section64 = (struct section_64 *)(segment64 + 1);
	uint32_t nsects = swap ? swap_uint32(segment64->nsects) : segment64->nsects;

	// SECTIONS ITERATE
	while (nsects) {
		// ALLOCATE AND APPEND IN FOUND_SECTION
		load_section(parsed_sections, section64, 64);
		section64 += 1;
		nsects--;
	}
}
