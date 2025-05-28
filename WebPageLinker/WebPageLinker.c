#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


/*
 * File: linked.c
 * Author: Chance Krueger
 * Purpose: Implements a graph-based model to represent web pages and their links. 
 *          The program processes commands to add pages, create links, and check 
 *          connectivity between pages.
 */



/*
 * page -- Represents a web page in a directed graph. 
 * Each page has a unique name and may contain links to other pages. 
 * It includes a linked list of outgoing links for efficient graph traversal. 
 * The `next` pointer links to the next page in the list, 
 * while `edges` points to the list of outgoing links. 
 * The `visited` flag is used to track whether the page has been visited during traversal.
 */
struct page {


	char *name;
	struct page *next;
	struct link *edges;
	int visited;
};



/*
 * link -- Represents a directed edge in a graph, connecting one page to another.  
 * The `to` pointer references the destination page of the link.  
 * The `next` pointer links to the next link in the adjacency list,  
 * allowing multiple outgoing links from a single page to be stored efficiently.
 */
struct link {


	struct page *to;
	struct link *next;
};



struct page *graphHead = NULL;



/*
* addPageToGraph(node) -- adds a new page node to the linked list graph.
* It checks if the graph is empty and initializes the head if necessary.
* If a page with the same name already exists, it prints an error and returns 1.
* Otherwise, it appends the new page to the end of the list and returns 0.
*/
int addPageToGraph(struct page *node) {

	if (graphHead == NULL) {
		graphHead = node;
		return 0;
	}

	struct page *cur = graphHead;
	struct page *behind = malloc(sizeof(struct page));
	
	if (behind == NULL) {
		fprintf(stderr, "Ran Out Of Memory.\n");
		return 1;
	}
	
	while (cur != NULL) {

		if (strcmp(node->name, cur->name) == 0) {
			fprintf(stderr, "There is already a Page with that name.\n");
			return 1;
		}

		behind = cur;
		cur = cur->next;
	}

	behind->next = node;
	return 0;
}




/*
* addLinkToPage(srcPage, link) -- creates a link between two pages in the graph.  
* It searches for the source page and the destination page in the linked list.  
* If either page is not found, it prints an error and returns 1.  
* Otherwise, it allocates memory for a new link structure and appends it  
* to the list of links for the source page. Returns 0 on success.  
*/
int addLinkToPage(char *srcPage, char *link) {


	struct page *src = malloc(sizeof(struct page));

	if (src == NULL) {
                fprintf(stderr, "Ran Out Of Memory.\n");
                return 1;
        }

	struct page *linkNode = malloc(sizeof(struct page));

        if (linkNode == NULL) {
                fprintf(stderr, "Ran Out Of Memory.\n");
                return 1;
        }

	struct page *cur = graphHead;

	int srcSeen = 0;
	int linkSeen = 0;

	while (cur != NULL) {

		int srcComp = strcmp(srcPage, cur->name);
		int linkComp = strcmp(link, cur->name);

		if (srcComp == 0) {
			src = cur;
			srcSeen = 1;
		} 

		if (linkComp == 0) {
			linkNode = cur;
			linkSeen = 1;
		}

		cur = cur->next;
	}

	if (srcSeen == 0 || linkSeen == 0) {
		fprintf(stderr, "Could not Find the link.\n");
		return 1;
	}


	struct link *linkNodeAct = malloc(sizeof(struct link));

	if (link == NULL) {
		fprintf(stderr, "Ran Out Of Memory.\n");
                return 1;
	}

	linkNodeAct->to = linkNode;
	linkNodeAct->next = NULL;

	struct link *linkCur = src->edges;
	
	struct link *behind = malloc(sizeof(struct link));

        if (behind == NULL) {
                fprintf(stderr, "Ran Out Of Memory.\n");
                return 1;
        }

	if (linkCur == NULL) {
		src->edges = linkNodeAct;
		return 0;
	}

	while (linkCur != NULL) {
		behind = linkCur;
		linkCur = linkCur->next;
	}

	behind->next = linkNodeAct;

	return 0;
}



/*
* findNode(name) -- searches for a page in the graph by its name.  
* It traverses the linked list of pages, comparing each page's name to the given name.  
* If a match is found, it returns a pointer to the corresponding page.  
* If no match is found, it returns NULL.  
*/
struct page * findNode(char *name) {

	struct page *node = NULL;

	struct page *cur = graphHead;

	while (cur != NULL) {

		int isWord = strcmp(cur->name, name);
		
		if (isWord == 0) {
			node = cur;
			break;
		}
	cur = cur->next;
	}
	return node;
}



