# WebPageLinker
## Description
This C program builds a directed graph of web pages and their links from input commands. It adds pages and links, then uses depth-first search to check if one page can reach another via links. It outputs 1 if connected, 0 if not. Errors are reported but don’t stop processing. Input is from a file or stdin.


## What I learned: 
    - How to represent a directed graph using adjacency lists in C with structs and pointers.
    - How to parse structured input line-by-line and handle different commands.
    - Implementing depth-first search (DFS) to determine connectivity between nodes.
    - Managing dynamic data structures with WebPageLinker lists for both pages and links.
    - Handling error conditions gracefully, differentiating fatal vs nonfatal errors.
    - Writing a Makefile for compilation with proper compiler flags.
    - Basics of memory management and, optionally, freeing allocated memory

## How To run it:
### Compile the program using a C compiler, for example:
    - Open your terminal in the folder containing WebPageLinker.c and your Makefile, then type:
        make
    - This will create an executable called WebPageLinker.
    
### Run the executable from the command line:
#### You can run the program in two ways:

##### a) Using an input file
    If you have a file called, say, input.txt with the commands (like @addPages, @addLinks, @isConnected), run:

        ./WebPageLinker input.txt

    The program will read commands from input.txt.

##### b) Using standard input (typing commands manually)
    If you want to enter commands manually, run:
    
        ./WebPageLinker
    
    Then type commands line by line, for example:

        @addPages myPage UofA csDept localTheater
        @addLinks myPage UofA
        @isConnected myPage localTheater

    When you’re done, press Ctrl+D (EOF) to end input.


## Future Improvements
    - Use Breadth-First Search instead of Depth-First Search for faster shortest-path detection and to avoid stack overflow on very large graphs.
    - Support weighted links
    - Add command to remove pages or links
    - Cycle Detection
