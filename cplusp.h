#include <bits/stdc++.h>

using namespace std;

class expression_node{
public:
	int valueInteger;
	virtual void print() = 0;
	virtual int evaluate() = 0;
};

class unary_minus_node : public expression_node {
protected:
	expression_node *expNode;
public:
	unary_minus_node(expression_node *exp_node);
	void print();
	int evaluate();
};

class operator_node : public expression_node {
protected:
	expression_node *leftNode;
	expression_node *rightNode;
	string operatorNode;
public:
	operator_node(expression_node *left, string op, expression_node *right);
	void print();
	int evaluate();
};

class value_node : public expression_node {
public:
	value_node(int value);
	void print();
	int evaluate();
};

class variable_node : public expression_node {
protected:
	string variableID;
public:
	variable_node(string id);
	void print();
	int evaluate();
};

class statement_node{
public:
	virtual void print() = 0;
	virtual void evaluate() = 0;
};

class assignment_statement_node : public statement_node {
protected:
	string variableID;
	expression_node *expressionNode;
public:
	assignment_statement_node(string id, expression_node *expr_node);
	void print();
	void evaluate();
};

class program_node{
protected:
	list<statement_node *> *statementList;
public:
	program_node(list<statement_node *> *stmt_list);
	void evaluate();
};

extern program_node *root;
extern map<string, int> id_table;
