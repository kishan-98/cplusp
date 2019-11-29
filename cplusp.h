#include <bits/stdc++.h>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

// using namespace std;

// Store data type group, heirarchy and supported operators
extern std::map<std::string, std::pair<int, int> > data_groups;

// using namespace llvm;

// The function getGlobalContext is depricated, so use declaration without definition
// static LLVMContext &Context = getGlobalContext();
static llvm::LLVMContext Context;
static llvm::Module *ModuleOb = new llvm::Module("cplusp", Context);
static llvm::IRBuilder<> Builder(Context);
static std::vector<std::string> FuncArgs;
// llvm::Value *ret;
typedef llvm::SmallVector<llvm::BasicBlock *, 16> BBList;
typedef llvm::SmallVector<llvm::Value *, 16> ValList;

typedef struct MYDATA dtype;

struct MYDATA {
	std::string dataType;
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
	void my_typecast(std::string toDataType){
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
		if(dataType == "TYPE_BOOL")std::cout << dataValue.valueBool;
	    else if(dataType == "TYPE_CHAR")std::cout << dataValue.valueChar;
		else if(dataType == "TYPE_INT")std::cout << dataValue.valueInteger;
	    else if(dataType == "TYPE_FLOAT")std::cout << dataValue.valueFloat;
		std::cout << "<" << dataType << ">";
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
	llvm::Value* generate(bool loadVariable = false){
			 if(dataType == "TYPE_BOOL") return Builder.getInt32(dataValue.valueBool);
	    else if(dataType == "TYPE_CHAR") return Builder.getInt32(dataValue.valueChar);
		else if(dataType == "TYPE_INT") return Builder.getInt32(dataValue.valueInteger);
	    else if(dataType == "TYPE_FLOAT") return Builder.getInt32(dataValue.valueFloat);
		return Builder.getInt32(0);
	}
	struct MYDATA operate(std::string op, struct MYDATA s2){
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
			   	else if(op == "%")	return MYDATA(std::fmod(dataValue.valueFloat, s2.dataValue.valueFloat));
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
		if(dataType == "TYPE_CHAR"){
			return MYDATA(-dataValue.valueChar);
		}
		else if(dataType == "TYPE_BOOL"){
			return MYDATA(-dataValue.valueBool);
		}
		else if(dataType == "TYPE_INT"){
			return MYDATA(-dataValue.valueInteger);
		}
		else if(dataType == "TYPE_FLOAT"){
			return MYDATA(-dataValue.valueFloat);
		}
		return MYDATA();
	}
	struct MYDATA not_logical(){
		if(dataType == "TYPE_CHAR"){
			return MYDATA(!dataValue.valueChar);
		}
		else if(dataType == "TYPE_BOOL"){
			return MYDATA(!dataValue.valueBool);
		}
		else if(dataType == "TYPE_INT"){
			return MYDATA(!dataValue.valueInteger);
		}
		return MYDATA();
	}
	struct MYDATA not_binary(){
		if(dataType == "TYPE_CHAR"){
			return MYDATA(~dataValue.valueChar);
		}
		// else if(dataType == "TYPE_BOOL"){
		// 	return MYDATA(~dataValue.valueBool);
		// }
		else if(dataType == "TYPE_INT"){
			return MYDATA(~dataValue.valueInteger);
		}
		return MYDATA();
	}
};

class expression_node{
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
public:
	dtype dataNode;
	virtual void setInitiatorChar(std::string init){initiatorChar = init; return;}
	virtual void addInitiatorChar(std::string term){initiatorChar += term; return;}
	virtual void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	virtual void setTerminatorChar(std::string term){terminatorChar = term; return;}
	virtual void setPrintStatement(bool print){printStatement = print; return;}
	virtual void print(){std::cout << " "; return;}
	virtual dtype evaluate(){return dtype();}
	virtual void print_evaluate(){std::cout << " "; return;};
	virtual llvm::Value* generate(bool loadVariable = false){return Builder.getInt32(0);};
};

class unary_minus_node : public expression_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	expression_node *expNode;
public:
	unary_minus_node(expression_node *exp_node, bool print = false, std::string term = "", std::string init = "");
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	dtype evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class unary_not_node : public expression_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	expression_node *expNode;
public:
	unary_not_node(expression_node *exp_node, bool print = false, std::string term = "", std::string init = "");
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	dtype evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class unary_complement_node : public expression_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	expression_node *expNode;
public:
	unary_complement_node(expression_node *exp_node, bool print = false, std::string term = "", std::string init = "");
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	dtype evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class operator_node : public expression_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	expression_node *leftNode;
	expression_node *rightNode;
	std::string operatorNode;
public:
	operator_node(expression_node *left, std::string op, expression_node *right, bool print = false, std::string term = "", std::string init = "");
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	dtype evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class value_node : public expression_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
public:
	value_node(bool value, bool print = false, std::string term = "", std::string init = "");
	value_node(char value, bool print = false, std::string term = "", std::string init = "");
	value_node(int value, bool print = false, std::string term = "", std::string init = "");
	value_node(float value, bool print = false, std::string term = "", std::string init = "");
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	dtype evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class variable_node : public expression_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	std::string variableID;
public:
	variable_node(std::string id, bool print = false, std::string term = "", std::string init = "");
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	dtype evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class statement_node{
protected:
	std::string initiatorChar;
	std::string terminatorChar;
public:
	virtual void setInitiatorChar(std::string) = 0;
	virtual void addInitiatorChar(std::string) = 0;
	virtual void remInitiatorChar(std::string) = 0;
	virtual void setTerminatorChar(std::string) = 0;
	virtual void setPrintStatement(bool) = 0;
	virtual void print() = 0;
	virtual void evaluate() = 0;
	virtual void print_evaluate() = 0;
	virtual llvm::Value* generate(bool loadVariable = false){return Builder.getInt32(0);};
};

class statement_list_node : public statement_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	std::list<statement_node *> *statementList;
public:
	statement_list_node(std::list<statement_node *> *statement_list, bool print = false, std::string term = "", std::string init = "");
	void insert_statement(statement_node *stmt_node);
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class expression_list_node : public statement_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	std::list<expression_node *> *expressionList;
public:
	expression_list_node(std::list<expression_node *> *expression_list, bool print = false, std::string term = "", std::string init = "");
	void insert_expression(expression_node *stmt_node);
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class expression_statement_node : public statement_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	expression_node *expressionNode;
public:
	expression_statement_node(expression_node *expr_node, bool print = false, std::string term = "", std::string init = "");
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class control_statement_node : public statement_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
public:
	virtual void setInitiatorChar(std::string) = 0;
	virtual void addInitiatorChar(std::string) = 0;
	virtual void remInitiatorChar(std::string) = 0;
	virtual void setTerminatorChar(std::string) = 0;
	virtual void setPrintStatement(bool) = 0;
	virtual void print() = 0;
	virtual void evaluate() = 0;
	virtual void print_evaluate() = 0;
	virtual llvm::Value* generate(bool loadVariable = false){return Builder.getInt32(0);};
};

