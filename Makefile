CC = gcc
CFLAGS = -Wall -g
LDFLAGS = 
OBJDIR=build

SRC = jesd204.c \
	jesd204-topo.c

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

all: jesd204

jesd204: $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))
	$(CC) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -r $(OBJDIR)
