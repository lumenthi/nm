# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lumenthi <lumenthi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/22 14:06:43 by lumenthi          #+#    #+#              #
#    Updated: 2020/02/12 17:31:10 by lumenthi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY : all clean fclean re

NM_NAME = ft_nm
OTOOL_NAME = ft_otool

CC = gcc
FLAGS = -Wall -Werror -Wextra

GREEN = '\033[4;32m'
RED = '\033[4;31m'
BLANK = '\033[0m'
YELLOW = '\033[4;33m'
CYAN = '\033[4;38;5;51m'

TICK = '\033[1;32m~\033[0m'
CROSS = '\033[1;31mx\033[0m'

###### FOLDERS ######

LIBDIR = libft
SRCDIR = sources
HEADDIR = headers
OBJDIR = objs

#####################

###### LIBRARY ######

LIBFT = $(LIBDIR)/libft.a

#####################

###### HEADERS ######

NM_OTOOL_HEADS = nm_otool.h

NM_OTOOL_HEADERS = $(addprefix $(HEADDIR)/, $(NM_OTOOL_HEADS))

#####################

###### SOURCES ######

NM_MAIN = nm.c

OTOOL_MAIN = otool.c

NM_OTOOL_SRCS = $(NM_MAIN) \
			$(OTOOL_MAIN) \
			section_struct.c \
			symbol_struct.c \
			display.c \
			macho.c \
			archive.c \
			fat.c \
			swap.c \
			segment.c \
			symbol.c \
			tools.c

NM_OTOOL_SOURCES = $(addprefix $(SRCDIR)/, $(NM_OTOOL_SRCS))

#####################

###### OBJECTS ######

NM_MAIN_OBJ = $(addprefix $(OBJDIR)/, $(NM_MAIN:.c=.o))

OTOOL_MAIN_OBJ = $(addprefix $(OBJDIR)/, $(OTOOL_MAIN:.c=.o))

NM_OTOOL_OBJS = $(addprefix $(OBJDIR)/, $(NM_OTOOL_SRCS:.c=.o))

#####################

vpath %.c $(SRCDIR)

all: $(NM_NAME) $(OTOOL_NAME)

###### FT_NM COMPILATION ######

$(NM_NAME): $(filter-out $(OTOOL_MAIN_OBJ), $(NM_OTOOL_OBJS)) ${NM_OTOOL_HEADERS}
	@ make -sC $(LIBDIR)
	$(CC) $(filter-out $(OTOOL_MAIN_OBJ), $(NM_OTOOL_OBJS)) -o $(NM_NAME) $(LIBFT)
	@ printf " %b | Compiled %b%b%b\n" $(TICK) $(GREEN) $(NM_NAME) $(BLANK)

###############################

###### FT_OTOOL COMPILATION ######

$(OTOOL_NAME): $(filter-out $(NM_MAIN_OBJ), $(NM_OTOOL_OBJS)) ${NM_OTOOL_HEADERS}
	@ make -sC $(LIBDIR)
	$(CC) $(filter-out $(NM_MAIN_OBJ), $(NM_OTOOL_OBJS)) -o $(OTOOL_NAME) $(LIBFT)
	@ printf " %b | Compiled %b%b%b\n" $(TICK) $(GREEN) $(OTOOL_NAME) $(BLANK)

#################################

$(NM_OTOOL_OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(NM_OTOOL_HEADERS)
	mkdir -p $(OBJDIR)
	$(CC) -c $(FLAGS) -I$(HEADDIR) -I$(LIBDIR) -o $@ $<

clean:
	@ make -sC $(LIBDIR) clean
	@ test -d $(OBJDIR) && \
	rm -rf $(OBJDIR) && \
	printf " %b | " $(TICK) && \
	printf "Removed %bobjects%b folders\n" $(YELLOW) $(BLANK) \
	|| (printf " %b | " $(CROSS) && \
	printf "No %bobjects%b folders\n" $(YELLOW) $(BLANK))

fclean: clean
	@ test -f $(LIBFT) && \
	rm -rf $(LIBFT) && \
	printf " %b | " $(TICK) && \
	printf "Removed %blibft%b library\n" $(RED) $(BLANK) \
	|| (printf " %b | " $(CROSS) && \
	printf "No %blibft%b library\n" $(RED) $(BLANK))
	@ test -f $(OTOOL_NAME) && \
	rm -rf $(OTOOL_NAME) && \
	printf " %b | " $(TICK) && \
	printf "Removed %b%b%b binary\n" $(RED) $(OTOOL_NAME) $(BLANK) \
	|| (printf " %b | " $(CROSS) && \
	printf "No %b%b%b binary\n" $(RED) $(OTOOL_NAME) $(BLANK))
	@ test -f $(NM_NAME) && \
	rm -rf $(NM_NAME) && \
	printf " %b | " $(TICK) && \
	printf "Removed %b%b%b binary\n" $(RED) $(NM_NAME) $(BLANK) \
	|| (printf " %b | " $(CROSS) && \
	printf "No %b%b%b binary\n" $(RED) $(NM_NAME) $(BLANK))

re: fclean all
