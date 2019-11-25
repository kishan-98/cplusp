#include "cplusp.h"

std::map<std::string, dtype> value_table;
std::map<std::string, bool> id_table;

// using namespace llvm;

// The function getGlobalContext is depricated, so use declaration without definition
// static LLVMContext &Context = getGlobalContext();
static llvm::LLVMContext Context;
static llvm::Module *ModuleOb = new llvm::Module("my compiler", Context);
static std::vector<std::string> FunArgs;
typedef llvm::SmallVector<llvm::BasicBlock *, 16> BBList;
typedef llvm::SmallVector<llvm::Value *, 16> ValList;

llvm::Function *createFunc(llvm::IRBuilder<> &Builder, std::string Name) {
	std::vector<llvm::Type *> Integers(FunArgs.size(), llvm::Type::getInt32Ty(Context));
	llvm::FunctionType *funcType = llvm::FunctionType::get(Builder.getInt32Ty(), Integers, false);
	llvm::Function *fooFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, Name, ModuleOb);
	return fooFunc;
}

void setFuncArgs(llvm::Function *fooFunc, std::vector<std::string> FunArgs) {
	unsigned Idx = 0;
	llvm::Function::arg_iterator AI, AE;
	for (AI = fooFunc->arg_begin(), AE = fooFunc->arg_end(); AI != AE; ++AI, ++Idx){
		AI->setName(FunArgs[Idx]);
	}
}

llvm::BasicBlock *createBB(llvm::Function *fooFunc, std::string Name) {
	return llvm::BasicBlock::Create(Context, Name, fooFunc);
}

llvm::GlobalVariable *createGlob(llvm::IRBuilder<> &Builder, std::string Name) {
	ModuleOb->getOrInsertGlobal(Name, Builder.getInt32Ty());
	llvm::GlobalVariable *gVar = ModuleOb->getNamedGlobal(Name);
	gVar->setLinkage(llvm::GlobalValue::CommonLinkage);
	gVar->setAlignment(4);
	return gVar;
}

llvm::Value *createArith(llvm::IRBuilder<> &Builder, llvm::Value *L, llvm::Value *R) {
	return Builder.CreateMul(L, R, "multmp");
}

llvm::Value *createIfElse(llvm::IRBuilder<> &Builder, BBList List, ValList VL) {
	llvm::Value *Condtn = VL[0];
	llvm::Value *Arg1 = VL[1];
	llvm::BasicBlock *ThenBB = List[0];
	llvm::BasicBlock *ElseBB = List[1];
	llvm::BasicBlock *MergeBB = List[2];
	Builder.CreateCondBr(Condtn, ThenBB, ElseBB);
	Builder.SetInsertPoint(ThenBB);
	llvm::Value *ThenVal = Builder.CreateAdd(Arg1, Builder.getInt32(1), "thenaddtmp");
	Builder.CreateBr(MergeBB);
	Builder.SetInsertPoint(ElseBB);
	llvm::Value *ElseVal = Builder.CreateAdd(Arg1, Builder.getInt32(2), "elseaddtmp");
	Builder.CreateBr(MergeBB);
	unsigned PhiBBSize = List.size() - 1;
	Builder.SetInsertPoint(MergeBB);
	llvm::LLVMContext tempContext;
	llvm::PHINode *Phi = Builder.CreatePHI(llvm::Type::getInt32Ty(Context), PhiBBSize, "iftmp");
	Phi->addIncoming(ThenVal, ThenBB);
	Phi->addIncoming(ElseVal, ElseBB);
	return Phi;
}

llvm::Value *createLoop(llvm::IRBuilder<> &Builder, BBList List, ValList VL, llvm::Value *StartVal, llvm::Value *EndVal) {
	llvm::BasicBlock *PreheaderBB = Builder.GetInsertBlock();
	llvm::Value *val = VL[0];
	llvm::BasicBlock *LoopBB = List[0];
	Builder.CreateBr(LoopBB);
	Builder.SetInsertPoint(LoopBB);
	llvm::PHINode *IndVar = Builder.CreatePHI(llvm::Type::getInt32Ty(Context), 2, "i");
	IndVar->addIncoming(StartVal, PreheaderBB);
	llvm::Value *Add = Builder.CreateAdd(val, Builder.getInt32(5), "addtmp");
	llvm::Value *StepVal = Builder.getInt32(1);
	llvm::Value *NextVal = Builder.CreateAdd(IndVar, StepVal, "nextval");
	llvm::Value *EndCond = Builder.CreateICmpULT(IndVar, EndVal, "endcond");
	EndCond = Builder.CreateICmpNE(EndCond, Builder.getInt32(0), "loopcond");
	llvm::BasicBlock *LoopEndBB = Builder.GetInsertBlock();
	llvm::BasicBlock *AfterBB = List[1];
	Builder.CreateCondBr(EndCond, LoopBB, AfterBB);
	Builder.SetInsertPoint(AfterBB);
	IndVar->addIncoming(NextVal, LoopEndBB);
	return Add;
}

