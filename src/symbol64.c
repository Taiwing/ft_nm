/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:07:45 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/13 18:46:02 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static char	symbol_type_no_section64(Elf64_Sym *elf_symbol)
{
	unsigned char	st_bind = ELF64_ST_BIND(elf_symbol->st_info);
	unsigned char	st_type = ELF64_ST_TYPE(elf_symbol->st_info);
	uint64_t		st_shndx = elf_symbol->st_shndx;

	if (st_bind == STB_WEAK && st_type == STT_OBJECT)
		return (st_shndx == SHN_UNDEF ? 'v' : 'V');
	else if (st_bind == STB_WEAK)
		return (st_shndx == SHN_UNDEF ? 'w' : 'W');
	else if (st_shndx == SHN_UNDEF)
		return (st_bind == STB_LOCAL ? 'u' : 'U');
	else if (st_shndx == SHN_ABS)
		return (st_bind == STB_LOCAL ? 'a' : 'A');
	else if (st_shndx == SHN_COMMON)
		return (st_bind == STB_LOCAL ? 'c' : 'C');
	return '?';
}

static char	symbol_type64(Elf64_Sym *elf_symbol, Elf64_Shdr *sections)
{
	char			type;
	unsigned char	st_bind = ELF64_ST_BIND(elf_symbol->st_info);
	unsigned char	st_type = ELF64_ST_TYPE(elf_symbol->st_info);
	uint64_t		sh_type, sh_flags;

	if ((type = symbol_type_no_section64(elf_symbol)) != '?')
		return (type);
	sh_type = sections[elf_symbol->st_shndx].sh_type;
	sh_flags = sections[elf_symbol->st_shndx].sh_flags;
	if (sh_type == SHT_NOBITS && (sh_flags & (SHF_ALLOC | SHF_WRITE)))
		type = st_bind == STB_LOCAL ? 'b' : 'B';
	else if (sh_type == SHT_DYNAMIC || ((sh_type == SHT_PROGBITS
		|| sh_type == SHT_INIT_ARRAY || sh_type == SHT_FINI_ARRAY)
		&& (sh_flags == (SHF_ALLOC | SHF_WRITE) || sh_flags == 1027)))
		type = st_bind == STB_LOCAL ? 'd' : 'D';
	else if (sh_type == SHT_PREINIT_ARRAY)
		type = 'D';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		type = st_bind == STB_LOCAL ? 't' : 'T';
	else if (st_type == STT_SECTION && sh_type == SHT_PROGBITS
		&& sh_flags == (SHF_MERGE | SHF_STRINGS))
		type = st_bind == STB_LOCAL ? 'n' : 'N';
	else if (st_type == STT_SECTION && sh_type == SHT_PROGBITS
		&& (!sh_flags || sh_flags == 2048 || sh_flags == 2096))
		type = 'N';
	else if (!(sh_flags & SHF_WRITE))
		type = st_bind == STB_LOCAL ? 'r' : 'R';
	return (type);
}

static char	*symbol_name64(Elf64_Sym *elf_symbol, t_nm_file *file)
{
	if (ELF64_ST_TYPE(elf_symbol->st_info) == STT_SECTION)
	{
		if (elf_symbol->st_shndx >= file->sections_count)
			return (NULL);
		else if (file->sections.hdr64[elf_symbol->st_shndx].sh_name
			>= file->shstrtab_size)
			return (NULL);
		else if (!is_null_terminated(file->shstrtab
			+ file->sections.hdr64[elf_symbol->st_shndx].sh_name,
			file->shstrtab_size
			- file->sections.hdr64[elf_symbol->st_shndx].sh_name))
			return (NULL);
		return (file->shstrtab
			+ file->sections.hdr64[elf_symbol->st_shndx].sh_name);
	}
	else
	{
		if (elf_symbol->st_name >= file->strtab_size)
			return (NULL);
		return (file->strtab + elf_symbol->st_name);
	}
}

t_list	*push_symbol64(t_list **dest, Elf64_Sym *elf_symbol,
	t_nm_file *file, int idx)
{
	t_nm_symbol	symbol = { 0 };

	if (elf_symbol->st_name >= file->strtab_size)
		return (NULL);
	if (!(symbol.name = symbol_name64(elf_symbol, file)))
		return (NULL);
	symbol.type = symbol_type64(elf_symbol, file->sections.hdr64);
	symbol.value = elf_symbol->st_value;
	symbol.idx = idx;
	return (ft_lst_push_back(dest, &symbol, sizeof(symbol)));
}
