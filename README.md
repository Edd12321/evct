# What?
This is a modal terminal text editor, written in pure C++. That means it doesn't use a GUI, but rather a text-based interface. It was all done from scratch, *without* the need of curses.h.

"evct" stands for Edd's Very Cool Texteditor. No particular reason for choosing that name, it just sounded nice. It's a modal text editor that supports "primary modes" (INSERT, VISUAL, NORMAL) and "secondary modes" (which I plan to add as plug-ins in the near future).

# How?
I wrote all the important subroutines in `editor.cpp`. This is where most of the TUI magic happens. What all the code really boils down to is just some ANSI/VT100 escape sequences and a little bit of creativity.

Below, you can see a list of all currently supported commands:

| Command | What it does                             |
|---------|------------------------------------------|
| :w      | Writes to the currently opened text file |
| :wq     | Also writes, but then exits the program  |
| :q!     | Quits the program without saving         |
| I (i)   | Enters -- INSERT -- pmode                |
| V (v)   | Enters -- VISUAL -- pmode                |
| Esc     | Enters -- NORMAL -- pmode                |
| arrows  | Navigate around the opened buffer        |
| :set    | Sets a variable/smode to 1 / 0           |

| Env variable     | What it does                  |
|------------------|-------------------------------|
|:set linum        | Enables/disables line numbers |

# Issues
* On Windows, evct flickers a lot
* Also on Windows, you have to exit insert mode to use the arrow keys