/*
* dfs(fromPage, toPage) -- performs a depth-first search (DFS) to check if there is a path from fromPage to toPage.
* It starts at fromPage, marking it as visited, and recursively explores its outgoing links to other pages.
* If it encounters toPage during the traversal, it returns 1, indicating that a path exists.
* If all possible links are explored and toPage is not found, it returns 0.
* The function assumes that visited is a field in the struct page to track the nodes that have been visited.
*/
int dfs(struct page *fromPage, struct page *toPage) {

	if (strcmp(fromPage->name, toPage->name) == 0) {
		return 1;
	}

	if (fromPage->visited == 1) {
		return 0;
	}

	fromPage->visited = 1;

	struct link *curLink = fromPage->edges;
    	while (curLink != NULL) {
        	if (dfs(curLink->to, toPage)) {
            		return 1;
        	}
        	curLink = curLink->next;
    	}
	return 0;
}



/*
* resetVisits() -- resets the 'visited' field of all pages in the graph to 0.
* It traverses the entire list of pages starting from graphHead and sets the 'visited' field of each page to 0.
* This function is typically used to prepare for a new search or traversal, ensuring that all pages are marked as unvisited before starting a new operation.
*/
void resetVisits() {
        struct page *cur = graphHead;
        while (cur != NULL) {
		cur->visited = 0;
                cur = cur->next;
        }
}



/*
* printConnection(pageOne, pageTwo) -- prints 1 if there is a path of links connecting pageOne to pageTwo, 
* otherwise prints 0. It uses depth-first search (DFS) to determine if a path exists between the two pages. 
* It first finds the pages corresponding to pageOne and pageTwo using the findNode function. 
* Then, it calls the dfs function to check if pageOne is connected to pageTwo. 
* After performing the search, it resets the 'visited' status of all pages to ensure the graph is ready for subsequent operations. 
* Assumes that the pages pageOne and pageTwo exist in the graph.
*/
void printConnection(char *pageOne, char *pageTwo) {


	struct page *nodeOne = findNode(pageOne);
	struct page *nodeTwo = findNode(pageTwo);


	printf("%d\n", dfs(nodeOne, nodeTwo));
	resetVisits();
}

/*
* findPage(pageName) -- returns 0 if the page with the name 'pageName' is found in the graph, 
* otherwise returns 1. It iterates through the list of pages starting from the head of the graph 
* and compares each page's name with the given 'pageName'. If a match is found, the function breaks out 
* of the loop and returns 0. If no match is found after traversing all pages, it returns 1. 
* Assumes that the graph is correctly initialized and contains pages.
*/
int findPage(char *pageName) {

	struct page *cur = graphHead;
	int found = 1;

        while (cur != NULL) {
		if (strcmp(cur->name, pageName) == 0) {
			found = 0;
			break;
		}
		cur = cur->next;
	}	
	return found;
}



