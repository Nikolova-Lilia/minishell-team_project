# minishell-team_project
the repo for the team project minishell

Minishell project - notes on the project:

The project implementation is split into two basic parts -  first part is the part that deals with user input and user interaction, like commands and signals. 
The second part is where the internal work is done (the execution).
The first part has to deal with the command (user input as a line/string) or a signal (ctrl + C, etc…).
The way bash parses commands, it basically goes through two phases, the lexical analysis (lexing) which produces “lexems” and then the syntax analysis (parsing the lexems).
Lexical analysis / tokenization: taking the input from the user and processing it char by char into “tokens”.
For example, let’s say we are building a calculator program and we have 1 + 2 as input, we can convert this input to a stream of tokens similar to the following:
![image](https://github.com/Nikolova-Lilia/minishell-team_project/assets/147531481/f8f0c3f8-2283-40eb-89e6-80e386d8e7fe)

A graphical representation of tokens
As you can see, the tokens have a type and value, for example, the first token from the left is of type NUMBER and has a value of 1.
