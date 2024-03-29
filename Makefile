############################## COMPILE VAR #####################################

CC			=	gcc
#CFLAGS		=	-Wall -Wextra -Werror
CFLAGS		=	-Wall -Wextra -Werror -g -fsanitize=address,undefined
HDIR		=	includes
SRCDIR		=	src
SUB1D		=	libft
HFLAGS		=	-I $(HDIR) -I $(SUB1D)/$(HDIR)
LIBS		=	$(SUB1D)/libft.a
NAME		=	ft_nm

############################## SOURCES #########################################

SRCC			=	filter.c\
					main.c\
					list64.c\
					elf.c\
					list_symbols.c\
					sort.c\
					symbol32.c\
					utils.c\
					strings.c\
					options.c\
					list32.c\
					symbol64.c\

ODIR			=	obj
OBJ				=	$(patsubst %.c,%.o,$(SRCC))

vpath			%.o	$(ODIR)
vpath			%.h	$(HDIR)
vpath			%.h	$(SUB1D)/$(HDIR)
vpath			%.c	$(SRCDIR)

############################## BUILD ###########################################

all: $(NAME)

$(NAME): $(SUB1D)/libft.a $(ODIR) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(patsubst %.o,$(ODIR)/%.o,$(OBJ)) $(LIBS)

$(SUB1D)/libft.a:
	make -C $(SUB1D)

filter.o: ft_nm.h libft.h
main.o: ft_nm.h libft.h
list64.o: ft_nm.h libft.h
elf.o: ft_nm.h libft.h
list_symbols.o: ft_nm.h libft.h
sort.o: ft_nm.h libft.h
symbol32.o: ft_nm.h libft.h
utils.o: ft_nm.h libft.h
strings.o: ft_nm.h libft.h
options.o: ft_nm.h libft.h
list32.o: ft_nm.h libft.h
symbol64.o: ft_nm.h libft.h
%.o: %.c
	@mkdir -p $(ODIR)
	$(CC) -c $(CFLAGS) $< $(HFLAGS) -o $(ODIR)/$@

$(ODIR):
	mkdir -p $@

############################## CLEANUP #########################################

clean:
	rm -rf $(ODIR)
	make -C $(SUB1D) fclean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