/*
* removeAllWhitespace(str) -- removes all whitespace characters (spaces, tabs, newlines) 
* from the string 'str'. It iterates over the string, copying non-whitespace characters 
* to the front of the string and updating the string to exclude whitespace. After processing, 
* the string is null-terminated at the correct position. 
* Assumes that 'str' is a valid, null-terminated string.
*/
void removeAllWhitespace(char *str) {
    int i = 0, j = 0;
    while (str[i] != 0) {
        if (!isspace((unsigned char) str[i])) { // Skip all whitespace (tabs, spaces, newlines)
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0'; // Null-terminate cleaned string
}



/*
 * freeMemory() -- Frees all dynamically allocated memory for the graph structure. 
 * It iterates over all the pages in the graph, freeing the memory allocated for 
 * each page’s name and its associated edges (links). For each page, it also 
 * frees the memory allocated for the links and then the page itself. 
 * Assumes that 'graphHead' is a valid pointer to the first page in the graph.
 */
void freeMemory() {

	struct page *curPage = graphHead;

	while (curPage != NULL) {

		struct link *curLink = curPage->edges;

		while (curLink != NULL) {
		
			struct link *tempNode = curLink;

			curLink = curLink->next;
			free(tempNode);
		}
		free(curPage->name);
		struct page *tempPage = curPage;
		curPage = curPage->next;
		free(tempPage);
	}
}


/*
* main(argc, argv) -- the entry point of the program. It processes command-line arguments to either 
* read from a file (if a file path is provided) or from stdin (if no file is specified). It expects one 
* of three actions: adding pages to a graph (@addPages), adding links between pages (@addLinks), or checking 
* if two pages are connected (@isConnected). The function parses each line of input, processes actions and 
* their arguments, and executes the appropriate graph manipulation. It returns 0 if no errors are encountered, 
* and 1 if there are errors (such as memory allocation failure, invalid input, or pages not found).
* Assumptions: The function assumes that input is well-formed according to the expected format and that 
* graph manipulation functions (like addPageToGraph and addLinkToPage) handle memory and edge cases correctly.
*/
int main(int argc, char* argv[]) {


        int errSeen = 0;
        FILE *input;

        if (argc > 1) {

                input = fopen(argv[1], "r");

                if (input == NULL) {
                        fprintf(stderr, "Couldn't open the file given.\n");
                        return 1;
                }

                if (argc > 2) {
                        fprintf(stderr, "Too many argumnets were given.\n");
                        errSeen = 1;
                }
        } else {
                input = stdin;
        }

        char *line = NULL;
        size_t len = 0;

        while (getline(&line, &len, input) > 0) {

                if (line == NULL) {
                        fprintf(stderr, "Not enough Space in memory.\n");
                        return 1;
                }

		char *action = NULL;

                char **pageLinks = malloc(strlen(line) * sizeof(char*));
		if (pageLinks == NULL) {
                        fprintf(stderr, "Ran Out Of Memory.\n");
                        return 1;
                }
		// Make sure that everything is initialized before setting into String list 
		for (int i = 0; i < strlen(line); i++) {
    			pageLinks[i] = NULL;
		}

                char *word = strtok(line, " \t\n");
                int index = 0;
                int pntrIndex = 0;

                int addPages = 1;
                int addLinks = 1;
                int isConnected = 1;

                while (word != 0) {

                        if (word[strlen(word) - 1] == '\n') {
                                word[strlen(word) - 1] = 0;
                        }

                        removeAllWhitespace(word);

                        if (index == 0) {

                                addPages = strcmp(word, "@addPages");
                                addLinks = strcmp(word, "@addLinks");
                                isConnected = strcmp(word, "@isConnected");

                                if (addPages == 0 || addLinks == 0 || isConnected == 0) {
                                        if (action != NULL) {
						free(action);
						action = NULL;
					}
					action = strdup(word);
                                        index = 1;
                                } else {
                                        fprintf(stderr, "Invalid Input.");
                                        errSeen = 1;
                                        break;
                                }

                        } else {

                                pageLinks[pntrIndex] = strdup(word);
                                pntrIndex++;

                        }


                        // LAST THING BEFORE END OF WHILE LOOP, WILL GO TO NEXT WORD IN LINE
                        word = strtok(NULL, " ");

                }

                if (action != NULL && action[0] != 0) {

                        // CHECK IF ACTION IS NULL, IF SO -> DONT MAKE STRUCTS
                        if (addPages == 0) {
                                int i = 0;
                                while (pageLinks[i] != 0) {
                                        char *pageWord = pageLinks[i];

                                        struct page *curNode = malloc(sizeof(struct page));
                                        curNode->name = pageWord;
                                        curNode->next = NULL;
                                        curNode->edges = NULL;
                                        curNode->visited = 0;


                                        errSeen += addPageToGraph(curNode);
                                        i++;

                                }

                        } else if(addLinks == 0) {

                                if (pageLinks[0] != 0 && pageLinks[1] != 0) {
                                        char *srcPage = NULL;
                                        if (pageLinks[0] != 0) {
                                                srcPage = strdup(pageLinks[0]);
                                        }

                                        int i = 1;

                                        while (pageLinks[i] != 0) {
                                                errSeen += addLinkToPage(srcPage, pageLinks[i]);
                                                i++;
                                        }
                                } else {
                                        if (pageLinks[0] == 0) {
                                                fprintf(stderr, "No Arguments were given in @addLinks");
                                                errSeen = 1;
                                        }
                                }

                        } else if (isConnected == 0) {

                                // CHECK HOW MANY ARGS WERE GIVEN < 2 or > 2 -> stderr, dont check if connected
                                if (pageLinks[0] == 0 || pageLinks[1] == 0 || pageLinks[2] != 0) {
                                        errSeen += 1;
                                        fprintf(stderr, "Either too many or too few arguments given.\n");
                                        // CHECK IF PAGES ARE REAL
                                } else {

                                        int pageOne = findPage(pageLinks[0]);
                                        int pageTwo = findPage(pageLinks[1]);

                                        if (pageOne != 0 || pageTwo != 0) {
                                                errSeen++;
                                                fprintf(stderr, "Either Page does not Exist.\n");
                                        } else {

                                                // CALL printConnection(2 strings)
                                                printConnection(pageLinks[0], pageLinks[1]);
                                        }
                                }

                        }

                }
		free(action);
		action = NULL;
		free(pageLinks);
		pageLinks = NULL;

        }
	free(line);
        if (input != stdin) {
                fclose(input);
        }
        freeMemory();
        return errSeen >= 1;
}
