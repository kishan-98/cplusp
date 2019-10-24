#include <bits/stdc++.h>

using namespace std;

// Store data type group, heirarchy and supported operators
extern map<string, pair<int, int> > data_groups;

typedef struct MYDATA dtype;

struct MYDATA {
	string dataType;
	bool validType;
	union{
		bool	valueBool;
		char	valueChar;
		int 	valueInteger;
		float	valueFloat;
	}dataValue;
	MYDATA(){
		validType 	= 	false;
		dataType	=	"";
	}
	MYDATA(bool value){
		validType				=	true;
		dataType				=	"TYPE_BOOL";
		dataValue.valueBool		=	value;
	}
	MYDATA(char value){
		validType				=	true;
		dataType				=	"TYPE_CHAR";
		dataValue.valueChar		=	value;
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
		if(dataType == "TYPE_BOOL"){
			if(toDataType == "TYPE_BOOL"){
				dataType = "TYPE_BOOL", dataValue.valueBool = (bool)dataValue.valueBool;
			}
			else if(toDataType == "TYPE_CHAR"){
				dataType = "TYPE_CHAR", dataValue.valueChar = (char)dataValue.valueBool;
			}
			else if(toDataType == "TYPE_INT"){
				dataType = "TYPE_INT", dataValue.valueInteger = (int)dataValue.valueBool;
			}
			else if(toDataType == "TYPE_FLOAT"){
				dataType = "TYPE_FLOAT", dataValue.valueFloat = (float)dataValue.valueBool;
			}
		}
		else if(dataType == "TYPE_CHAR"){
			if(toDataType == "TYPE_BOOL"){
				dataType = "TYPE_BOOL", dataValue.valueBool = (bool)dataValue.valueChar;
			}
			else if(toDataType == "TYPE_CHAR"){
				dataType = "TYPE_CHAR", dataValue.valueChar = (char)dataValue.valueChar;
			}
			else if(toDataType == "TYPE_INT"){
				dataType = "TYPE_INT", dataValue.valueInteger = (int)dataValue.valueChar;
			}
			else if(toDataType == "TYPE_FLOAT"){
				dataType = "TYPE_FLOAT", dataValue.valueFloat = (float)dataValue.valueChar;
			}
		}
		else if(dataType == "TYPE_INT"){
			if(toDataType == "TYPE_BOOL"){
				dataType = "TYPE_BOOL", dataValue.valueBool = (bool)dataValue.valueInteger;
			}
			else if(toDataType == "TYPE_CHAR"){
				dataType = "TYPE_CHAR", dataValue.valueChar = (char)dataValue.valueInteger;
			}
			else if(toDataType == "TYPE_INT"){
				dataType = "TYPE_INT", dataValue.valueInteger = (int)dataValue.valueInteger;
			}
			else if(toDataType == "TYPE_FLOAT"){
				dataType = "TYPE_FLOAT", dataValue.valueFloat = (float)dataValue.valueInteger;
			}
		}
		else if(dataType == "TYPE_FLOAT"){
			if(toDataType == "TYPE_BOOL"){
				dataType = "TYPE_BOOL", dataValue.valueBool = (bool)dataValue.valueFloat;
			}
			else if(toDataType == "TYPE_CHAR"){
				dataType = "TYPE_CHAR", dataValue.valueChar = (char)dataValue.valueFloat;
			}
			else if(toDataType == "TYPE_INT"){
				dataType = "TYPE_INT", dataValue.valueInteger = (int)dataValue.valueFloat;
			}
			else if(toDataType == "TYPE_FLOAT"){
				dataType = "TYPE_FLOAT", dataValue.valueFloat = (float)dataValue.valueFloat;
			}
		}
	    return;
	}
	void print(){
		if(dataType == "TYPE_BOOL")cout << dataValue.valueBool;
	    else if(dataType == "TYPE_CHAR")cout << dataValue.valueChar;
		else if(dataType == "TYPE_INT")cout << dataValue.valueInteger;
	    else if(dataType == "TYPE_FLOAT")cout << dataValue.valueFloat;
		cout << "<" << dataType << ">";
	    return;
	}
	bool evaluate(){
		if(!validType) return false;
		if(dataType == "TYPE_BOOL") return (dataValue.valueBool ? true : false);
	    else if(dataType == "TYPE_CHAR") return (dataValue.valueChar ? true : false);
		else if(dataType == "TYPE_INT") return (dataValue.valueInteger ? true : false);
	    else if(dataType == "TYPE_FLOAT") return (dataValue.valueFloat ? true : false);
	    return false;
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
			if(dataType == "TYPE_BOOL"){
					 if(op == "+") 	return MYDATA(dataValue.valueBool + s2.dataValue.valueBool);
				else if(op == "-") 	return MYDATA(dataValue.valueBool - s2.dataValue.valueBool);
				else if(op == "*") 	return MYDATA(dataValue.valueBool * s2.dataValue.valueBool);
				else if(op == "/") 	return MYDATA(dataValue.valueBool / s2.dataValue.valueBool);
				else if(op == "%") 	return MYDATA(dataValue.valueBool % s2.dataValue.valueBool);
				else if(op == "||")	return MYDATA(dataValue.valueBool || s2.dataValue.valueBool);
				else if(op == "&&") return MYDATA(dataValue.valueBool && s2.dataValue.valueBool);
				else if(op == "==") return MYDATA(dataValue.valueBool == s2.dataValue.valueBool);
				else if(op == "!=") return MYDATA(dataValue.valueBool != s2.dataValue.valueBool);
				else if(op == ">") 	return MYDATA(dataValue.valueBool > s2.dataValue.valueBool);
				else if(op == ">=") return MYDATA(dataValue.valueBool >= s2.dataValue.valueBool);
				else if(op == "<") 	return MYDATA(dataValue.valueBool < s2.dataValue.valueBool);
				else if(op == "<=") return MYDATA(dataValue.valueBool <= s2.dataValue.valueBool);
				else if(op == "|") 	return MYDATA(dataValue.valueBool | s2.dataValue.valueBool);
				else if(op == "^") 	return MYDATA(dataValue.valueBool ^ s2.dataValue.valueBool);
				else if(op == "&") 	return MYDATA(dataValue.valueBool & s2.dataValue.valueBool);
				else if(op == "<<") return MYDATA(dataValue.valueBool << s2.dataValue.valueBool);
				else if(op == ">>") return MYDATA(dataValue.valueBool >> s2.dataValue.valueBool);
				else return MYDATA();
			}
			else if(dataType == "TYPE_CHAR"){
					 if(op == "+") 	return MYDATA(dataValue.valueChar + s2.dataValue.valueChar);
				else if(op == "-") 	return MYDATA(dataValue.valueChar - s2.dataValue.valueChar);
				else if(op == "*") 	return MYDATA(dataValue.valueChar * s2.dataValue.valueChar);
				else if(op == "/") 	return MYDATA(dataValue.valueChar / s2.dataValue.valueChar);
				else if(op == "%") 	return MYDATA(dataValue.valueChar % s2.dataValue.valueChar);
				else if(op == "||")	return MYDATA(dataValue.valueChar || s2.dataValue.valueChar);
				else if(op == "&&") return MYDATA(dataValue.valueChar && s2.dataValue.valueChar);
				else if(op == "==") return MYDATA(dataValue.valueChar == s2.dataValue.valueChar);
				else if(op == "!=") return MYDATA(dataValue.valueChar != s2.dataValue.valueChar);
				else if(op == ">") 	return MYDATA(dataValue.valueChar > s2.dataValue.valueChar);
				else if(op == ">=") return MYDATA(dataValue.valueChar >= s2.dataValue.valueChar);
				else if(op == "<") 	return MYDATA(dataValue.valueChar < s2.dataValue.valueChar);
				else if(op == "<=") return MYDATA(dataValue.valueChar <= s2.dataValue.valueChar);
				else if(op == "|") 	return MYDATA(dataValue.valueChar | s2.dataValue.valueChar);
				else if(op == "^") 	return MYDATA(dataValue.valueChar ^ s2.dataValue.valueChar);
				else if(op == "&") 	return MYDATA(dataValue.valueChar & s2.dataValue.valueChar);
				else if(op == "<<") return MYDATA(dataValue.valueChar << s2.dataValue.valueChar);
				else if(op == ">>") return MYDATA(dataValue.valueChar >> s2.dataValue.valueChar);
				else return MYDATA();
			}
			else if(dataType == "TYPE_INT"){
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
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
public:
	dtype dataNode;
	virtual void setInitiatorChar(string init){initiatorChar = init; return;}
	virtual void addInitiatorChar(string term){initiatorChar += term; return;}
	virtual void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	virtual void setTerminatorChar(string term){terminatorChar = term; return;}
	virtual void setPrintStatement(bool print){printStatement = print; return;}
	virtual void print(){cout << " "; return;}
	virtual dtype evaluate(){return dtype();}
	virtual void print_evaluate(){cout << " "; return;};
};

class unary_minus_node : public expression_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	expression_node *expNode;
public:
	unary_minus_node(expression_node *exp_node, bool print = false, string term = "", string init = "");
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	dtype evaluate();
	void print_evaluate();
};

class operator_node : public expression_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	expression_node *leftNode;
	expression_node *rightNode;
	string operatorNode;
public:
	operator_node(expression_node *left, string op, expression_node *right, bool print = false, string term = "", string init = "");
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	dtype evaluate();
	void print_evaluate();
};