operator_node::operator_node(expression_node *left, std::string op, expression_node *right, bool print, std::string term, std::string init){
	initiatorChar	=	init;
	terminatorChar	=	term;
	printStatement 	= 	print;
	leftNode 		=	left;
	operatorNode	=	op;
	rightNode 		=	right;
}
void operator_node::print(){
	std::cout << initiatorChar;
	std::cout << "<# "; leftNode->print(); std::cout << " " << operatorNode << " "; rightNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	return;
}
dtype operator_node::evaluate(){
	dtype	leftDataNode 	=	leftNode->evaluate();
	dtype	rightDataNode 	=	rightNode->evaluate();
	dataNode = leftDataNode.operate(operatorNode, rightDataNode);
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ operator_node: "; leftDataNode.print(); std::cout << " " << operatorNode << " "; rightDataNode.print(); std::cout << " = "; dataNode.print(); std::cout << " $>" << std::endl;
	}
	return dataNode;
}
void operator_node::print_evaluate(){
	std::cout << initiatorChar;
	std::cout << "<# "; leftNode->print(); std::cout << " " << operatorNode << " "; rightNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	dtype	leftDataNode 	=	leftNode->evaluate();
	dtype	rightDataNode 	=	rightNode->evaluate();
	dataNode = leftDataNode.operate(operatorNode, rightDataNode);
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ operator_node: "; leftDataNode.print(); std::cout << " " << operatorNode << " "; rightDataNode.print(); std::cout << " = "; dataNode.print(); std::cout << " $>" << std::endl;
	}
	return;
}


unary_minus_node::unary_minus_node(expression_node *exp_node, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expNode(exp_node) {
}
void unary_minus_node::print(){
	std::cout << initiatorChar;
	std::cout << "<# - "; expNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	return;
}
dtype unary_minus_node::evaluate(){
	dtype expData 	=	expNode->evaluate();
	dataNode = expData.negate();
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ unary_minus_node: - "; expData.print(); std::cout << " = "; dataNode.print(); std::cout << " $>" << std::endl;
	}
	return dataNode;
}
void unary_minus_node::print_evaluate(){
	std::cout << initiatorChar;
	std::cout << "<# - "; expNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	dtype expData 	=	expNode->evaluate();
	dataNode = expData.negate();
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ unary_minus_node: - "; expData.print(); std::cout << " = "; dataNode.print(); std::cout << " $>" << std::endl;
	}
	return;
}


unary_not_node::unary_not_node(expression_node *exp_node, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expNode(exp_node) {
}
void unary_not_node::print(){
	std::cout << initiatorChar;
	std::cout << "<# ! "; expNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	return;
}
dtype unary_not_node::evaluate(){
	dtype expData 	=	expNode->evaluate();
	dataNode = expData.not_logical();
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ unary_not_node: ! "; expData.print(); std::cout << " = "; dataNode.print(); std::cout << " $>" << std::endl;
	}
	return dataNode;
}
void unary_not_node::print_evaluate(){
	std::cout << initiatorChar;
	std::cout << "<# ! "; expNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	dtype expData 	=	expNode->evaluate();
	dataNode = expData.not_logical();
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ unary_not_node: ! "; expData.print(); std::cout << " = "; dataNode.print(); std::cout << " $>" << std::endl;
	}
	return;
}


unary_complement_node::unary_complement_node(expression_node *exp_node, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expNode(exp_node) {
}
void unary_complement_node::print(){
	std::cout << initiatorChar;
	std::cout << "<# ~ "; expNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	return;
}
dtype unary_complement_node::evaluate(){
	dtype expData 	=	expNode->evaluate();
	dataNode = expData.not_binary();
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ unary_complement_node: ~ "; expData.print(); std::cout << " = "; dataNode.print(); std::cout << " $>" << std::endl;
	}
	return dataNode;
}
void unary_complement_node::print_evaluate(){
	std::cout << initiatorChar;
	std::cout << "<# ~ "; expNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	dtype expData 	=	expNode->evaluate();
	dataNode = expData.not_binary();
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ unary_complement_node: ~ "; expData.print(); std::cout << " = "; dataNode.print(); std::cout << " $>" << std::endl;
	}
	return;
}


