/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol32.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:07:45 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/04 19:42:59 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static char	symbol_type32(Elf32_Sym *elf_symbol, Elf32_Shdr *sections)
{
	char			type = '?';
	unsigned char	bind = ELF32_ST_BIND(elf_symbol->st_info);

	if (bind == STB_WEAK && ELF32_ST_TYPE(elf_symbol->st_info) == STT_OBJECT)
		type = elf_symbol->st_shndx == SHN_UNDEF ? 'v' : 'V';
	else if (bind == STB_WEAK)
		type = elf_symbol->st_shndx == SHN_UNDEF ? 'w' : 'W';
	else if (elf_symbol->st_shndx == SHN_UNDEF)
		type = bind == STB_LOCAL ? 'u' : 'U';
	else if (elf_symbol->st_shndx == SHN_ABS)
		type = bind == STB_LOCAL ? 'a' : 'A';
	else if (elf_symbol->st_shndx == SHN_COMMON)
		type = bind == STB_LOCAL ? 'c' : 'C';
	else if (sections[elf_symbol->st_shndx].sh_type == SHT_NOBITS
		&& (sections[elf_symbol->st_shndx].sh_flags & (SHF_ALLOC | SHF_WRITE)))
		type = bind == STB_LOCAL ? 'b' : 'B';
	else if (sections[elf_symbol->st_shndx].sh_type == SHT_DYNAMIC ||
		((sections[elf_symbol->st_shndx].sh_type == SHT_PROGBITS
		|| sections[elf_symbol->st_shndx].sh_type == SHT_INIT_ARRAY
		|| sections[elf_symbol->st_shndx].sh_type == SHT_FINI_ARRAY)
		&& (sections[elf_symbol->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)
		|| sections[elf_symbol->st_shndx].sh_flags == 1027)))
		type = bind == STB_LOCAL ? 'd' : 'D';
	else if (sections[elf_symbol->st_shndx].sh_type == SHT_PREINIT_ARRAY)
		type = 'D';
	else if (sections[elf_symbol->st_shndx].sh_type == SHT_PROGBITS
		&& sections[elf_symbol->st_shndx].sh_flags
			== (SHF_ALLOC | SHF_EXECINSTR))
		type = bind == STB_LOCAL ? 't' : 'T';
	else if (ELF32_ST_TYPE(elf_symbol->st_info) == STT_SECTION
		&& sections[elf_symbol->st_shndx].sh_type == SHT_PROGBITS
		&& sections[elf_symbol->st_shndx].sh_flags == (SHF_MERGE | SHF_STRINGS))
		type = bind == STB_LOCAL ? 'n' : 'N';
	else if (ELF32_ST_TYPE(elf_symbol->st_info) == STT_SECTION
		&& sections[elf_symbol->st_shndx].sh_type == SHT_PROGBITS
		&& (!sections[elf_symbol->st_shndx].sh_flags
		|| sections[elf_symbol->st_shndx].sh_flags == 2048
		|| sections[elf_symbol->st_shndx].sh_flags == 2096))
		type = 'N';
	else if (!(sections[elf_symbol->st_shndx].sh_flags & SHF_WRITE))
		type = bind == STB_LOCAL ? 'r' : 'R';
	return (type);
}

static char	*symbol_name32(Elf32_Sym *elf_symbol, t_nm_file *file)
{
	if (ELF32_ST_TYPE(elf_symbol->st_info) == STT_SECTION)
	{
		if (elf_symbol->st_shndx >= file->sections_count)
			return (NULL);
		else if (file->sections.hdr32[elf_symbol->st_shndx].sh_name
			>= file->shstrtab_size)
			return (NULL);
		return (file->shstrtab
			+ file->sections.hdr32[elf_symbol->st_shndx].sh_name);
	}
	else
	{
		if (elf_symbol->st_name >= file->strtab_size)
			return (NULL);
		return (file->strtab + elf_symbol->st_name);
	}
}

t_list	*push_symbol32(t_list **dest, Elf32_Sym *elf_symbol,
	t_nm_file *file, int idx)
{
	t_nm_symbol	symbol = { 0 };

	if (elf_symbol->st_name >= file->strtab_size)
		return (NULL);
	if (!(symbol.name = symbol_name32(elf_symbol, file)))
		return (NULL);
	symbol.type = symbol_type32(elf_symbol, file->sections.hdr32);
	symbol.value = elf_symbol->st_value;
	symbol.idx = idx;
	return (ft_lst_push_back(dest, &symbol, sizeof(symbol)));
}
