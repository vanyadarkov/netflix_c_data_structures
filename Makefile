CC = gcc
CFLAGS = -Wall -std=c11
SRCS = main.c f_serial.c f_coada.c f_lista.c f_stiva.c
OBJS = $(SRCS:.c=.o)
MAIN = tema2

.PHONY: clean run build

build: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) -o $(MAIN) $(OBJS) $(CFLAGS)

run: build
	./$(MAIN)
	
$(OBS): %.o %.c  
	$(CC) $< $(CFLAGS) -c -o $@
	
clean:
	$(RM) ./*.o *~ $(MAIN)