class value_node : public expression_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
public:
	value_node(bool value, bool print = false, string term = "", string init = "");
	value_node(char value, bool print = false, string term = "", string init = "");
	value_node(int value, bool print = false, string term = "", string init = "");
	value_node(float value, bool print = false, string term = "", string init = "");
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	dtype evaluate();
	void print_evaluate();
};

class variable_node : public expression_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	string variableID;
public:
	variable_node(string id, bool print = false, string term = "", string init = "");
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	dtype evaluate();
	void print_evaluate();
};

class statement_node{
protected:
	string initiatorChar;
	string terminatorChar;
public:
	virtual void setInitiatorChar(string) = 0;
	virtual void addInitiatorChar(string) = 0;
	virtual void remInitiatorChar(string) = 0;
	virtual void setTerminatorChar(string) = 0;
	virtual void setPrintStatement(bool) = 0;
	virtual void print() = 0;
	virtual void evaluate() = 0;
	virtual void print_evaluate() = 0;
};

class statement_list_node : public statement_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	list<statement_node *> *statementList;
public:
	statement_list_node(list<statement_node *> *statement_list, bool print = false, string term = "", string init = "");
	void insert_statement(statement_node *stmt_node);
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
};

class expression_list_node : public statement_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	list<expression_node *> *expressionList;
public:
	expression_list_node(list<expression_node *> *expression_list, bool print = false, string term = "", string init = "");
	void insert_expression(expression_node *stmt_node);
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
};

