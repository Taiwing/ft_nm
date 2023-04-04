/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:07:45 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/04 19:42:59 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	delete_symbol(void *symbol, size_t size)
{
	(void)size;

	ft_memdel(&symbol);
}

char	symbol_type64(Elf64_Sym *elf_symbol, Elf64_Shdr *sections)
{
	char			type = '?';
	unsigned char	bind = ELF64_ST_BIND(elf_symbol->st_info);

	if (bind == STB_WEAK && ELF64_ST_TYPE(elf_symbol->st_info))
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
		&& sections[elf_symbol->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		type = bind == STB_LOCAL ? 'b' : 'B';
	else if (sections[elf_symbol->st_shndx].sh_type == SHT_DYNAMIC ||
		(sections[elf_symbol->st_shndx].sh_type == SHT_PROGBITS
		&& sections[elf_symbol->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)))
		type = bind == STB_LOCAL ? 'd' : 'D';
	else if (sections[elf_symbol->st_shndx].sh_type == SHT_PREINIT_ARRAY)
		type = 'D';
	else if (sections[elf_symbol->st_shndx].sh_type == SHT_PROGBITS
		&& sections[elf_symbol->st_shndx].sh_flags
			== (SHF_ALLOC | SHF_EXECINSTR))
		type = bind == STB_LOCAL ? 't' : 'T';
	else if (sections[elf_symbol->st_shndx].sh_type & SHT_PROGBITS
		&& !(sections[elf_symbol->st_shndx].sh_flags & SHF_WRITE))
		type = bind == STB_LOCAL ? 'r' : 'R';
	return (type);
}

t_list	*push_symbol32(t_list **dest, Elf32_Sym *elf_symbol, t_nm_file *file)
{
	t_nm_symbol	symbol = { 0 };

	if (elf_symbol->st_name >= file->strtab_size)
		return (NULL);
	symbol.name = file->strtab + elf_symbol->st_name;
	symbol.type = 'U'; //TODO: find appropriate letter
	symbol.value = elf_symbol->st_value;
	return (ft_lst_push_back(dest, &symbol, sizeof(symbol)));
}

t_list	*push_symbol64(t_list **dest, Elf64_Sym *elf_symbol, t_nm_file *file)
{
	t_nm_symbol	symbol = { 0 };

	if (elf_symbol->st_name >= file->strtab_size)
		return (NULL);
	symbol.name = file->strtab + elf_symbol->st_name;
	symbol.type = symbol_type64(elf_symbol, file->sections.hdr64);
	symbol.value = elf_symbol->st_value;
	return (ft_lst_push_back(dest, &symbol, sizeof(symbol)));
}