value_node::value_node(bool value, bool print, std::string term, std::string init){
	initiatorChar	=	init;
	terminatorChar	=	term;
	printStatement 	= 	print;
	dataNode 		=	dtype(value);
}
value_node::value_node(char value, bool print, std::string term, std::string init){
	initiatorChar	=	init;
	terminatorChar	=	term;
	printStatement 	= 	print;
	dataNode 		=	dtype(value);
}
value_node::value_node(int value, bool print, std::string term, std::string init){
	initiatorChar	=	init;
	terminatorChar	=	term;
	printStatement 	= 	print;
	dataNode 		=	dtype(value);
}
value_node::value_node(float value, bool print, std::string term, std::string init){
	initiatorChar	=	init;
	terminatorChar	=	term;
	printStatement 	= 	print;
	dataNode 		=	dtype(value);
}
void value_node::print(){
	std::cout << initiatorChar;
	dataNode.print();
	std::cout << terminatorChar;
	return;
}
dtype value_node::evaluate(){
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ value: "; dataNode.print(); std::cout << " $>" << std::endl;
	}
	return dataNode;
}
void value_node::print_evaluate(){
	std::cout << initiatorChar;
	dataNode.print();
	std::cout << terminatorChar;
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ value: "; dataNode.print(); std::cout << " $>" << std::endl;
	}
	return;
}


variable_node::variable_node(std::string id, bool print, std::string term, std::string init){
	initiatorChar	=	init;
	terminatorChar	=	term;
	printStatement = print;
	variableID = id;
}
void variable_node::print(){
	std::cout << initiatorChar;
	std::cout << variableID;
	std::cout << terminatorChar;
	return;
}
dtype variable_node::evaluate(){
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ variable: " << variableID << " = "; value_table[variableID].print(); std::cout << " $>" << std::endl;
	}
	return value_table[variableID];
}
void variable_node::print_evaluate(){
	std::cout << initiatorChar;
	std::cout << variableID;
	std::cout << terminatorChar;
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ variable: " << variableID << " = "; value_table[variableID].print(); std::cout << " $>" << std::endl;
	}
	return;
}


statement_list_node::statement_list_node(std::list<statement_node *> *statement_list, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), statementList(statement_list) {
}
void statement_list_node::insert_statement(statement_node *stmt_node){
	// statementList->push_front(stmt_node);
	statementList->push_back(stmt_node);
	return;
}
void statement_list_node::print(){
	std::list<statement_node *>::iterator stmtIt;
	for(stmtIt = statementList->begin(); stmtIt != statementList->end(); stmtIt++){
		(*stmtIt)->setInitiatorChar(initiatorChar);
		(*stmtIt)->print();
	}
	std::cout << terminatorChar;
	return;
}
void statement_list_node::evaluate(){
	std::list<statement_node *>::iterator stmtIt;
	for(stmtIt = statementList->begin(); stmtIt != statementList->end(); stmtIt++){
		(*stmtIt)->evaluate();
		if(printStatement){
			std::cout << initiatorChar;
			std::cout << "\t<$ statement_list_node: "; (*stmtIt)->print(); std::cout << " $>" << std::endl;
		}
	}
	return;
}
void statement_list_node::print_evaluate(){
	std::list<statement_node *>::iterator stmtIt;
	for(stmtIt = statementList->begin(); stmtIt != statementList->end(); stmtIt++){
		(*stmtIt)->setInitiatorChar(initiatorChar);
		(*stmtIt)->print();
		(*stmtIt)->evaluate();
		if(printStatement){
			std::cout << initiatorChar;
			std::cout << "\t<$ statement_list_node: "; (*stmtIt)->print(); std::cout << " $>" << std::endl;
		}
	}
	std::cout << terminatorChar;
	return;
}


expression_statement_node::expression_statement_node(expression_node *expr_node, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expressionNode(expr_node) {
}
void expression_statement_node::print(){
	std::cout << initiatorChar;
	std::cout << "<# "; expressionNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	return;
}
void expression_statement_node::evaluate(){
	expressionNode->evaluate();
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ expression_statement_node: "; expressionNode->print(); std::cout << " $>" << std::endl;
	}
	return;
}
void expression_statement_node::print_evaluate(){
	std::cout << initiatorChar;
	std::cout << "<# "; expressionNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	expressionNode->evaluate();
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ expression_statement_node: "; expressionNode->print(); std::cout << " $>" << std::endl;
	}
	return;
}


