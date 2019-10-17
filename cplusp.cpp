#include "cplusp.h"

map<string, dtype> value_table;
map<string, bool> id_table;

operator_node::operator_node(expression_node *left, string op, expression_node *right, bool print, char term){
	terminatorChar	=	term;
	printStatement 	= 	print;
	leftNode 		=	left;
	operatorNode	=	op;
	rightNode 		=	right;
}
void operator_node::print(){
	cout << "<# "; leftNode->print(); cout << operatorNode << " "; rightNode->print(); cout << "#>";
	if(terminatorChar){cout << terminatorChar;}
	return;
}
dtype operator_node::evaluate(){
	dtype	leftDataNode 	=	leftNode->evaluate();
	dtype	rightDataNode 	=	rightNode->evaluate();
	dataNode = leftDataNode.operate(operatorNode, rightDataNode);
	if(printStatement){
		cout << "\t<$ operator_node: "; leftDataNode.print(); cout << " " << operatorNode << " "; rightDataNode.print(); cout << " = "; dataNode.print(); cout << " $>" << endl;
	}
	return dataNode;
}


unary_minus_node::unary_minus_node(expression_node *exp_node, bool print, char term) : terminatorChar(term), printStatement(print), expNode(exp_node) {
}
void unary_minus_node::print(){
	cout << "<# - "; expNode->print(); cout << "#>";
	if(terminatorChar){cout << terminatorChar;}
	return;
}
dtype unary_minus_node::evaluate(){
	dtype expData 	=	expNode->evaluate();
	dataNode = expData.negate();
	if(printStatement){
		cout << "\t<$ unary_minus_node: - "; expData.print(); cout << " = "; dataNode.print(); cout << " $>" << endl;
	}
	return dataNode;
}


value_node::value_node(bool value, bool print, char term){
	terminatorChar	=	term;
	printStatement 	= 	print;
	dataNode 		=	dtype(value);
}
value_node::value_node(char value, bool print, char term){
	terminatorChar	=	term;
	printStatement 	= 	print;
	dataNode 		=	dtype(value);
}
value_node::value_node(int value, bool print, char term){
	terminatorChar	=	term;
	printStatement 	= 	print;
	dataNode 		=	dtype(value);
}
value_node::value_node(float value, bool print, char term){
	terminatorChar	=	term;
	printStatement 	= 	print;
	dataNode 		=	dtype(value);
}
void value_node::print(){
	dataNode.print();
	if(terminatorChar){cout << terminatorChar;}
	return;
}
dtype value_node::evaluate(){
	if(printStatement){
		cout << "\t<$ value: "; dataNode.print(); cout << " $>" << endl;
	}
	return dataNode;
}


variable_node::variable_node(string id, bool print, char term){
	terminatorChar	=	term;
	printStatement = print;
	variableID = id;
}
void variable_node::print(){
	cout << variableID;
	if(terminatorChar){cout << terminatorChar;}
	return;
}
dtype variable_node::evaluate(){
	if(printStatement){
		cout << "\t<$ variable: " << variableID << " = "; value_table[variableID].print(); cout << " $>" << endl;
	}
	return value_table[variableID];
}


expression_statement_node::expression_statement_node(expression_node *expr_node, bool print, char term) : terminatorChar(term), printStatement(print), expressionNode(expr_node) {
}
void expression_statement_node::print(){
	cout << "<# "; expressionNode->print(); cout << "#>";
	if(terminatorChar){cout << terminatorChar;}
	return;
}
void expression_statement_node::evaluate(){
	if(printStatement){
		cout << "\t<$ expression_statement_node: "; expressionNode->print(); cout << " $>" << endl;
	}
	return;
}


tertiary_statement_node::tertiary_statement_node(expression_node *expr_node, statement_node *true_statement, statement_node *false_statement, bool print, char term) : terminatorChar(term), printStatement(print), expressionNode(expr_node), trueExpressionStatement(true_statement), falseExpressionStatement(false_statement) {
}
void tertiary_statement_node::print(){
	cout << "<# "; expressionNode->print(); cout << "? "; trueExpressionStatement->print(); cout << ": "; falseExpressionStatement->print(); cout << "#>";
	if(terminatorChar){cout << terminatorChar;}
	return;
}
void tertiary_statement_node::evaluate(){
	dataNode = expressionNode->evaluate();
	if(dataNode.evaluate()){
		trueExpressionStatement->evaluate();
	}
	else{
		falseExpressionStatement->evaluate();
	}
	if(printStatement){
		cout << "\t<$ tertiary_statement_node: "; (dataNode.evaluate() ? trueExpressionStatement->print() : falseExpressionStatement->print()); cout << " $>" << endl;
	}
	return;
}


declaration_statement_node::declaration_statement_node(string data_type, string id, expression_node *expr_node, bool print, char term) : terminatorChar(term), printStatement(print), variableID(id), dataType(data_type), expressionNode(expr_node) {
}
void declaration_statement_node::print(){
	cout << "<# " << variableID << " = "; expressionNode->print(); cout << "#>";
	if(terminatorChar){cout << terminatorChar;}
	return;
}
void declaration_statement_node::evaluate(){
	// value_table[variableID] = expressionNode->evaluate();
	if(printStatement){
		cout << "\t<$ declaration_statement_node: " << variableID << " = "; value_table[variableID].print(); cout << " $>" << endl;
	}
	return;
}


assignment_statement_node::assignment_statement_node(string id, expression_node *expr_node, bool print, char term) : terminatorChar(term), printStatement(print), variableID(id), expressionNode(expr_node) {
}
void assignment_statement_node::print(){
	cout << "<# " << variableID << " = "; expressionNode->print(); cout << "#>";
	if(terminatorChar){cout << terminatorChar;}
	return;
}
void assignment_statement_node::evaluate(){
	value_table[variableID] = expressionNode->evaluate();
	if(printStatement){
		cout << "\t<$ assignment_statement_node: " << variableID << " = "; value_table[variableID].print(); cout << " $>" << endl;
	}
	return;
}

program_node::program_node(list<statement_node *> *stmt_list, bool print, char term): terminatorChar(term), printStatement(print), statementList(stmt_list) {
}
void program_node::evaluate(){
	if(printStatement){
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
	return;
}
