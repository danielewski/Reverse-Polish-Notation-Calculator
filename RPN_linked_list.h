#ifndef RPN_LINKED_LIST_H
#define RPN_LINKED_LIST_H

#include <stdbool.h>

typedef struct RPN_Node_struct {
  bool is_operator;
  char operator;
  double value;
  struct RPN_Node_struct *next;
} RPN_Node;

// Basic linked list operations
double pop(RPN_Node **head);
void push(RPN_Node **head, double value);

void print_list(RPN_Node *head);
void print_node(RPN_Node *node);
void free_list(RPN_Node *head);

// RPN Specific operations
bool is_operator(char c);
RPN_Node * prompt_and_parse(void);
void print_as_infix(RPN_Node *head);

#endif