expression_list_node::expression_list_node(std::list<expression_node *> *expression_list, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expressionList(expression_list) {
}
void expression_list_node::insert_expression(expression_node *stmt_node){
	expressionList->push_front(stmt_node);
	return;
}
void expression_list_node::print(){
	std::list<expression_node *>::iterator stmtIt;
	for(stmtIt = expressionList->begin(); stmtIt != expressionList->end(); stmtIt++){
		std::cout << initiatorChar;
		std::cout << "<# "; (*stmtIt)->print(); std::cout << " #>";
	}
	std::cout << terminatorChar;
	return;
}
void expression_list_node::evaluate(){
	std::list<expression_node *>::iterator stmtIt;
	for(stmtIt = expressionList->begin(); stmtIt != expressionList->end(); stmtIt++){
		(*stmtIt)->evaluate();
		if(printStatement){
			std::cout << initiatorChar;
			std::cout << "\t<$ expression_list_node: "; (*stmtIt)->print(); std::cout << " $>" << std::endl;
		}
	}
	return;
}
void expression_list_node::print_evaluate(){
	std::list<expression_node *>::iterator exprIt;
	for(exprIt = expressionList->begin(); exprIt != expressionList->end(); exprIt++){
		std::cout << initiatorChar;
		std::cout << "<# "; (*exprIt)->print(); std::cout << " #>";
		(*exprIt)->evaluate();
		if(printStatement){
			std::cout << initiatorChar;
			std::cout << "\t<$ expression_list_node: "; (*exprIt)->print(); std::cout << " $>" << std::endl;
		}
	}
	std::cout << terminatorChar;
	return;
}


tertiary_statement_node::tertiary_statement_node(expression_node *expr_node, statement_node *true_statement, statement_node *false_statement, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expressionNode(expr_node), trueExpressionStatement(true_statement), falseExpressionStatement(false_statement) {
}
void tertiary_statement_node::print(){
	std::cout << initiatorChar;
	std::cout << "<# "; expressionNode->print(); std::cout << " ? "; trueExpressionStatement->print(); std::cout << " : "; falseExpressionStatement->print(); std::cout << " #>";
	std::cout << terminatorChar;
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
		std::cout << initiatorChar;
		std::cout << "\t<$ tertiary_statement_node: "; (dataNode.evaluate() ? trueExpressionStatement->print() : falseExpressionStatement->print()); std::cout << " $>" << std::endl;
	}
	return;
}
void tertiary_statement_node::print_evaluate(){
	std::cout << initiatorChar;
	std::cout << "<# "; expressionNode->print(); std::cout << " ? "; trueExpressionStatement->print(); std::cout << " : "; falseExpressionStatement->print(); std::cout << " #>";
	std::cout << terminatorChar;
	dataNode = expressionNode->evaluate();
	if(dataNode.evaluate()){
		trueExpressionStatement->evaluate();
	}
	else{
		falseExpressionStatement->evaluate();
	}
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ tertiary_statement_node: "; (dataNode.evaluate() ? trueExpressionStatement->print() : falseExpressionStatement->print()); std::cout << " $>" << std::endl;
	}
	return;
}


