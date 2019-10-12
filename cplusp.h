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
public:
	expression_node *leftNode;
	expression_node *rightNode;
	operator_node(expression_node *left, expression_node *right);
};

class plus_node : public operator_node {
public:
	plus_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class minus_node : public operator_node {
public:
	minus_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class multiply_node : public operator_node {
public:
	multiply_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class divide_node : public operator_node {
public:
	divide_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class modulo_node : public operator_node {
public:
	modulo_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class logand_node : public operator_node {
public:
	logand_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class logor_node : public operator_node {
public:
	logor_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class or_node : public operator_node {
public:
	or_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class xor_node : public operator_node {
public:
	xor_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class and_node : public operator_node {
public:
	and_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class eq_node : public operator_node {
public:
	eq_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class ne_node : public operator_node {
public:
	ne_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class gt_node : public operator_node {
public:
	gt_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class ge_node : public operator_node {
public:
	ge_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class lt_node : public operator_node {
public:
	lt_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class le_node : public operator_node {
public:
	le_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class lshift_node : public operator_node {
public:
	lshift_node(expression_node *left, expression_node *right);
	void print();
	int evaluate();
};

class rshift_node : public operator_node {
public:
	rshift_node(expression_node *left, expression_node *right);
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
