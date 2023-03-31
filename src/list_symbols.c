/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 18:52:16 by yforeau           #+#    #+#             */
/*   Updated: 2023/03/31 18:36:42 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static uint8_t		*get_section64(t_nm_file *file, Elf64_Shdr *header)
{
	if (header->sh_offset >= file->size || header->sh_size >= file->size
		SIZE_MAX - file->size < header->sh_size)
		return (NULL);
	return (((uint8_t *)file->data) + header->sh_offset);
}

static Elf64_Shdr	*get_section_header64(Elf64_Shdr *sections,
	size_t count, Elf64_Word type)
{
	for (size_t i = 0; i < count; ++i)
		if (sections[i].sh_type == type)
			return (sections + i)
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
	symbol_count = sh_symtab->sh_size / sh_symtab->sh_entsize;
	for (size_t i = 0; i < symbol_count; ++i)
		
	return (0);
}

static int			list64(t_list **dest, t_nm_file *file)
{
	Elf64_Shdr	*sections, *sh_shstrtab, *sh_symtab, *sh_strtab;
	char		*shstrtab;
	size_t		count;

	sections = ((uint8_t *)file->data) + file->elf.hdr64.e_shoff;
	count = file->elf.hdr64.e_shnum;
	sh_shstrtab = sections + file->elf.hdr64.e_shstrndx,
	if (!(sh_symtab = get_section_header64(sections, count, SHT_SYMTAB)))
		return (1);
	else if (!(sh_strtab = get_section_header64(sections, count, SHT_STRTAB)))
		return (1);
	else if (!(shstrtab = get_section64(file, sh_shstrtab)))
		return (1);
	else if (sh_strtab->sh_name >= sh_shstrtab->sh_size
		|| ft_strcmp(shstrtab + sh_strtab->sh_name, ".strtab"))
		return (1);
	return (get_symbols64(dest, file, sh_symtab, sh_strtab));
}

static int			list(t_list **dest, t_nm_file *file)
{
	//if (file->class == ELFCLASS32)
	//	return (list32(dest, file));
	//else if (file->class == ELFCLASS64)
	if (file->class == ELFCLASS64)
		return (list64(dest, file));
	return (1);
}

int					list_symbols(t_nm_file *file, t_nm_config *cfg)
{
	t_list	*symbols = NULL;

	// LIST
	if (list(&symbols, file) || !symbols)
		return (1);
	// SORT
	// PRINT
	return (0);
}