if_statement_node::if_statement_node(std::list<std::string> *cond_type, std::list<expression_node *> *expression_list, std::list<statement_list_node *> *statement_list, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), condType(cond_type), expressionList(expression_list), statementList(statement_list) {
}
void if_statement_node::insert_condition(std::string cond_type, expression_node *expr_node, statement_list_node *stmt_node){
	condType->push_front(cond_type);
	expressionList->push_front(expr_node);
	statementList->push_front(stmt_node);
	return;
}
void if_statement_node::print(){
	std::list<std::string>::iterator strIt;
	std::list<expression_node *>::iterator expIt;
	std::list<statement_list_node *>::iterator stmtIt;
	std::string nextInitiatorChar = initiatorChar + "\t";
	std::cout << std::endl;
	for(strIt = condType->begin(), expIt = expressionList->begin(), stmtIt = statementList->begin(); strIt != condType->end() && expIt != expressionList->end() && stmtIt != statementList->end(); strIt++, expIt++, stmtIt++){
		std::cout << initiatorChar;
		std::cout << "<# " << (*strIt) << " "; (*expIt)->print(); std::cout << " THEN " << std::endl;
		(*stmtIt)->setInitiatorChar(nextInitiatorChar);
		(*stmtIt)->print();
		std::cout << initiatorChar;
		std::cout << "   " << (*strIt) << " #>";
	}
	std::cout << terminatorChar << std::endl;
	return;
}
void if_statement_node::evaluate(){
	dtype dataNode;
	std::list<expression_node *>::iterator expIt;
	std::list<statement_list_node *>::iterator stmtIt;
	for(expIt = expressionList->begin(), stmtIt = statementList->begin(); expIt != expressionList->end() && stmtIt != statementList->end(); expIt++, stmtIt++){
		dataNode = (*expIt)->evaluate();
		if(dataNode.evaluate()){
			(*stmtIt)->evaluate();
			if(printStatement){
				std::cout << initiatorChar;
				std::cout << "\t<$ if_statement_node: "; (*stmtIt)->print(); std::cout << " $>" << std::endl;
			}
			break;
		}
	}
	return;
}
void if_statement_node::print_evaluate(){
	std::list<std::string>::iterator strIt;
	std::list<expression_node *>::iterator expIt;
	std::list<statement_list_node *>::iterator stmtIt;
	dtype dataNode;
	bool flag = true;
	std::string nextInitiatorChar = initiatorChar + "\t";
	std::cout << std::endl;
	for(strIt = condType->begin(), expIt = expressionList->begin(), stmtIt = statementList->begin(); strIt != condType->end() && expIt != expressionList->end() && stmtIt != statementList->end(); strIt++, expIt++, stmtIt++){
		std::cout << initiatorChar;
		std::cout << "<# " << (*strIt) << " "; (*expIt)->print(); std::cout << " THEN " << std::endl;
		(*stmtIt)->setInitiatorChar(nextInitiatorChar);
		(*stmtIt)->print();
		std::cout << initiatorChar;
		std::cout << "   " << (*strIt) << " #>";
		if(flag){
			dataNode = (*expIt)->evaluate();
			if(dataNode.evaluate()){
				(*stmtIt)->evaluate();
				if(printStatement){
					std::cout << initiatorChar;
					std::cout << "\t<$ if_statement_node: "; (*stmtIt)->print(); std::cout << " $>" << std::endl;
				}
				flag = false;
			}
		}
	}
	std::cout << terminatorChar << std::endl;
	return;
}