class expression_statement_node : public statement_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	expression_node *expressionNode;
public:
	expression_statement_node(expression_node *expr_node, bool print = false, string term = "", string init = "");
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
};

class control_statement_node : public statement_node {
protected:
	string initiatorChar;
	string terminatorChar;
public:
	virtual void setInitiatorChar(string) = 0;
	virtual void addInitiatorChar(string) = 0;
	virtual void remInitiatorChar(string) = 0;
	virtual void setTerminatorChar(string) = 0;
	virtual void setPrintStatement(bool) = 0;
	virtual void print() = 0;
	virtual void evaluate() = 0;
	virtual void print_evaluate() = 0;
};

class tertiary_statement_node : public control_statement_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	dtype dataNode;
	expression_node *expressionNode;
	statement_node *trueExpressionStatement;
	statement_node *falseExpressionStatement;
public:
	tertiary_statement_node(expression_node *expr_node, statement_node *true_statement, statement_node *false_statement, bool print = false, string term = "", string init = "");
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
};

class if_statement_node : public control_statement_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	list<string> *condType;
	list<expression_node *> *expressionList;
	list<statement_list_node *> *statementList;
public:
	if_statement_node(list<string> *cond_type, list<expression_node *> *expression_list, list<statement_list_node *> *statement_list, bool print = false, string term = "", string init = "");
	void insert_condition(string cond_type, expression_node *expr_node, statement_list_node *stmt_node);
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
};

class loop_statement_node : public statement_node {
protected:
	string initiatorChar;
	string terminatorChar;
public:
	virtual void setInitiatorChar(string) = 0;
	virtual void addInitiatorChar(string) = 0;
	virtual void remInitiatorChar(string) = 0;
	virtual void setTerminatorChar(string) = 0;
	virtual void setPrintStatement(bool) = 0;
	virtual void print() = 0;
	virtual void evaluate() = 0;
	virtual void print_evaluate() = 0;
};

class while_statement_node : public loop_statement_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	expression_node *expressionNode;
	statement_list_node *statementList;
public:
	while_statement_node(expression_node *expr_node, statement_list_node *statement_list, bool print = false, string term = "", string init = "");
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
};

class for_statement_node : public loop_statement_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	statement_node *initNode;
	expression_node *termNode;
	statement_node *modfNode;
	statement_list_node *statementList;
public:
	for_statement_node(statement_node *init_node, expression_node *term_node, statement_node *modf_node, statement_list_node *statement_list, bool print = false, string term = "", string init = "");
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
};

class declaration_statement_node : public statement_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	string variableID, dataType;
	expression_node *expressionNode;
public:
	declaration_statement_node(string data_type, string id, expression_node *expr_node, bool print = false, string term = "", string init = "");
	// declaration_statement_node(string id, bool print = false, string term = "", string init = "");
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
};

class assignment_statement_node : public statement_node {
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	string variableID;
	expression_node *expressionNode;
public:
	assignment_statement_node(string id, expression_node *expr_node, bool print = false, string term = "", string init = "");
	void setInitiatorChar(string init){initiatorChar = init; return;}
	void addInitiatorChar(string term){initiatorChar += term; return;}
	void remInitiatorChar(string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
};

class program_node{
protected:
	string initiatorChar;
	string terminatorChar;
	bool printStatement;
	statement_list_node *statementList;
public:
	program_node(statement_list_node *stmt_list, bool print = false, string term = "", string init = "");
	void evaluate();
	void print_evaluate();
};

extern program_node *root;
extern map<string, dtype> value_table;
extern map<string, bool> id_table;
