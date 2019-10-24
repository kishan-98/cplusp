#include "cplusp.h"

map<string, dtype> value_table;
map<string, bool> id_table;

operator_node::operator_node(expression_node *left, string op, expression_node *right, bool print, string term, string init){
	initiatorChar	=	init;
	terminatorChar	=	term;
	printStatement 	= 	print;
	leftNode 		=	left;
	operatorNode	=	op;
	rightNode 		=	right;
}
void operator_node::print(){
	cout << initiatorChar;
	cout << "<# "; leftNode->print(); cout << " " << operatorNode << " "; rightNode->print(); cout << " #>";
	cout << terminatorChar;
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
void operator_node::print_evaluate(){
	cout << initiatorChar;
	cout << "<# "; leftNode->print(); cout << " " << operatorNode << " "; rightNode->print(); cout << " #>";
	cout << terminatorChar;
	dtype	leftDataNode 	=	leftNode->evaluate();
	dtype	rightDataNode 	=	rightNode->evaluate();
	dataNode = leftDataNode.operate(operatorNode, rightDataNode);
	if(printStatement){
		cout << "\t<$ operator_node: "; leftDataNode.print(); cout << " " << operatorNode << " "; rightDataNode.print(); cout << " = "; dataNode.print(); cout << " $>" << endl;
	}
	return;
}


unary_minus_node::unary_minus_node(expression_node *exp_node, bool print, string term, string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expNode(exp_node) {
}
void unary_minus_node::print(){
	cout << initiatorChar;
	cout << "<# - "; expNode->print(); cout << " #>";
	cout << terminatorChar;
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
void unary_minus_node::print_evaluate(){
	cout << initiatorChar;
	cout << "<# - "; expNode->print(); cout << " #>";
	cout << terminatorChar;
	dtype expData 	=	expNode->evaluate();
	dataNode = expData.negate();
	if(printStatement){
		cout << "\t<$ unary_minus_node: - "; expData.print(); cout << " = "; dataNode.print(); cout << " $>" << endl;
	}
	return;
}


value_node::value_node(bool value, bool print, string term, string init){
	initiatorChar	=	init;
	terminatorChar	=	term;
	printStatement 	= 	print;
	dataNode 		=	dtype(value);
}
value_node::value_node(char value, bool print, string term, string init){
	initiatorChar	=	init;
	terminatorChar	=	term;
	printStatement 	= 	print;
	dataNode 		=	dtype(value);
}
value_node::value_node(int value, bool print, string term, string init){
	initiatorChar	=	init;
	terminatorChar	=	term;
	printStatement 	= 	print;
	dataNode 		=	dtype(value);
}
value_node::value_node(float value, bool print, string term, string init){
	initiatorChar	=	init;
	terminatorChar	=	term;
	printStatement 	= 	print;
	dataNode 		=	dtype(value);
}
void value_node::print(){
	cout << initiatorChar;
	dataNode.print();
	cout << terminatorChar;
	return;
}
dtype value_node::evaluate(){
	if(printStatement){
		cout << "\t<$ value: "; dataNode.print(); cout << " $>" << endl;
	}
	return dataNode;
}
void value_node::print_evaluate(){
	cout << initiatorChar;
	dataNode.print();
	cout << terminatorChar;
	if(printStatement){
		cout << "\t<$ value: "; dataNode.print(); cout << " $>" << endl;
	}
	return;
}


variable_node::variable_node(string id, bool print, string term, string init){
	initiatorChar	=	init;
	terminatorChar	=	term;
	printStatement = print;
	variableID = id;
}
void variable_node::print(){
	cout << initiatorChar;
	cout << variableID;
	cout << terminatorChar;
	return;
}
dtype variable_node::evaluate(){
	if(printStatement){
		cout << "\t<$ variable: " << variableID << " = "; value_table[variableID].print(); cout << " $>" << endl;
	}
	return value_table[variableID];
}
void variable_node::print_evaluate(){
	cout << initiatorChar;
	cout << variableID;
	cout << terminatorChar;
	if(printStatement){
		cout << "\t<$ variable: " << variableID << " = "; value_table[variableID].print(); cout << " $>" << endl;
	}
	return;
}


statement_list_node::statement_list_node(list<statement_node *> *statement_list, bool print, string term, string init) : initiatorChar(init), terminatorChar(term), printStatement(print), statementList(statement_list) {
}
void statement_list_node::insert_statement(statement_node *stmt_node){
	statementList->push_front(stmt_node);
	return;
}
void statement_list_node::print(){
	list<statement_node *>::iterator stmtIt;
	for(stmtIt = statementList->begin(); stmtIt != statementList->end(); stmtIt++){
		cout << initiatorChar;
		(*stmtIt)->print();
	}
	cout << terminatorChar;
	return;
}
void statement_list_node::evaluate(){
	list<statement_node *>::iterator stmtIt;
	for(stmtIt = statementList->begin(); stmtIt != statementList->end(); stmtIt++){
		(*stmtIt)->evaluate();
		if(printStatement){
			cout << "\t<$ statement_list_node: "; (*stmtIt)->print(); cout << " $>" << endl;
		}
	}
	return;
}
void statement_list_node::print_evaluate(){
	list<statement_node *>::iterator stmtIt;
	for(stmtIt = statementList->begin(); stmtIt != statementList->end(); stmtIt++){
		cout << initiatorChar;
		(*stmtIt)->print();
		(*stmtIt)->evaluate();
		if(printStatement){
			cout << "\t<$ statement_list_node: "; (*stmtIt)->print(); cout << " $>" << endl;
		}
	}
	cout << terminatorChar;
	return;
}


expression_statement_node::expression_statement_node(expression_node *expr_node, bool print, string term, string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expressionNode(expr_node) {
}
void expression_statement_node::print(){
	cout << initiatorChar;
	cout << "<# "; expressionNode->print(); cout << " #>";
	cout << terminatorChar;
	return;
}
void expression_statement_node::evaluate(){
	expressionNode->evaluate();
	if(printStatement){
		cout << "\t<$ expression_statement_node: "; expressionNode->print(); cout << " $>" << endl;
	}
	return;
}
void expression_statement_node::print_evaluate(){
	cout << initiatorChar;
	cout << "<# "; expressionNode->print(); cout << " #>";
	cout << terminatorChar;
	expressionNode->evaluate();
	if(printStatement){
		cout << "\t<$ expression_statement_node: "; expressionNode->print(); cout << " $>" << endl;
	}
	return;
}


expression_list_node::expression_list_node(list<expression_node *> *expression_list, bool print, string term, string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expressionList(expression_list) {
}
void expression_list_node::insert_expression(expression_node *stmt_node){
	expressionList->push_front(stmt_node);
	return;
}
void expression_list_node::print(){
	list<expression_node *>::iterator stmtIt;
	for(stmtIt = expressionList->begin(); stmtIt != expressionList->end(); stmtIt++){
		cout << initiatorChar;
		cout << "<# "; (*stmtIt)->print(); cout << " #>";
	}
	cout << terminatorChar;
	return;
}
void expression_list_node::evaluate(){
	list<expression_node *>::iterator stmtIt;
	for(stmtIt = expressionList->begin(); stmtIt != expressionList->end(); stmtIt++){
		(*stmtIt)->evaluate();
		if(printStatement){
			cout << "\t<$ expression_list_node: "; (*stmtIt)->print(); cout << " $>" << endl;
		}
	}
	return;
}
void expression_list_node::print_evaluate(){
	list<expression_node *>::iterator exprIt;
	for(exprIt = expressionList->begin(); exprIt != expressionList->end(); exprIt++){
		cout << initiatorChar;
		cout << "<# "; (*exprIt)->print(); cout << " #>";
		(*exprIt)->evaluate();
		if(printStatement){
			cout << "\t<$ expression_list_node: "; (*exprIt)->print(); cout << " $>" << endl;
		}
	}
	cout << terminatorChar;
	return;
}


tertiary_statement_node::tertiary_statement_node(expression_node *expr_node, statement_node *true_statement, statement_node *false_statement, bool print, string term, string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expressionNode(expr_node), trueExpressionStatement(true_statement), falseExpressionStatement(false_statement) {
}
void tertiary_statement_node::print(){
	cout << initiatorChar;
	cout << "<# "; expressionNode->print(); cout << " ? "; trueExpressionStatement->print(); cout << " : "; falseExpressionStatement->print(); cout << " #>";
	cout << terminatorChar;
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
void tertiary_statement_node::print_evaluate(){
	cout << initiatorChar;
	cout << "<# "; expressionNode->print(); cout << " ? "; trueExpressionStatement->print(); cout << " : "; falseExpressionStatement->print(); cout << " #>";
	cout << terminatorChar;
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


if_statement_node::if_statement_node(list<string> *cond_type, list<expression_node *> *expression_list, list<statement_list_node *> *statement_list, bool print, string term, string init) : initiatorChar(init), terminatorChar(term), printStatement(print), condType(cond_type), expressionList(expression_list), statementList(statement_list) {
}
void if_statement_node::insert_condition(string cond_type, expression_node *expr_node, statement_list_node *stmt_node){
	condType->push_front(cond_type);
	expressionList->push_front(expr_node);
	statementList->push_front(stmt_node);
	return;
}
void if_statement_node::print(){
	list<string>::iterator strIt;
	list<expression_node *>::iterator expIt;
	list<statement_list_node *>::iterator stmtIt;
	for(strIt = condType->begin(), expIt = expressionList->begin(), stmtIt = statementList->begin(); strIt != condType->end() && expIt != expressionList->end() && stmtIt != statementList->end(); strIt++, expIt++, stmtIt++){
		cout << initiatorChar;
		cout << "<# " << (*strIt) << " "; (*expIt)->print(); cout << " THEN " << endl;
		(*stmtIt)->setInitiatorChar("\t\t");
		(*stmtIt)->print();
		(*stmtIt)->setInitiatorChar("");
		cout << "   " << (*strIt) << " #>" << endl;
	}
	cout << terminatorChar;
	return;
}
void if_statement_node::evaluate(){
	dtype dataNode;
	list<expression_node *>::iterator expIt;
	list<statement_list_node *>::iterator stmtIt;
	for(expIt = expressionList->begin(), stmtIt = statementList->begin(); expIt != expressionList->end() && stmtIt != statementList->end(); expIt++, stmtIt++){
		dataNode = (*expIt)->evaluate();
		if(dataNode.evaluate()){
			(*stmtIt)->evaluate();
			if(printStatement){
				cout << "\t<$ if_statement_node: "; (*stmtIt)->print(); cout << " $>" << endl;
			}
			break;
		}
	}
	return;
}
void if_statement_node::print_evaluate(){
	list<string>::iterator strIt;
	list<expression_node *>::iterator expIt;
	list<statement_list_node *>::iterator stmtIt;
	dtype dataNode;
	bool flag = true;
	for(strIt = condType->begin(), expIt = expressionList->begin(), stmtIt = statementList->begin(); strIt != condType->end() && expIt != expressionList->end() && stmtIt != statementList->end(); strIt++, expIt++, stmtIt++){
		cout << initiatorChar;
		cout << "<# " << (*strIt) << " "; (*expIt)->print(); cout << " THEN " << endl;
		(*stmtIt)->setInitiatorChar("\t\t");
		(*stmtIt)->print();
		(*stmtIt)->setInitiatorChar("");
		cout << "   " << (*strIt) << " #>" << endl;
		if(flag){
			dataNode = (*expIt)->evaluate();
			if(dataNode.evaluate()){
				(*stmtIt)->evaluate();
				if(printStatement){
					cout << "\t<$ if_statement_node: "; (*stmtIt)->print(); cout << " $>" << endl;
				}
				flag = false;
			}
		}
	}
	cout << terminatorChar;
	return;
}


while_statement_node::while_statement_node(expression_node *expr_node, statement_list_node *statement_list, bool print, string term, string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expressionNode(expr_node), statementList(statement_list){
}
void while_statement_node::print(){
	cout << initiatorChar;
	cout << "<# WHILE "; expressionNode->print(); cout << " DO" << endl;
	statementList->setInitiatorChar("\t\t");
	statementList->print();
	statementList->setInitiatorChar("");
	cout << "   WHILE #>" << endl;
	cout << terminatorChar;
	return;
}
void while_statement_node::evaluate(){
	dtype dataNode = expressionNode->evaluate();
	while(dataNode.evaluate()){
		statementList->evaluate();
		if(printStatement){
			cout << "\t<$ while_statement_node: "; statementList->print(); cout << " $>" << endl;
		}
	}
	return;
}
void while_statement_node::print_evaluate(){
	cout << initiatorChar;
	cout << "<# WHILE "; expressionNode->print(); cout << " DO" << endl;
	statementList->setInitiatorChar("\t\t");
	statementList->print();
	statementList->setInitiatorChar("");
	cout << "   WHILE #>" << endl;
	cout << terminatorChar;
	dtype dataNode = expressionNode->evaluate();
	while(dataNode.evaluate()){
		statementList->evaluate();
		if(printStatement){
			cout << "\t<$ while_statement_node: "; statementList->print(); cout << " $>" << endl;
		}
	}
	return;
}


declaration_statement_node::declaration_statement_node(string data_type, string id, expression_node *expr_node, bool print, string term, string init) : initiatorChar(init), terminatorChar(term), printStatement(print), variableID(id), dataType(data_type), expressionNode(expr_node) {
}
void declaration_statement_node::print(){
	cout << initiatorChar;
	cout << "<# " << variableID << " = "; expressionNode->print(); cout << " #>";
	cout << terminatorChar;
	return;
}
void declaration_statement_node::evaluate(){
	// value_table[variableID] = expressionNode->evaluate();
	if(printStatement){
		cout << "\t<$ declaration_statement_node: " << variableID << " = "; value_table[variableID].print(); cout << " $>" << endl;
	}
	return;
}
void declaration_statement_node::print_evaluate(){
	cout << initiatorChar;
	cout << "<# " << variableID << " = "; expressionNode->print(); cout << " #>";
	cout << terminatorChar;
	// value_table[variableID] = expressionNode->evaluate();
	if(printStatement){
		cout << "\t<$ declaration_statement_node: " << variableID << " = "; value_table[variableID].print(); cout << " $>" << endl;
	}
	return;
}


assignment_statement_node::assignment_statement_node(string id, expression_node *expr_node, bool print, string term, string init) : initiatorChar(init), terminatorChar(term), printStatement(print), variableID(id), expressionNode(expr_node) {
}
void assignment_statement_node::print(){
	cout << initiatorChar;
	cout << "<# " << variableID << " = "; expressionNode->print(); cout << " #>";
	cout << terminatorChar;
	return;
}
void assignment_statement_node::evaluate(){
	value_table[variableID] = expressionNode->evaluate();
	if(printStatement){
		cout << "\t<$ assignment_statement_node: " << variableID << " = "; value_table[variableID].print(); cout << " $>" << endl;
	}
	return;
}
void assignment_statement_node::print_evaluate(){
	cout << initiatorChar;
	cout << "<# " << variableID << " = "; expressionNode->print(); cout << " #>";
	cout << terminatorChar;
	value_table[variableID] = expressionNode->evaluate();
	if(printStatement){
		cout << "\t<$ assignment_statement_node: " << variableID << " = "; value_table[variableID].print(); cout << " $>" << endl;
	}
	return;
}

program_node::program_node(statement_list_node *stmt_list, bool print, string term, string init): initiatorChar(init), terminatorChar(term), printStatement(print), statementList(stmt_list) {
}
void program_node::evaluate(){
	if(printStatement){
		cout << "===================================================PRINT========================================================================" << endl;
		statementList->print();
		cout << "================================================EVALUATE========================================================================" << endl;
		statementList->evaluate();
		cout << "================================================PRINT+EVALUATE==================================================================" << endl;
		statementList->print_evaluate();
		cout << "================================================================================================================================" << endl;
	}
	return;
}