class tertiary_statement_node : public control_statement_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	dtype dataNode;
	expression_node *expressionNode;
	statement_node *trueExpressionStatement;
	statement_node *falseExpressionStatement;
public:
	tertiary_statement_node(expression_node *expr_node, statement_node *true_statement, statement_node *false_statement, bool print = false, std::string term = "", std::string init = "");
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class if_statement_node : public control_statement_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	std::list<std::string> *condType;
	std::list<expression_node *> *expressionList;
	std::list<statement_list_node *> *statementList;
public:
	if_statement_node(std::list<std::string> *cond_type, std::list<expression_node *> *expression_list, std::list<statement_list_node *> *statement_list, bool print = false, std::string term = "", std::string init = "");
	void insert_condition(std::string cond_type, expression_node *expr_node, statement_list_node *stmt_node);
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class loop_statement_node : public statement_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
public:
	virtual void setInitiatorChar(std::string) = 0;
	virtual void addInitiatorChar(std::string) = 0;
	virtual void remInitiatorChar(std::string) = 0;
	virtual void setTerminatorChar(std::string) = 0;
	virtual void setPrintStatement(bool) = 0;
	virtual void print() = 0;
	virtual void evaluate() = 0;
	virtual void print_evaluate() = 0;
	virtual llvm::Value* generate(bool loadVariable = false){return Builder.getInt32(0);};
};

class while_statement_node : public loop_statement_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	expression_node *expressionNode;
	statement_list_node *statementList;
public:
	while_statement_node(expression_node *expr_node, statement_list_node *statement_list, bool print = false, std::string term = "", std::string init = "");
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class for_statement_node : public loop_statement_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	statement_node *initNode;
	expression_node *termNode;
	statement_node *modfNode;
	statement_list_node *statementList;
public:
	for_statement_node(statement_node *init_node, expression_node *term_node, statement_node *modf_node, statement_list_node *statement_list, bool print = false, std::string term = "", std::string init = "");
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class declaration_statement_node : public statement_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	std::string variableID, dataType;
	expression_node *expressionNode;
public:
	declaration_statement_node(std::string data_type, std::string id, expression_node *expr_node, bool print = false, std::string term = "", std::string init = "");
	// declaration_statement_node(std::string id, bool print = false, std::string term = "", std::string init = "");
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class assignment_statement_node : public statement_node {
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	std::string variableID;
	expression_node *expressionNode;
public:
	assignment_statement_node(std::string id, expression_node *expr_node, bool print = false, std::string term = "", std::string init = "");
	void setInitiatorChar(std::string init){initiatorChar = init; return;}
	void addInitiatorChar(std::string term){initiatorChar += term; return;}
	void remInitiatorChar(std::string term){if(initiatorChar.length() >= term.length() && initiatorChar.substr(initiatorChar.length() - term.length(), term.length()) == term){initiatorChar = initiatorChar.substr(0, initiatorChar.length() - term.length());} return;}
	void setTerminatorChar(std::string term){terminatorChar = term; return;}
	void setPrintStatement(bool print){printStatement = print; return;}
	void print();
	void evaluate();
	void print_evaluate();
	llvm::Value* generate(bool loadVariable = false);
};

class program_node{
protected:
	std::string initiatorChar;
	std::string terminatorChar;
	bool printStatement;
	statement_list_node *statementList;
public:
	program_node(statement_list_node *stmt_list, bool print = false, std::string term = "", std::string init = "");
	void evaluate();
	void print_evaluate();
	void generate(bool loadVariable = false);
};

extern program_node *root;
extern std::map<std::string, dtype> value_table;
extern std::map<std::string, bool> id_table;
extern std::map<std::string, llvm::Value*> symbol_table;
extern std::map<std::string, llvm::GlobalVariable*> variable_table;
