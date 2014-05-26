#report-generator
##Summer Project 2014

###Goal
The goal of this project to create a tool that can be used to generate rehearsal reports in LaTeX easily for stage management. This tool will however be able to work with a variety of template files.

###Use
This tool allows the user to quickly fill in template source files without altering the actual file. A copy will be made and further action can
then be done on the copy, keeping the source untouched.
Note: A text file named ```input.txt``` will be created after a successful run. This can be redirected into the program to create the same output
files again.

###Instructions
1. Edit ```config``` in the ```src/``` directory to match your liking.
2. Create template files using specified field separators and multi line separators defined in ```config```.
3. Place template files in the ```templates/``` directory.
4. Run the program with desired arguments on the console.(output files will be placed in ```build/``` directory)
5. Follow prompts for inputting replace texts for fields.

###Arguments
```
-f='FILENAME'
```
Specify a file for the program to parse. The file MUST be in the templates directory.
```
-e
```
Authorize the program to execute all file & norm commands without user confirmation.

###Example of a template file
```
\documentclass[12pt] {article}
\begin{document}
Average Runtime: ~AVERAGE RUNTIME~ \\
|Trial Runtime: ~TRIAL RUNTIME~ \\|
\end{document}
```
This could produce the following built file.
```
\documentclass[12pt] {article}
\begin{document}
Average Runtime: 1.22s\\
Trial Runtime: 1.00s \\
Trial Runtime: 1.22s \\
Trial Runtime: 1.44s \\
\end{document}
```
Assuming the ```-e``` flag was used, ```input.txt``` would look like this.
```
1.22s
1.00s
y
1.22s
y
1.44s
n
```

