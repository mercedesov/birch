/* books1.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct s_book {
  char title[64];
  int pages;
  struct s_book *next;
};
typedef struct s_book Book;

Book *first;

void addbook(char *title, int pages)
{
  Book *p, *new;

  if (!first)
  {
    p = malloc(sizeof(Book));
    memset(p, 0, sizeof(Book));
    strncpy(p->title, title, 63);
    p->pages = pages;
    p->next = 0;
    first = p; 

    return;
  }

  for (p=first; p->next; p++);
  
  new = malloc(sizeof(Book));
  memset(new, 0, sizeof(Book));
  strncpy(new->title, title, 63);
  new->pages = pages;
  new->next = 0;

  p->next = new;

  return;
}


void listbooks(char *searchstr)
{
  Book *p;

  for (p = first; p; p = p->next)
    if (!searchstr || !strcmp(searchstr, p->title))
      printf("Pages: %d\tTitle: '%s'\n",
          p->pages, p->title);

  return;
}


int main()
{
  first = 0;

  addbook("The Bible", 3500);
  addbook("Dice man", 340);
  addbook("Pippi Longstockings", 119);
  addbook("Tintin", 350);
  addbook("Around the world in 80 days", 290);

  listbooks(0);
  return 0;
}
    
