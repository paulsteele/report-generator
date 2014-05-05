CFILES = src/report_generator.c src/argument_handler.c
OFILES = build/report_generator.o build/argument_handler.o
FLAGS = -std=gnu99
DEBUG = -g -Wall -Werror
LINKS = 

all: $(CFILES)
	gcc -c $(FLAGS) src/report_generator.c -o build/report_generator.o
	gcc -c $(FLAGS) src/argument_handler.c -o build/argument_handler.o
	gcc -o out $(FLAGS) $(OFILES)

debug: $(CFILES)
	gcc -c $(FLAGS) $(DEBUG) src/report_generator.c -o build/report_generator.o
	gcc -c $(FLAGS) $(DEBUG) src/argument_handler.c -o build/argument_handler.o
	gcc -o out $(FLAGS) $(OFILES)

clean:
	rm build/*.o
	rm out
