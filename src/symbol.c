/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:07:45 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/01 21:32:18 by yforeau          ###   ########.fr       */
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
	char	type = '?';

	if (elf_symbol->st_shndx == SHN_UNDEF)
		type = 'U';
	else if (elf_symbol->st_shndx == SHN_ABS)
		type = 'A';
	else if (elf_symbol->st_shndx == SHN_COMMON)
		type = 'C';
	else if (sections[elf_symbol->st_shndx].sh_type == SHT_NOBITS
		&& sections[elf_symbol->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		type = 'B';
	else if (sections[elf_symbol->st_shndx].sh_type == SHT_PROGBITS
		&& sections[elf_symbol->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		type = 'D';
	else if (sections[elf_symbol->st_shndx].sh_type == SHT_PROGBITS
		&& sections[elf_symbol->st_shndx].sh_flags
			== (SHF_ALLOC | SHF_EXECINSTR))
		type = 'T';
	else if (sections[elf_symbol->st_shndx].sh_type == SHT_PROGBITS
		&& sections[elf_symbol->st_shndx].sh_flags == SHF_ALLOC)
		type = 'R';
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
