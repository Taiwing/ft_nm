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

t_list	*push_symbol32(t_list **dest, Elf32_Sym *elf_symbol,
	char *strtab, size_t size)
{
	t_nm_symbol	symbol = { 0 };

	if (elf_symbol->st_name >= size)
		return (NULL);
	symbol.name = strtab + elf_symbol->st_name;
	symbol.type = 'U'; //TODO: find appropriate letter
	symbol.value = elf_symbol->st_value;
	return (ft_lst_push_back(dest, &symbol, sizeof(symbol)));
}

t_list	*push_symbol64(t_list **dest, Elf64_Sym *elf_symbol,
	char *strtab, size_t size)
{
	t_nm_symbol	symbol = { 0 };

	if (elf_symbol->st_name >= size)
		return (NULL);
	symbol.name = strtab + elf_symbol->st_name;
	symbol.type = 'U'; //TODO: find appropriate letter
	symbol.value = elf_symbol->st_value;
	return (ft_lst_push_back(dest, &symbol, sizeof(symbol)));
}
