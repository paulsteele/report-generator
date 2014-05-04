CFILES = src/report_generator.c
HFILES = src/report_generator.h
OFILES = build/report_generator.o
FLAGS = -std=gnu99
DEBUG = -g -Wall -Werror
LINKS = 

all: $(CFILES) $(HFILES)
	gcc -c $(FLAGS) $(CFILES) -o $(OFILES)
	gcc -o out $(FLAGS) $(OFILES)

debug: $(CFILES) $(HFILES)
	gcc -c $(FLAGS) $(DEBUG) $(CFILES) -o $(OFILES)
	gcc -o out $(FLAGS) $(OFILES)

clean:
	rm build/*.o
	rm out
