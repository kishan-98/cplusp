#include "cplusp.h"

map<string, dtype> id_table;

operator_node::operator_node(expression_node *left, string op, expression_node *right){
	leftNode 		=	left;
	operatorNode	=	op;
	rightNode 		=	right;
}
void operator_node::print(){
	cout << "<# "; leftNode->print(); cout << " " << operatorNode << " "; rightNode->print(); cout << " #>";
	return;
}
dtype operator_node::evaluate(){
	dtype	leftDataNode 	=	leftNode->evaluate();
	dtype	rightDataNode 	=	rightNode->evaluate();
	dataNode = leftDataNode.operate(operatorNode, rightDataNode);
	cout << "\t<$ operator_node: "; leftDataNode.print(); cout << " " << operatorNode << " "; rightDataNode.print(); cout << " = "; dataNode.print(); cout << " $>" << endl;
	return dataNode;
}


unary_minus_node::unary_minus_node(expression_node *exp_node) : expNode(exp_node) {
}
void unary_minus_node::print(){
	cout << "<# - "; expNode->print(); cout << " #>";
	return;
}
dtype unary_minus_node::evaluate(){
	dtype expData 	=	expNode->evaluate();
	dataNode = expData.negate();
	cout << "\t<$ unary_minus_node: - "; expData.print(); cout << " = "; dataNode.print(); cout << " $>" << endl;
	return dataNode;
}


value_node::value_node(int value){
	dataNode = dtype(value);
}
value_node::value_node(float value){
	dataNode = dtype(value);
}
void value_node::print(){
	dataNode.print();
	return;
}
dtype value_node::evaluate(){
	cout << "\t<$ value: "; dataNode.print(); cout << " $>" << endl;
	return dataNode;
}


variable_node::variable_node(string id){
	variableID = id;
}
void variable_node::print(){
	cout << variableID;
	return;
}
dtype variable_node::evaluate(){
	cout << "\t<$ variable: " << variableID << " = "; id_table[variableID].print(); cout << " $>" << endl;
	return id_table[variableID];
}


assignment_statement_node::assignment_statement_node(string id, expression_node *expr_node) : variableID(id), expressionNode(expr_node) {
}
void assignment_statement_node::print(){
	cout << "<# " << variableID << " = "; expressionNode->print(); cout << " #>" << endl;
}
void assignment_statement_node::evaluate(){
	id_table[variableID] = expressionNode->evaluate();
	cout << "\t<$ assignment_statement: " << variableID << " = "; id_table[variableID].print(); cout << " $>" << endl;
	return;
}

program_node::program_node(list<statement_node *> *stmt_list): statementList(stmt_list) {
}
void program_node::evaluate(){
	list<statement_node *>::iterator statementListIterator;
	// cout << "===================================================PRINT========================================================================" << endl;
	// for(statementListIterator = statementList->begin(); statementListIterator != statementList->end(); statementListIterator++){
	// 	(*statementListIterator)->print();
	// 	// (*statementListIterator)->evaluate();
	// }
	// cout << "================================================EVALUATE========================================================================" << endl;
	// for(statementListIterator = statementList->begin(); statementListIterator != statementList->end(); statementListIterator++){
	// 	// (*statementListIterator)->print();
	// 	(*statementListIterator)->evaluate();
	// }
	cout << "================================================PRINT+EVALUATE==================================================================" << endl;
	for(statementListIterator = statementList->begin(); statementListIterator != statementList->end(); statementListIterator++){
		(*statementListIterator)->print();
		(*statementListIterator)->evaluate();
	}
	cout << "================================================================================================================================" << endl;
}