while_statement_node::while_statement_node(expression_node *expr_node, statement_list_node *statement_list, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expressionNode(expr_node), statementList(statement_list){
}
void while_statement_node::print(){
	std::string nextInitiatorChar = initiatorChar + "\t";
	std::cout << std::endl;
	std::cout << initiatorChar;
	std::cout << "<# WHILE "; expressionNode->print(); std::cout << " DO" << std::endl;
	statementList->setInitiatorChar(nextInitiatorChar);
	statementList->print();
	std::cout << initiatorChar;
	std::cout << "   WHILE #>";
	std::cout << terminatorChar << std::endl;
	return;
}
void while_statement_node::evaluate(){
	dtype dataNode = expressionNode->evaluate();
	while(dataNode.evaluate()){
		statementList->evaluate();
		if(printStatement){
			std::cout << initiatorChar;
			std::cout << "\t<$ while_statement_node: "; statementList->print(); std::cout << " $>" << std::endl;
		}
		dataNode = expressionNode->evaluate();
	}
	return;
}
void while_statement_node::print_evaluate(){
	std::string nextInitiatorChar = initiatorChar + "\t";
	std::cout << std::endl;
	std::cout << initiatorChar;
	std::cout << "<# WHILE "; expressionNode->print(); std::cout << " DO" << std::endl;
	statementList->setInitiatorChar(nextInitiatorChar);
	statementList->print();
	std::cout << initiatorChar;
	std::cout << "   WHILE #>";
	dtype dataNode = expressionNode->evaluate();
	while(dataNode.evaluate()){
		statementList->evaluate();
		if(printStatement){
			std::cout << initiatorChar;
			std::cout << "\t<$ while_statement_node: "; statementList->print(); std::cout << " $>" << std::endl;
		}
	}
	std::cout << terminatorChar << std::endl;
	return;
}


for_statement_node::for_statement_node(statement_node *init_node, expression_node *term_node, statement_node *modf_node, statement_list_node *statement_list, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), initNode(init_node), termNode(term_node), modfNode(modf_node), statementList(statement_list){
}
void for_statement_node::print(){
	std::string nextInitiatorChar = initiatorChar + "\t";
	std::cout << std::endl;
	std::cout << initiatorChar;
	std::cout << "<# FOR "; initNode->print(); std::cout << " UNLESS "; termNode->print(); std::cout << " UPDATE BY "; modfNode->print(); std::cout << " DO" << std::endl;
	statementList->setInitiatorChar(nextInitiatorChar);
	statementList->print();
	std::cout << initiatorChar;
	std::cout << "   FOR #>";
	std::cout << terminatorChar << std::endl;
	return;
}
void for_statement_node::evaluate(){
	initNode->evaluate();
	dtype condNode = termNode->evaluate();
	while(condNode.evaluate()){
		statementList->evaluate();
		if(printStatement){
			std::cout << initiatorChar;
			std::cout << "\t<$ for_statement_node: "; statementList->print(); std::cout << " $>" << std::endl;
		}
		modfNode->evaluate();
		condNode = termNode->evaluate();
	}
	return;
}
void for_statement_node::print_evaluate(){
	std::string nextInitiatorChar = initiatorChar + "\t";
	std::cout << std::endl;
	std::cout << initiatorChar;
	std::cout << "<# FOR "; initNode->print(); std::cout << " UNLESS "; termNode->print(); std::cout << " UPDATE BY "; modfNode->print(); std::cout << " DO" << std::endl;
	statementList->setInitiatorChar(nextInitiatorChar);
	statementList->print();
	std::cout << initiatorChar;
	std::cout << "   FOR #>";
	initNode->evaluate();
	dtype condNode = termNode->evaluate();
	while(condNode.evaluate()){
		statementList->evaluate();
		if(printStatement){
			std::cout << initiatorChar;
			std::cout << "\t<$ for_statement_node: "; statementList->print(); std::cout << " $>" << std::endl;
		}
		modfNode->evaluate();
		condNode = termNode->evaluate();
	}
	std::cout << terminatorChar << std::endl;
	return;
}


declaration_statement_node::declaration_statement_node(std::string data_type, std::string id, expression_node *expr_node, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), variableID(id), dataType(data_type), expressionNode(expr_node) {
}
void declaration_statement_node::print(){
	std::cout << initiatorChar;
	std::cout << "<# " << variableID << " = "; expressionNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	return;
}
void declaration_statement_node::evaluate(){
	// value_table[variableID] = expressionNode->evaluate();
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ declaration_statement_node: " << variableID << " = "; value_table[variableID].print(); std::cout << " $>" << std::endl;
	}
	return;
}
void declaration_statement_node::print_evaluate(){
	std::cout << initiatorChar;
	std::cout << "<# " << variableID << " = "; expressionNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	// value_table[variableID] = expressionNode->evaluate();
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ declaration_statement_node: " << variableID << " = "; value_table[variableID].print(); std::cout << " $>" << std::endl;
	}
	return;
}


assignment_statement_node::assignment_statement_node(std::string id, expression_node *expr_node, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), variableID(id), expressionNode(expr_node) {
}
void assignment_statement_node::print(){
	std::cout << initiatorChar;
	std::cout << "<# " << variableID << " = "; expressionNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	return;
}
void assignment_statement_node::evaluate(){
	value_table[variableID] = expressionNode->evaluate();
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ assignment_statement_node: " << variableID << " = "; value_table[variableID].print(); std::cout << " $>" << std::endl;
	}
	return;
}
void assignment_statement_node::print_evaluate(){
	std::cout << initiatorChar;
	std::cout << "<# " << variableID << " = "; expressionNode->print(); std::cout << " #>";
	std::cout << terminatorChar;
	value_table[variableID] = expressionNode->evaluate();
	if(printStatement){
		std::cout << initiatorChar;
		std::cout << "\t<$ assignment_statement_node: " << variableID << " = "; value_table[variableID].print(); std::cout << " $>" << std::endl;
	}
	return;
}

program_node::program_node(statement_list_node *stmt_list, bool print, std::string term, std::string init): initiatorChar(init), terminatorChar(term), printStatement(print), statementList(stmt_list) {
}
void program_node::evaluate(){
	if(printStatement){
		std::cout << "===================================================PRINT========================================================================" << std::endl;
		statementList->print();
		std::cout << "================================================EVALUATE========================================================================" << std::endl;
		statementList->evaluate();
		// std::cout << "================================================PRINT+EVALUATE==================================================================" << std::endl;
		// statementList->print_evaluate();
		std::cout << "================================================================================================================================" << std::endl;
	}
	return;
}
