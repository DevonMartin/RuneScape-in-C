# RuneScape in C
#### Video Demo:  [YouTube](https://youtu.be/h1mt6AdUYqM/)
#### Description:
RuneScape in C is a text-based recreation of the video game that originally inspired me to learn how to code back in the late 2000s/early 2010s.
It is, in it's current state, only able to fully perform the functions of what's available in the spawn room of the game. However, the basic
framework is created to allow it to grow quickly. At time of project submission, this project contains 1,555 lines of code across 12 files, an
imported sqlite3 .c file, and a relational database with 3 linked tables storing user/character data.

runescape.c is where `int main(void)` lives, and runs every possible function in the game. It starts by logging in, initializing some data for the
game, and then prompting the user for a function to run. It reloads character data after every function to pass it in to `do_something()` so that
any new data from previous functions is accounted for. 

login.c is where `login()` and other supporting functions live. This feature alone is extensive and has many bug checks, condition checks, repeating
code, etc. This is why it is it's own file. It utilizes SQLite for data storage. It allows both the creation of new accounts based on the
availability of a username, plus the correct format of a username and a password, and the logging in of existing accounts based on the accurate
input of a matching username and password. It retrieves the character ID and passes it into runescape.c to be used to retrieve and update data
during the playing of the game. 

game_functions.c is where most functions ran by the game itself live, such as clearing the screen during dialogue and requiring user input to
proceed. It is it's own file because it is specifically meant to be things the game requires in order to behave as I would like, but not
functions that the player would ever use alone.

player_commands.c is where `int do_something()` lives, which is the function that is called every time the player responds to the prompt "What would
you like to do?" Most of the commands are programmed directly into `int do_something()`, but some are separated due to their complexity. I believe
leaving the majority of the code within the single function makes it easier to follow. Availability of functions is determined by character data
such as location and whether a dialogue has been executed previously. 

ids.c is where the functions involving item and object IDs live. It includes initialization of items and objects into arrays of functions for speed
and simplicity of finding and executing commands based off of IDs. All IDs are the actual in-game IDs of the items and objects in RuneScape. This
file is separate because of the extensive number of items and objects within the game. Currently, it is limited to only a handful, but if development
of the game continued, it would get long fast. 

global.c is where several global variables and definitions live, to allow usage of these things in every file. The lack of having to pass extra
arguments into functions is why I have used global variables for some things and not others. 