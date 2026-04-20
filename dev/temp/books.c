#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "books.h"

void print_book_info(Book book) {
    printf("Title: %s\n", book.title);
    printf("Author: %s\n", book.author);
    printf("ISBN: %s\n", book.isbn);
}