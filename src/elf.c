/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 20:43:23 by yforeau           #+#    #+#             */
/*   Updated: 2023/01/05 19:40:15 by yforeau          ###   ########.fr       */
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
		|| raw->e_ident[EI_VERSION] != EV_CURRENT
		|| raw->e_ident[EI_OSABI] == 0x05
		|| raw->e_ident[EI_OSABI] > ELF_OSABI_MAX)
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
		|| raw->e_ident[EI_VERSION] != EV_CURRENT
		|| raw->e_ident[EI_OSABI] == 0x05
		|| raw->e_ident[EI_OSABI] > ELF_OSABI_MAX)
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

const char		*g_nm_osabi[ELF_OSABI_MAX + 1] = {
	"System V",
	"HP-UX",
	"NetBSD",
	"Linux",
	"GNU Hurd",
	NULL,
	"Solaris",
	"AIX (Monterey)",
	"IRIX",
	"FreeBSD",
	"Tru64",
	"Novell Modesto",
	"OpenBSD",
	"OpenVMS",
	"NonStop Kernel",
	"AROS",
	"FenixOS",
	"Nuxi CloudABI",
	"Stratus Technologies OpenVOS",
};

const char		*g_nm_type[] = {
	[ET_NONE] = "NONE (None)",
	[ET_REL] = "REL (Relocatable file)",
	[ET_EXEC] = "EXEC (Executable file)",
	[ET_DYN] = "DYN (Shared object file)",
	[ET_CORE] = "CORE (Core file)",
	[ET_CORE + 1] = "OS Specific: (%x)",
	[ET_CORE + 2] = "Processor Specific: (%x)",
	[ET_CORE + 3] = "<unknown>: %x",
};

void			print_elf32_header(Elf32_Ehdr *hdr)
{
	unsigned int	type_index = ET_CORE + 3;

	ft_printf("ELF Header:\n");
	ft_printf("  Magic:  %16t %02hhx\n", hdr->e_ident);
	ft_printf("  %-34s ELF32\n", "Class:");
	ft_printf("  %-34s 2's complement, %s endian\n", "Data:",
		hdr->e_ident[EI_DATA] == ELFDATA2LSB ? "little" : "big");
	ft_printf("  %-34s %d%s\n", "Version:", hdr->e_version,
		hdr->e_version == EV_CURRENT ? " (current)" : "");
	ft_printf("  %-34s UNIX - %s\n", "OS/ABI:",
		g_nm_osabi[hdr->e_ident[EI_OSABI]]);
	ft_printf("  %-34s %d\n", "ABI Version:", hdr->e_ident[EI_PAD]);
	ft_printf("  %-34s ", "Type:");
	if (hdr->e_type <= ET_CORE)
		type_index = hdr->e_type;
	else if (hdr->e_type >= ET_LOOS && hdr->e_type <= ET_HIOS)
		type_index = ET_CORE + 1;
	else if (hdr->e_type >= ET_LOPROC)
		type_index = ET_CORE + 2;
	ft_printf(g_nm_type[type_index], hdr->e_type);
	ft_printf("\n");
}

void			print_elf64_header(Elf64_Ehdr *hdr)
{
	unsigned int	type_index = ET_CORE + 3;

	ft_printf("ELF Header:\n");
	ft_printf("  Magic:  %16t %02hhx\n", hdr->e_ident);
	ft_printf("  %-34s ELF64\n", "Class:");
	ft_printf("  %-34s 2's complement, %s endian\n", "Data:",
		hdr->e_ident[EI_DATA] == ELFDATA2LSB ? "little" : "big");
	ft_printf("  %-34s %d%s\n", "Version:", hdr->e_version,
		hdr->e_version == EV_CURRENT ? " (current)" : "");
	ft_printf("  %-34s UNIX - %s\n", "OS/ABI:",
		g_nm_osabi[hdr->e_ident[EI_OSABI]]);
	ft_printf("  %-34s %d\n", "ABI Version:", hdr->e_ident[EI_PAD]);
	ft_printf("  %-34s ", "Type:");
	if (hdr->e_type <= ET_CORE)
		type_index = hdr->e_type;
	else if (hdr->e_type >= ET_LOOS && hdr->e_type <= ET_HIOS)
		type_index = ET_CORE + 1;
	else if (hdr->e_type >= ET_LOPROC)
		type_index = ET_CORE + 2;
	ft_printf(g_nm_type[type_index], hdr->e_type);
	ft_printf("\n");
}
