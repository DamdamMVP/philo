NAME		=	philo

CC			=	cc
CFLAGS		=	-Werror -Wextra -Wall -g -I $(INCDIR)

SRCDIR		=	src
INCDIR		=	inc
OBJDIR		=	obj

SRC			=	main inits utils verif actions threads
SRCS		=	$(addprefix $(SRCDIR)/, $(addsuffix .c, $(SRC)))
OBJS		=	$(addprefix $(OBJDIR)/, $(SRC:=.o))


all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Compilation de $(NAME) terminée."

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compilation de $<"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR)
	@echo "Nettoyage des fichiers objets."

fclean: clean
	@rm -f $(NAME)
	@echo "Nettoyage complet."

re: fclean all

debug: CFLAGS += -fsanitize=thread
debug: re

leak: CFLAGS += -fsanitize=leak
leak: re

norminette:
	@echo "Norminette de $(NAME) dans $(SRCDIR) et $(INCDIR)..."
	@if norminette $(SRCDIR)/*.c $(INCDIR)/*.h | grep -v "OK!" | grep -q "Error!"; then \
		norminette $(SRCDIR)/*.c $(INCDIR)/*.h | grep -v "OK!" | \
		while read line; do \
			if echo $$line | grep -q "Error!"; then \
				echo "\033[0;31m$$line\033[0m"; \
			else \
				echo "$$line"; \
			fi; \
		done; \
	else \
		echo "\033[0;32mAll files are norminette friendly !\033[0m"; \
	fi

.PHONY: all clean fclean re norminette leak debug