#Makefile cree par Vincent Boyer
#Septembre 2015

#definition des commandes
CC = gcc
RM = rm

#options du compilateur
#CFLAGS = -I/opt/local/include
#LDFLAGS = -L/opt/local/lib -lmutex_trains

#fichiers et dossiers
PROGNAME = mutex_trains
#HEADERS= readwritepng.h
SRC = $(wildcard *.c) 
OBJ = $(SRC:.c=.o)

all: $(PROGNAME)

$(PROGNAME) : $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

%.o : %.c
	$(CC) $< -c


clean:
	$(RM) -f $(OBJ) *~ $(PROGNAME)

