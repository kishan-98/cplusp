#include "cplusp.h"

map<string, int> id_table;

operator_node::operator_node(expression_node *left, string op, expression_node *right){
	leftNode 		=	left;
	operatorNode	=	op;
	rightNode 		=	right;
}
void operator_node::print(){
	cout << "<# "; leftNode->print(); cout << " " << operatorNode << " "; rightNode->print(); cout << " #>";
	return;
}
int operator_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	if(operatorNode == "+")valueInteger = (leftValueInteger + rightValueInteger);
	if(operatorNode == "-")valueInteger = (leftValueInteger - rightValueInteger);
	if(operatorNode == "*")valueInteger = (leftValueInteger * rightValueInteger);
	if(operatorNode == "/")valueInteger = (leftValueInteger / rightValueInteger);
	if(operatorNode == "%")valueInteger = (leftValueInteger % rightValueInteger);
	if(operatorNode == "||")valueInteger = (leftValueInteger || rightValueInteger);
	if(operatorNode == "&&")valueInteger = (leftValueInteger && rightValueInteger);
	if(operatorNode == "|")valueInteger = (leftValueInteger | rightValueInteger);
	if(operatorNode == "^")valueInteger = (leftValueInteger ^ rightValueInteger);
	if(operatorNode == "&")valueInteger = (leftValueInteger & rightValueInteger);
	if(operatorNode == "==")valueInteger = (leftValueInteger == rightValueInteger);
	if(operatorNode == "!=")valueInteger = (leftValueInteger != rightValueInteger);
	if(operatorNode == ">")valueInteger = (leftValueInteger > rightValueInteger);
	if(operatorNode == ">=")valueInteger = (leftValueInteger >= rightValueInteger);
	if(operatorNode == "<")valueInteger = (leftValueInteger < rightValueInteger);
	if(operatorNode == "<=")valueInteger = (leftValueInteger <= rightValueInteger);
	if(operatorNode == "<<")valueInteger = (leftValueInteger << rightValueInteger);
	if(operatorNode == ">>")valueInteger = (leftValueInteger >> rightValueInteger);
	cout << "\t<$ operator_node: " << leftValueInteger << " " << operatorNode << " " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


unary_minus_node::unary_minus_node(expression_node *exp_node) : expNode(exp_node) {
}
void unary_minus_node::print(){
	cout << "<# - "; expNode->print(); cout << " #>";
	return;
}
int unary_minus_node::evaluate(){
	int expValueInteger 	=	expNode->evaluate();
	valueInteger = (-expValueInteger);
	cout << "\t<$ unary_minus_node: - " << expValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


value_node::value_node(int value){
	valueInteger = value;
}
void value_node::print(){
	cout << valueInteger;
	return;
}
int value_node::evaluate(){
	cout << "\t<$ value: " << valueInteger << " $>" << endl;
	return valueInteger;
}


variable_node::variable_node(string id){
	variableID = id;
}
void variable_node::print(){
	cout << variableID;
	return;
}
int variable_node::evaluate(){
	cout << "\t<$ variable: " << variableID << " = " << id_table[variableID] << " $>" << endl;
	return id_table[variableID];
}


assignment_statement_node::assignment_statement_node(string id, expression_node *expr_node) : variableID(id), expressionNode(expr_node) {
}
void assignment_statement_node::print(){
	cout << "<# " << variableID << " = "; expressionNode->print(); cout << " #>" << endl;
}
void assignment_statement_node::evaluate(){
	id_table[variableID] = expressionNode->evaluate();
	cout << "\t<$ assignment_statement: " << variableID << " = " << id_table[variableID] << " $>" << endl;
	return;
}

program_node::program_node(list<statement_node *> *stmt_list): statementList(stmt_list) {
}
void program_node::evaluate(){
	list<statement_node *>::iterator statementListIterator;
	cout << "================================================PRINT+EVALUATE==================================================================" << endl;
	for(statementListIterator = statementList->begin(); statementListIterator != statementList->end(); statementListIterator++){
		(*statementListIterator)->print();
		(*statementListIterator)->evaluate();
	}
	// cout << "================================================EVALUATE========================================================================" << endl;
	// for(statementListIterator = statementList->begin(); statementListIterator != statementList->end(); statementListIterator++){
	// 	// (*statementListIterator)->print();
	// 	(*statementListIterator)->evaluate();
	// }
	// cout << "================================================PRINT===========================================================================" << endl;
	// for(statementListIterator = statementList->begin(); statementListIterator != statementList->end(); statementListIterator++){
	// 	(*statementListIterator)->print();
	// 	// (*statementListIterator)->evaluate();
	// }
	cout << "================================================================================================================================" << endl;
}
