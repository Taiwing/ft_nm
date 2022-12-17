/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 20:43:23 by yforeau           #+#    #+#             */
/*   Updated: 2022/12/17 21:10:31 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void		swap_elf32_header(Elf32_Ehdr *dest)
{
	ft_memswap(&dest->e_type, sizeof(dest->e_type));
	ft_memswap(&dest->e_machine, sizeof(dest->e_machine));
	ft_memswap(&dest->e_version, sizeof(dest->e_version));
	ft_memswap(&dest->e_entry, sizeof(dest->e_entry));
	ft_memswap(&dest->e_phoff, sizeof(dest->e_phoff));
	ft_memswap(&dest->e_shoff, sizeof(dest->e_shoff));
	ft_memswap(&dest->e_flags, sizeof(dest->e_flags));
	ft_memswap(&dest->e_ehsize, sizeof(dest->e_ehsize));
	ft_memswap(&dest->e_phentsize, sizeof(dest->e_phentsize));
	ft_memswap(&dest->e_phnum, sizeof(dest->e_phnum));
	ft_memswap(&dest->e_shentsize, sizeof(dest->e_shentsize));
	ft_memswap(&dest->e_shnum, sizeof(dest->e_shnum));
	ft_memswap(&dest->e_shstrndx, sizeof(dest->e_shstrndx));
}

static void		swap_elf64_header(Elf64_Ehdr *dest)
{
	ft_memswap(&dest->e_type, sizeof(dest->e_type));
	ft_memswap(&dest->e_machine, sizeof(dest->e_machine));
	ft_memswap(&dest->e_version, sizeof(dest->e_version));
	ft_memswap(&dest->e_entry, sizeof(dest->e_entry));
	ft_memswap(&dest->e_phoff, sizeof(dest->e_phoff));
	ft_memswap(&dest->e_shoff, sizeof(dest->e_shoff));
	ft_memswap(&dest->e_flags, sizeof(dest->e_flags));
	ft_memswap(&dest->e_ehsize, sizeof(dest->e_ehsize));
	ft_memswap(&dest->e_phentsize, sizeof(dest->e_phentsize));
	ft_memswap(&dest->e_phnum, sizeof(dest->e_phnum));
	ft_memswap(&dest->e_shentsize, sizeof(dest->e_shentsize));
	ft_memswap(&dest->e_shnum, sizeof(dest->e_shnum));
	ft_memswap(&dest->e_shstrndx, sizeof(dest->e_shstrndx));
}

static int		load_elf32_header(Elf32_Ehdr *dest, Elf32_Ehdr *raw)
{
	if (ft_memcmp(raw->e_ident, ELFMAG, 4)
		|| (raw->e_ident[EI_DATA] != ELFDATA2LSB
		&& raw->e_ident[EI_DATA] != ELFDATA2MSB)
		|| raw->e_ident[EI_VERSION] != EV_CURRENT)
		return (1);
	ft_memcpy(dest, raw, sizeof(Elf32_Ehdr));
	if ((__BYTE_ORDER == __LITTLE_ENDIAN
		&& raw->e_ident[EI_DATA] == ELFDATA2MSB)
		|| (__BYTE_ORDER == __BIG_ENDIAN
		&& raw->e_ident[EI_DATA] == ELFDATA2LSB))
		swap_elf32_header(dest);
	if (dest->e_version != (uint32_t)dest->e_ident[EI_VERSION]
		|| dest->e_ehsize != sizeof(Elf32_Ehdr))
		return (1);
	//TODO: check the other fields
	return (0);
}

static int		load_elf64_header(Elf64_Ehdr *dest, Elf64_Ehdr *raw)
{
	if (ft_memcmp(raw->e_ident, ELFMAG, 4)
		|| (raw->e_ident[EI_DATA] != ELFDATA2LSB
		&& raw->e_ident[EI_DATA] != ELFDATA2MSB)
		|| raw->e_ident[EI_VERSION] != EV_CURRENT)
		return (1);
	ft_memcpy(dest, raw, sizeof(Elf64_Ehdr));
	if ((__BYTE_ORDER == __LITTLE_ENDIAN
		&& raw->e_ident[EI_DATA] == ELFDATA2MSB)
		|| (__BYTE_ORDER == __BIG_ENDIAN
		&& raw->e_ident[EI_DATA] == ELFDATA2LSB))
		swap_elf64_header(dest);
	if (dest->e_version != (uint32_t)dest->e_ident[EI_VERSION]
		|| dest->e_ehsize != sizeof(Elf64_Ehdr))
		return (1);
	//TODO: check the other fields
	return (0);
}

int				read_elf_header(t_nm_file *dest, t_nm_config *cfg)
{
	dest->class = ((Elf32_Ehdr *)dest->data)->e_ident[EI_CLASS];
	if (dest->class == ELFCLASS32
		&& !load_elf32_header(&dest->elf.hdr32, dest->data))
		return (0);
	else if (dest->class == ELFCLASS64
		&& !load_elf64_header(&dest->elf.hdr64, dest->data))
		return (0);
	return (!!ft_dprintf(2, "%s: invalid elf header\n", cfg->exec));
}

void			print_elf32_header(Elf32_Ehdr *hdr)
{
	ft_printf("ELF Header:\n");
	ft_printf("  Magic:  %16t %02hhx\n", hdr->e_ident);
	ft_printf("  %-34s ELF32\n", "Class:");
	ft_printf("  %-34s 2's complement, %s endian\n", "Data:",
		hdr->e_ident[EI_DATA] == ELFDATA2LSB ? "little" : "big");
	ft_printf("  %-34s %d%s\n", "Version:", hdr->e_version,
		hdr->e_version == EV_CURRENT ? " (current)" : "");
}

void			print_elf64_header(Elf64_Ehdr *hdr)
{
	ft_printf("ELF Header:\n");
	ft_printf("  Magic:  %16t %02hhx\n", hdr->e_ident);
	ft_printf("  %-34s ELF64\n", "Class:");
	ft_printf("  %-34s 2's complement, %s endian\n", "Data:",
		hdr->e_ident[EI_DATA] == ELFDATA2LSB ? "little" : "big");
	ft_printf("  %-34s %d%s\n", "Version:", hdr->e_version,
		hdr->e_version == EV_CURRENT ? " (current)" : "");
}
