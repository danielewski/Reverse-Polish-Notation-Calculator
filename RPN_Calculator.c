#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "RPN_linked_list.c"

double evaluate_rpn_expression(RPN_Node *head);

int main() {
  RPN_Node *head = prompt_and_parse();

  printf("Interpreting input as: ");
  print_as_infix(head);

  double result = evaluate_rpn_expression(head);

  printf("Result: %.2lf\n", result);

  free_list(head);
  head = NULL;

  return 0;
}

double evaluate_rpn_expression(RPN_Node *head) {
	RPN_Node *copy = malloc(sizeof(RPN_Node));
	copy->next = NULL;
	double val1;
	double val2;
	while(head!=NULL){
		
		if(head->is_operator){
		val2 = pop(&copy);		
		val1 = pop(&copy);
		if(head->operator == '+'){
			push(&copy, val1+val2);
		}
		if(head->operator == '-'){
			push(&copy, val1-val2);
		}
		if(head->operator == '*'){
			push(&copy, val1*val2);
		}
		if(head->operator == '/'){
			if(val2 == 0){
				printf("Error: Requested division by 0\n");
				return 0;
			}
			push(&copy, val1/val2);
		}
		if(head->operator == '^'){
			if(val1 ==0 && val1==0){
				printf("Error: 0^0 is indeterminate\n");
				return 0;
			}
			push(&copy, pow(val1,val2));
		}
		}else{
		push(&copy, head->value);
		}
		
		head = head->next;
		
	}
	return copy->value;
  // This function should walk through the linked list specified by head.
  // It should also declare another linked list, named stack.
  
  //
  // For a term in the input list, it should check if it is a value or
  // an operator. If it is a value, it should push it on the stack.
  // If it is an operator, it should pop two terms off the stack and
  // use those as the operands to the operator. Be careful of the order of
  // the terms since for some operators that matters.
  //
  // After processing the list, the stack will have one element, which is
  // the answer. Store it in a double, free the stack and return the answer.
  return 0;
}