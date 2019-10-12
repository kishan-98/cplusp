#include "cplusp.h"

map<string, int> id_table;

operator_node::operator_node(expression_node *left, expression_node *right){
	leftNode 	= left;
	rightNode 	= right;
}


plus_node::plus_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void plus_node::print(){
	cout << "<# "; leftNode->print(); cout << " + "; rightNode->print(); cout << " #>";
	return;
}
int plus_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger + rightValueInteger);
	cout << "\t<$ plus_node: " << leftValueInteger << " + " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


minus_node::minus_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void minus_node::print(){
	cout << "<# "; leftNode->print(); cout << " - "; rightNode->print(); cout << " #>";
	return;
}
int minus_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger - rightValueInteger);
	cout << "\t<$ minus_node: " << leftValueInteger << " - " << rightValueInteger << " = " << valueInteger << " $>" << endl;
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


multiply_node::multiply_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void multiply_node::print(){
	cout << "<# "; leftNode->print(); cout << " * "; rightNode->print(); cout << " #>";
	return;
}
int multiply_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger * rightValueInteger);
	cout << "\t<$ multiply_node: " << leftValueInteger << " * " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


divide_node::divide_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void divide_node::print(){
	cout << "<# "; leftNode->print(); cout << " / "; rightNode->print(); cout << " #>";
	return;
}
int divide_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger / rightValueInteger);
	cout << "\t<$ divide_node: " << leftValueInteger << " / " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


modulo_node::modulo_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void modulo_node::print(){
	cout << "<# "; leftNode->print(); cout << " % "; rightNode->print(); cout << " #>";
	return;
}
int modulo_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger % rightValueInteger);
	cout << "\t<$ modulo_node: " << leftValueInteger << " % " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


logand_node::logand_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void logand_node::print(){
	cout << "<# "; leftNode->print(); cout << " && "; rightNode->print(); cout << " #>";
	return;
}
int logand_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger && rightValueInteger);
	cout << "\t<$ logand_node: " << leftValueInteger << " && " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


logor_node::logor_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void logor_node::print(){
	cout << "<# "; leftNode->print(); cout << " || "; rightNode->print(); cout << " #>";
	return;
}
int logor_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger || rightValueInteger);
	cout << "\t<$ logor_node: " << leftValueInteger << " || " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


or_node::or_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void or_node::print(){
	cout << "<# "; leftNode->print(); cout << " | "; rightNode->print(); cout << " #>";
	return;
}
int or_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger | rightValueInteger);
	cout << "\t<$ or_node: " << leftValueInteger << " | " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


xor_node::xor_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void xor_node::print(){
	cout << "<# "; leftNode->print(); cout << " ^ "; rightNode->print(); cout << " #>";
	return;
}
int xor_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger ^ rightValueInteger);
	cout << "\t<$ xor_node: " << leftValueInteger << " ^ " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


and_node::and_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void and_node::print(){
	cout << "<# "; leftNode->print(); cout << " & "; rightNode->print(); cout << " #>";
	return;
}
int and_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger & rightValueInteger);
	cout << "\t<$ and_node: " << leftValueInteger << " & " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


eq_node::eq_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void eq_node::print(){
	cout << "<# "; leftNode->print(); cout << " == "; rightNode->print(); cout << " #>";
	return;
}
int eq_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger == rightValueInteger);
	cout << "\t<$ eq_node: " << leftValueInteger << " == " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


ne_node::ne_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void ne_node::print(){
	cout << "<# "; leftNode->print(); cout << " != "; rightNode->print(); cout << " #>";
	return;
}
int ne_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger != rightValueInteger);
	cout << "\t<$ ne_node: " << leftValueInteger << " != " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


gt_node::gt_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void gt_node::print(){
	cout << "<# "; leftNode->print(); cout << " > "; rightNode->print(); cout << " #>";
	return;
}
int gt_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger > rightValueInteger);
	cout << "\t<$ gt_node: " << leftValueInteger << " > " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


ge_node::ge_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void ge_node::print(){
	cout << "<# "; leftNode->print(); cout << " >= "; rightNode->print(); cout << " #>";
	return;
}
int ge_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger >= rightValueInteger);
	cout << "\t<$ ge_node: " << leftValueInteger << " >= " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


lt_node::lt_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void lt_node::print(){
	cout << "<# "; leftNode->print(); cout << " < "; rightNode->print(); cout << " #>";
	return;
}
int lt_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger < rightValueInteger);
	cout << "\t<$ lt_node: " << leftValueInteger << " < " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


le_node::le_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void le_node::print(){
	cout << "<# "; leftNode->print(); cout << " <= "; rightNode->print(); cout << " #>";
	return;
}
int le_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger <= rightValueInteger);
	cout << "\t<$ le_node: " << leftValueInteger << " <= " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


lshift_node::lshift_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void lshift_node::print(){
	cout << "<# "; leftNode->print(); cout << " << "; rightNode->print(); cout << " #>";
	return;
}
int lshift_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger << rightValueInteger);
	cout << "\t<$ lshift_node: " << leftValueInteger << " << " << rightValueInteger << " = " << valueInteger << " $>" << endl;
	return valueInteger;
}


rshift_node::rshift_node(expression_node *left, expression_node *right) : operator_node(left, right) {
}
void rshift_node::print(){
	cout << "<# "; leftNode->print(); cout << " >> "; rightNode->print(); cout << " #>";
	return;
}
int rshift_node::evaluate(){
	int leftValueInteger 	=	leftNode->evaluate();
	int	rightValueInteger 	=	rightNode->evaluate();
	valueInteger = (leftValueInteger >> rightValueInteger);
	cout << "\t<$ rshift_node: " << leftValueInteger << " >> " << rightValueInteger << " = " << valueInteger << " $>" << endl;
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
