#include <bits/stdc++.h>

using namespace std;

// Store data type group, heirarchy and supported operators
extern map<string, pair<int, int> > data_groups;

typedef struct MYDATA dtype;

struct MYDATA {
	string dataType;
	bool validType;
	union{
		int 	valueInteger;
		float	valueFloat;
	}dataValue;
	MYDATA(){
		validType = false;
	}
	MYDATA(int value){
		validType				=	true;
		dataType				=	"TYPE_INT";
		dataValue.valueInteger	=	value;
	}
	MYDATA(float value){
		validType				=	true;
		dataType				=	"TYPE_FLOAT";
		dataValue.valueFloat	=	value;
	}
	void my_typecast(string toDataType){
		if(dataType == "TYPE_INT"){
			if(toDataType == "TYPE_INT"){
				dataType = "TYPE_INT", dataValue.valueInteger = (int)dataValue.valueInteger;
			}
			else if(toDataType == "TYPE_FLOAT"){
				dataType = "TYPE_FLOAT", dataValue.valueFloat = (float)dataValue.valueInteger;
			}
		}
		else if(dataType == "TYPE_FLOAT"){
			if(toDataType == "TYPE_INT"){
				dataType = "TYPE_INT", dataValue.valueInteger = (int)dataValue.valueFloat;
			}
			else if(toDataType == "TYPE_FLOAT"){
				dataType = "TYPE_FLOAT", dataValue.valueFloat = (float)dataValue.valueFloat;
			}
		}
	    return;
	}
	void print(){
	    if(dataType == "TYPE_INT")cout << dataValue.valueInteger;
	    else if(dataType == "TYPE_FLOAT")cout << dataValue.valueFloat;
		cout << "<" << dataType << ">";
	    return;
	}
	struct MYDATA operate(string op, struct MYDATA s2){
		if(data_groups[dataType].first != data_groups[s2.dataType].first){
		    return MYDATA();
		}
		else{
		    if(data_groups[dataType].second > data_groups[s2.dataType].second){
		        s2.my_typecast(dataType);
		    }
		    else if(data_groups[dataType].second < data_groups[s2.dataType].second){
		        my_typecast(s2.dataType);
		    }
			if(dataType == "TYPE_INT"){
					 if(op == "+") 	return MYDATA(dataValue.valueInteger + s2.dataValue.valueInteger);
				else if(op == "-") 	return MYDATA(dataValue.valueInteger - s2.dataValue.valueInteger);
				else if(op == "*") 	return MYDATA(dataValue.valueInteger * s2.dataValue.valueInteger);
				else if(op == "/") 	return MYDATA(dataValue.valueInteger / s2.dataValue.valueInteger);
				else if(op == "%") 	return MYDATA(dataValue.valueInteger % s2.dataValue.valueInteger);
				else if(op == "||")	return MYDATA(dataValue.valueInteger || s2.dataValue.valueInteger);
				else if(op == "&&") return MYDATA(dataValue.valueInteger && s2.dataValue.valueInteger);
				else if(op == "==") return MYDATA(dataValue.valueInteger == s2.dataValue.valueInteger);
				else if(op == "!=") return MYDATA(dataValue.valueInteger != s2.dataValue.valueInteger);
				else if(op == ">") 	return MYDATA(dataValue.valueInteger > s2.dataValue.valueInteger);
				else if(op == ">=") return MYDATA(dataValue.valueInteger >= s2.dataValue.valueInteger);
				else if(op == "<") 	return MYDATA(dataValue.valueInteger < s2.dataValue.valueInteger);
				else if(op == "<=") return MYDATA(dataValue.valueInteger <= s2.dataValue.valueInteger);
				else if(op == "|") 	return MYDATA(dataValue.valueInteger | s2.dataValue.valueInteger);
				else if(op == "^") 	return MYDATA(dataValue.valueInteger ^ s2.dataValue.valueInteger);
				else if(op == "&") 	return MYDATA(dataValue.valueInteger & s2.dataValue.valueInteger);
				else if(op == "<<") return MYDATA(dataValue.valueInteger << s2.dataValue.valueInteger);
				else if(op == ">>") return MYDATA(dataValue.valueInteger >> s2.dataValue.valueInteger);
				else return MYDATA();
			}
			else if(dataType == "TYPE_FLOAT"){
					 if(op == "+")	return MYDATA(dataValue.valueFloat + s2.dataValue.valueFloat);
			   	else if(op == "-")	return MYDATA(dataValue.valueFloat - s2.dataValue.valueFloat);
			   	else if(op == "*")	return MYDATA(dataValue.valueFloat * s2.dataValue.valueFloat);
			   	else if(op == "/")	return MYDATA(dataValue.valueFloat / s2.dataValue.valueFloat);
			   	else if(op == "%")	return MYDATA(fmod(dataValue.valueFloat, s2.dataValue.valueFloat));
			   	else if(op == "||")	return MYDATA(dataValue.valueFloat || s2.dataValue.valueFloat);
			   	else if(op == "&&")	return MYDATA(dataValue.valueFloat && s2.dataValue.valueFloat);
			   	else if(op == "==")	return MYDATA(dataValue.valueFloat == s2.dataValue.valueFloat);
			   	else if(op == "!=")	return MYDATA(dataValue.valueFloat != s2.dataValue.valueFloat);
			   	else if(op == ">")	return MYDATA(dataValue.valueFloat > s2.dataValue.valueFloat);
			   	else if(op == ">=")	return MYDATA(dataValue.valueFloat >= s2.dataValue.valueFloat);
			   	else if(op == "<")	return MYDATA(dataValue.valueFloat < s2.dataValue.valueFloat);
			   	else if(op == "<=")	return MYDATA(dataValue.valueFloat <= s2.dataValue.valueFloat);
				else return MYDATA();
			}
			return MYDATA();
		}
	}
	struct MYDATA negate(){
		if(dataType == "TYPE_INT"){
			return MYDATA(-dataValue.valueInteger);
		}
		else if(dataType == "TYPE_FLOAT"){
			return MYDATA(-dataValue.valueFloat);
		}
		return MYDATA();
	}
};

class expression_node{
public:
	dtype dataNode;
	virtual void print() = 0;
	virtual dtype evaluate() = 0;
};

class unary_minus_node : public expression_node {
protected:
	expression_node *expNode;
public:
	unary_minus_node(expression_node *exp_node);
	void print();
	dtype evaluate();
};

class operator_node : public expression_node {
protected:
	expression_node *leftNode;
	expression_node *rightNode;
	string operatorNode;
public:
	operator_node(expression_node *left, string op, expression_node *right);
	void print();
	dtype evaluate();
};

class value_node : public expression_node {
public:
	value_node(int value);
	value_node(float value);
	void print();
	dtype evaluate();
};

class variable_node : public expression_node {
protected:
	string variableID;
public:
	variable_node(string id);
	void print();
	dtype evaluate();
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
extern map<string, dtype> id_table;
