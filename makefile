CFILES = src/report_generator.cpp src/argument_handler.cpp
OFILES = build/report_generator.o build/argument_handler.o
FLAGS = -std=c++11 -g
DEBUG = -g -Wall -Werror
LINKS = 

all: $(CFILES)
	g++ -c $(FLAGS) src/report_generator.cpp -o build/report_generator.o
	g++ -c $(FLAGS) src/argument_handler.cpp -o build/argument_handler.o
	g++ -o out $(FLAGS) $(OFILES)

debug: $(CFILES)
	g++ -c $(FLAGS) $(DEBUG) src/report_generator.cpp -o build/report_generator.o
	g++ -c $(FLAGS) $(DEBUG) src/argument_handler.cpp -o build/argument_handler.o
	g++ -o out $(FLAGS) $(OFILES)

clean:
	rm build/*
	rm out
	rm *.pdf
	rm *.rtf