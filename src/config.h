//The separator for definable fields, default ~
#define SEPARATOR ~
//Number of commands to run once files are generated
#define NUM_COMMANDS 2
//array of commands to be run, undefined behavior if size != NUM_COMMANDS
//enter as a comma separated list with commands in quotes
char* commands[NUM_COMMANDS] = {"ls", "ls -a"};