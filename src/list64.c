/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:25:09 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/01 21:43:30 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void			*get_section64(t_nm_file *file, Elf64_Shdr *header)
{
	if (header->sh_offset >= file->size || header->sh_size >= file->size
		|| SIZE_MAX - file->size < header->sh_size)
		return (NULL);
	return (((uint8_t *)file->data) + header->sh_offset);
}

static Elf64_Shdr	*get_section_header64(Elf64_Shdr *sections,
	size_t count, Elf64_Word type, int skip)
{
	for (size_t i = 0; i < count; ++i)
		if (sections[i].sh_type == type && !skip--)
			return (sections + i);
	return (NULL);
}

static int			get_symbols64(t_list **dest, t_nm_file *file,
	Elf64_Shdr *sh_symtab, Elf64_Shdr *sh_strtab)
{
	Elf64_Sym	*symtab;
	char		*strtab;
	size_t		symbol_count;

	if (sh_symtab->sh_entsize != sizeof(Elf64_Sym))
		return (1);
	else if (!(symtab = get_section64(file, sh_symtab)))
		return (1);
	else if (!(strtab = get_section64(file, sh_strtab)))
		return (1);
	symbol_count = sh_symtab->sh_size / sizeof(Elf64_Sym);
	for (size_t i = 0; i < symbol_count; ++i)
		if (!push_symbol64(dest, symtab + i, strtab, sh_strtab->sh_size))
			return (1);
	return (0);
}

int				list64(t_list **dest, t_nm_file *file)
{
	Elf64_Shdr	*sections, *sh_shstrtab, *sh_symtab, *sh_strtab;
	char		*shstrtab;
	int			skip = 0;
	size_t		count;

	sections = (Elf64_Shdr *)(((uint8_t *)file->data)
		+ file->elf.hdr64.e_shoff);
	count = file->elf.hdr64.e_shnum;
	sh_shstrtab = sections + file->elf.hdr64.e_shstrndx;
	if (!(shstrtab = get_section64(file, sh_shstrtab)))
		return (1);
	else if (!(sh_symtab
		= get_section_header64(sections, count, SHT_SYMTAB, 0)))
		return (1);
	while ((sh_strtab
		= get_section_header64(sections, count, SHT_STRTAB, skip)))
	{
		if (sh_strtab->sh_name >= sh_shstrtab->sh_size)
			return (1);
		if (ft_strcmp(shstrtab + sh_strtab->sh_name, ".strtab"))
			++skip;
		else
			break;
	}
	if (!sh_strtab)
		return (1);
	return (get_symbols64(dest, file, sh_symtab, sh_strtab));
}
