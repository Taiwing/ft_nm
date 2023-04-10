/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 21:44:48 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/10 20:00:48 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void			*get_section32(t_nm_file *file, Elf32_Shdr *header)
{
	if (header->sh_offset >= file->size || header->sh_size >= file->size
		|| SIZE_MAX - file->size < header->sh_size)
		return (NULL);
	return (((uint8_t *)file->data) + header->sh_offset);
}

static Elf32_Shdr	*get_section_header32(t_nm_file *file,
	Elf32_Word type, int skip)
{
	for (size_t i = 0; i < file->sections_count; ++i)
		if (file->sections.hdr32[i].sh_type == type && !skip--)
			return (file->sections.hdr32 + i);
	return (NULL);
}

static int			get_symbols32(t_list **dest, t_nm_file *file,
	Elf32_Shdr *sh_symtab, Elf32_Shdr *sh_strtab)
{
	Elf32_Sym	*symtab;
	size_t		symbol_count;

	if (sh_symtab->sh_entsize != sizeof(Elf32_Sym))
		return (1);
	else if (!(symtab = get_section32(file, sh_symtab)))
		return (1);
	else if (!(file->strtab = get_section32(file, sh_strtab)))
		return (1);
	file->strtab_size = sh_strtab->sh_size;
	symbol_count = sh_symtab->sh_size / sizeof(Elf32_Sym);
	for (size_t i = 1; i < symbol_count; ++i)
		if (!push_symbol32(dest, symtab + i, file, i))
			return (1);
	return (0);
}

int				list32(t_list **dest, t_nm_file *file, t_nm_config *cfg)
{
	Elf32_Shdr	*sh_shstrtab, *sh_symtab, *sh_strtab;
	int			skip = 0;

	file->sections = (s_elf_shdr)(Elf32_Shdr *)(((uint8_t *)file->data)
		+ file->elf.hdr32.e_shoff);
	file->sections_count = file->elf.hdr32.e_shnum;
	sh_shstrtab = file->sections.hdr32 + file->elf.hdr32.e_shstrndx;
	if (!(file->shstrtab = get_section32(file, sh_shstrtab)))
		return (ft_dprintf(2, "%s: shstrtab section missing\n", cfg->exec));
	else if (!(sh_symtab = get_section_header32(file, SHT_SYMTAB, 0)))
		return (ft_dprintf(2, "%s: symtab section header missing\n",
			cfg->exec));
	file->shstrtab_size = sh_shstrtab->sh_size;
	while ((sh_strtab = get_section_header32(file, SHT_STRTAB, skip)))
	{
		if (sh_strtab->sh_name >= file->shstrtab_size)
			return (ft_dprintf(2, "%s: section name index is out of bound\n",
				cfg->exec));
		//TODO: check if string is null-terminated
		if (ft_strcmp(file->shstrtab + sh_strtab->sh_name, ".strtab"))
			++skip;
		else
			break;
	}
	if (!sh_strtab)
		return (ft_dprintf(2, "%s: strtab section header missing\n",
			cfg->exec));
	return (get_symbols32(dest, file, sh_symtab, sh_strtab));
}
