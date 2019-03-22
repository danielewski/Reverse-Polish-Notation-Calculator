#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RPN_linked_list.h"

double pop(RPN_Node **head) {
  // Ensure we can pop
  if (*head == NULL) {
    printf("Error: Attempted to pop NULL pointer\n");
    return 0;
  }

  // Store head so we can free it
  RPN_Node *temp = *head;
  // Store the result so we can return it
  double result = (*head)->value;

  // Advance the head pointer
  *head = (*head)->next;

  // Free the old head pointer
  free(temp);
  return result;
}

void push(RPN_Node **head, double value) {
  // Create a new node and fill it in
  RPN_Node *temp = malloc(sizeof(RPN_Node));
  temp->is_operator = false;
  temp->value = value;
  temp->next = *head;

  // Make the new node the head
  *head = temp;
}

void print_list(RPN_Node *head) {
  while (head != NULL) {
    print_node(head);
    head = head->next;
  }
}

void print_node(RPN_Node *node) {
  if (node->is_operator) {
    printf("op: %c\n", node->operator);
  } else {
    printf("val: %lf\n", node->value);
  }
}

void free_list(RPN_Node *head) {
  while (head != NULL) {
    // Save pointer to next thing to free
    RPN_Node *next = head->next;

    // Free up head
    head->next = NULL;
    free(head);

    // Advance head pointer
    head = next;
  }
}

bool is_operator(char c) {
  switch (c) {
  case '+':
  case '-':
  case '*':
  case '/':
  case '^':
    return true;
  default:
    return false;
  }
}


RPN_Node * prompt_and_parse(void) {
  printf("Please enter an RPN expression: ");

  char buffer[101];
  // Reads up to 100 characters from stdin
  fgets(buffer, 101, stdin);

  // Remove any newlines
  char *term = strpbrk(buffer, "\n\r");
  if (term) {
    *term = '\0';
  }

  // Setup linked list. We will be adding to the tail
  // So we need both head and tail pointers
  RPN_Node *head = NULL;
  RPN_Node *tail = NULL;;

  // Tokenizes buffer by splitting on spaces
  char *token = strtok(buffer, " ");

  // Count operators/values to validate the RPN expression
  int operator_count = 0;
  int value_count = 0;

  while (token) {
    // Declare the next node
    RPN_Node *node = malloc(sizeof(RPN_Node));
    node->next = NULL;

    // Determine the type
    if (strlen(token) == 1 && is_operator(token[0])) {
      // Operator
      node->is_operator = true;
      node->operator = token[0];
      ++operator_count;

      // First two terms must be values
      if (value_count < 2) {
        // Set counts to impossible values so we will clean up the memory
        operator_count = -1;
        value_count = -1;

        // Stop processing terms
        break;
      }
    } else {
      // Treat as a number
      node->is_operator = false;
      node->value = strtod(token, NULL);
      ++value_count;
    }

    // Add node to the list
    if (head == NULL) {
      // First node in list
      head = node;
      tail = node;
    } else {
      // Add after existing tail
      tail->next = node;
      tail = node;
    }

    // Move to the next token
    token = strtok(NULL, " ");
  }

  // For a valid RPN (with only binary operators) the following must be true
  // * If only one number, no operators
  // * If more than one value, first two terms must be values
  // * If two or more values, the number of values must be one more than the the
  //   number of operators
  // The verification of the first 2 terms is handled above
  if (operator_count + 1 != value_count || (value_count == 1 && operator_count != 0)) {
    // Print an error
    printf("Malformed RPN expression. ");
    if (operator_count == -1) {
      printf("First two terms must be numbers.\n");
    } else if (value_count == 1 && operator_count != 0) {
      printf("With only one number, no operators are allowed.\n");
    } else {
      printf("Must have exactly one more number than operator. Currently there "
             "are %d numbers and %d operators\n",
             value_count, operator_count);
      printf("First two terms must be numbers.\n");
    }

    // Cleanup our memory
    free_list(head);
    head = NULL;
  }

  return head;
}

typedef struct RPN_Print_Struct {
  char *content;
  struct RPN_Print_Struct *next;
} RPN_Print;

void push_print(RPN_Print **head, char *str);
void pop_print(RPN_Print **head);

void print_as_infix(RPN_Node *head) {
  RPN_Node *current = head;
  RPN_Print *out = NULL;

  const size_t buffer_sz = 100;
  char buffer[buffer_sz];

  while (current != NULL) {
    if (current->is_operator) {
      // Have an operator
      char *rhs = out->content;
      pop_print(&out);
      char *lhs = out->content;
      pop_print(&out);

      // Build string
      snprintf(buffer, buffer_sz, "(%s %c %s)", lhs, current->operator, rhs);
      char *term = malloc(strlen(buffer) + 1);
      strcpy(term, buffer);
      push_print(&out, term);

      // Cleanup
      free(rhs);
      rhs = NULL;
      free(lhs);
      lhs = NULL;
    } else {
      // Have a value
      // Build string
      snprintf(buffer, buffer_sz, "%.2lf", current->value);
      char *term = malloc(strlen(buffer) + 1);
      strcpy(term, buffer);
      push_print(&out, term);
    }

    // Advance to next term
    current = current->next;
  }

  // Done building, just print and cleanup
  if (out != NULL) {
    printf("%s\n", out->content);
    free(out->content);
    out->content = NULL;
    free(out);
  }
}

void pop_print(RPN_Print **head) {
  RPN_Print *temp = *head;
  *head = (*head)->next;
  free(temp);
}

void push_print(RPN_Print **head, char *str) {
  RPN_Print *temp = malloc(sizeof(RPN_Print));
  temp->next = *head;
  temp->content = str;
  *head = temp;
}

