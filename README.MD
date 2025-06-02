Rough (and probably very innacurate) outline of the project structure:
![Parsing Structure](https://accu.org/journals/overload/26/145/balaam_2510/1.png) \
1. Lexer -> Parser -> executable structure -> executor
2. separate module for builtins
3. environment variables

##Needs

- prompt awaiting new command
- working history
- search and launch right executable (Path both absolute and relative)
- at most 1 global variable to indicate received signal (using only PID and no access to additional data)
- Not interpret unclosed quotes or special characters which are not required by the
subject such as \ (backslash) or ; (semicolon)
- handle single quote which should prevent the shell from interpreting the meta-
characters in the quoted sequence
- handle double quotes, ,,  



##Redirections

- < should redirect input
- > should redirect output
- << should be given a delimeter, then read the input until a line containing the delimeter is seen. NB: does not update the history!
- >> should redirect output in append mode 


##Other functions

- implement pipes | (also think of single pipe). 
- handle environment variables ($ followed by sequence of characters) which should expand to their values
- handle $? shich should expand to the exit status of the most recent exec foreground pipeline
- handle cntrl-c, cntrl-d and cntrl-\ (behave like bash)
- In interactive mode: 
    - ctrl-C displays a new prompt on a new line.
    - ctrl-D exits the shell.
    - ctrl-\ does nothing.

Your shell must implement the following built-in commands:
    - echo with option -n
    - cd with only a relative or absolute path
    - pwd with no options
    - export with no options
    - unset with no options
    - env with no options or arguments
    - exit with no options


Useful links: \
https://en.wikipedia.org/wiki/Lexical_analysis \
https://en.wikipedia.org/wiki/Abstract_syntax_tree \
https://www.gnu.org/software/termutils/manual/termcap-1.3/html_mono/termcap.html

More TBD
