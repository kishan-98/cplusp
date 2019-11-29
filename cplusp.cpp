#include "cplusp.h"

std::map<std::string, dtype> value_table;
std::map<std::string, std::vector<dtype> > array1_table;
std::map<std::string, std::vector<std::vector<dtype> > > array2_table;
std::map<std::string, std::pair<int, int> > array_size;
std::map<std::string, bool> id_table;
std::map<std::string, llvm::Value*> symbol_table;
std::map<std::string, llvm::GlobalVariable*> variable_table;
llvm::Function *mainFunc;

llvm::Function *createFunc(llvm::IRBuilder<> &Builder, std::string Name) {
	std::vector<llvm::Type *> Integers(FuncArgs.size(), llvm::Type::getInt32Ty(Context));
	llvm::FunctionType *funcType = llvm::FunctionType::get(Builder.getInt32Ty(), Integers, false);
	llvm::Function *Func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, Name, ModuleOb);
	return Func;
}

void setFuncArgs(llvm::Function *Func, std::vector<std::string> FuncArgs) {
	unsigned Idx = 0;
	llvm::Function::arg_iterator AI, AE;
	for (AI = Func->arg_begin(), AE = Func->arg_end(); AI != AE; ++AI, ++Idx){
		AI->setName(FuncArgs[Idx]);
	}
}

llvm::BasicBlock *createBB(llvm::Function *Func, std::string Name) {
	return llvm::BasicBlock::Create(Context, Name, Func);
}

llvm::GlobalVariable *createGlob(llvm::IRBuilder<> &Builder, std::string Name) {
	ModuleOb->getOrInsertGlobal(Name, Builder.getInt32Ty());
	llvm::GlobalVariable *gVar = ModuleOb->getNamedGlobal(Name);
	gVar->setLinkage(llvm::GlobalValue::CommonLinkage);
	gVar->setInitializer(llvm::ConstantInt::get(Context, llvm::APInt(32,0)));
	gVar->setAlignment(4);
	return gVar;
}

llvm::GlobalVariable *createGlob(llvm::IRBuilder<> &Builder, std::string Name, int N1) {
	llvm::ArrayType * arr_type = llvm::ArrayType::get(llvm::Type::getInt32Ty(Context), N1);
    //PointerType * ptr_typ = PointerType::get(arr_type, 0);
    llvm::GlobalVariable * gVar = new llvm::GlobalVariable(*ModuleOb, arr_type, false, llvm::GlobalValue::ExternalLinkage, 0, Name);
    gVar->setInitializer(llvm::ConstantAggregateZero::get(arr_type));
	return gVar;
}

llvm::GlobalVariable *createGlob(llvm::IRBuilder<> &Builder, std::string Name, int N1, int N2) {
	llvm::ArrayType * arr_type = llvm::ArrayType::get(llvm::Type::getInt32Ty(Context), N1*N2);
    //PointerType * ptr_typ = PointerType::get(arr_type, 0);
    llvm::GlobalVariable * gVar = new llvm::GlobalVariable(*ModuleOb, arr_type, false, llvm::GlobalValue::ExternalLinkage, 0, Name);
    gVar->setInitializer(llvm::ConstantAggregateZero::get(arr_type));
	return gVar;
}

llvm::Value* createArith(llvm::IRBuilder<> &Builder, std::string op, llvm::Value *L, llvm::Value *R){
		 if(op == "+")return Builder.CreateAdd(L, R, "addtmp");
	else if(op == "-")return Builder.CreateSub(L, R, "subtmp");
	else if(op == "*")return Builder.CreateMul(L, R, "multmp");
	// else if(op == "/")return Builder.CreateDiv(L, R, "addtmp");
	// else if(op == "%")return Builder.CreateRem(L, R, "addtmp");
	// else if(op == "-")return Builder.CreateSub(L, R, "addtmp");
	// else if(op == "-")return Builder.CreateSub(L, R, "addtmp");
	return Builder.CreateAdd(L, R, "addtmp");
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
llvm::Value* operator_node::generate(bool loadVariable){
		// std::cout << "This is generator function in operator_node" << std::endl;
		llvm::Value *L = leftNode->generate();
		llvm::Value *R = rightNode->generate();
		return createArith(Builder, operatorNode, L, R);
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
llvm::Value* unary_minus_node::generate(bool loadVariable){
		// std::cout << "This is generator function in unary_minus_node" << std::endl;
		llvm::Value *E = expNode->generate();
		return Builder.CreateNeg(E, "negtmp");
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
llvm::Value* unary_not_node::generate(bool loadVariable){
		// std::cout << "This is generator function in unary_not_node" << std::endl;
		llvm::Value *E = expNode->generate();
		return Builder.CreateNot(E, "nottmp");
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
llvm::Value* unary_complement_node::generate(bool loadVariable){
		// std::cout << "This is generator function in unary_complement_node" << std::endl;
		llvm::Value *E = expNode->generate();
		return Builder.CreateNot(E, "comptmp");
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
llvm::Value* value_node::generate(bool loadVariable){
		// std::cout << "This is generator function in value_node" << std::endl;
		return dataNode.generate();
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
llvm::Value* variable_node::generate(bool loadVariable){
		// std::cout << "This is generator function in variable_node" << std::endl;
		if(!variable_table[variableID]){
			variable_table[variableID] = createGlob(Builder, variableID);
			// llvm::ConstantInt* const_int_val = llvm::ConstantInt::get(ModuleOb->getContext(), llvm::APInt(32,0));
			// variable_table[variableID]->setInitializer(const_int_val);
			// symbol_table[variableID] = variable_table[variableID]->getInitializer();
			symbol_table[variableID] = Builder.CreateStore(Builder.getInt32(0), variable_table[variableID]);
		}
		symbol_table[variableID] = Builder.CreateLoad(variable_table[variableID]);
		// if(!symbol_table[variableID]){
		// 	symbol_table[variableID] = Builder.getInt32(0);
		// }
		return symbol_table[variableID];
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
llvm::Value* statement_list_node::generate(bool loadVariable){
		// std::cout << "This is generator function in statement_list_node" << std::endl;
		std::list<statement_node *>::iterator stmtIt;
		int statementCount = 0;
		for(stmtIt = statementList->begin(); stmtIt != statementList->end(); stmtIt++){
			(*stmtIt)->generate();
			statementCount++;
		}
		return Builder.getInt32(statementCount);
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
llvm::Value* expression_statement_node::generate(bool loadVariable){
		// std::cout << "This is generator function in expression_statement_node" << std::endl;
		return expressionNode->generate();
}


expression_list_node::expression_list_node(std::list<expression_node *> *expression_list, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), expressionList(expression_list) {
}
void expression_list_node::insert_expression(expression_node *stmt_node){
	expressionList->push_front(stmt_node);
	return;
}
void expression_list_node::print(){
	std::list<expression_node *>::iterator exprIt;
	for(exprIt = expressionList->begin(); exprIt != expressionList->end(); exprIt++){
		std::cout << initiatorChar;
		std::cout << "<# "; (*exprIt)->print(); std::cout << " #>";
	}
	std::cout << terminatorChar;
	return;
}
void expression_list_node::evaluate(){
	std::list<expression_node *>::iterator exprIt;
	for(exprIt = expressionList->begin(); exprIt != expressionList->end(); exprIt++){
		(*exprIt)->evaluate();
		if(printStatement){
			std::cout << initiatorChar;
			std::cout << "\t<$ expression_list_node: "; (*exprIt)->print(); std::cout << " $>" << std::endl;
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
llvm::Value* expression_list_node::generate(bool loadVariable){
		// std::cout << "This is generator function in expression_list_node" << std::endl;
		std::list<expression_node *>::iterator exprIt;
		int expressionCount = 0;
		for(exprIt = expressionList->begin(); exprIt != expressionList->end(); exprIt++){
			(*exprIt)->generate();
			expressionCount++;
		}
		return Builder.getInt32(expressionCount);
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
llvm::Value* tertiary_statement_node::generate(bool loadVariable){
		// std::cout << "This is generator function in tertiary_statement_node" << std::endl;
		llvm::BasicBlock *TrueBB = createBB(mainFunc, "trueBlock");
		llvm::BasicBlock *FalseBB = createBB(mainFunc, "falseBlock");
		llvm::BasicBlock *MergeBB = createBB(mainFunc, "mergeBlock");
		llvm::BasicBlock *entry = createBB(mainFunc, "entry");
		llvm::Value *Condtn = Builder.CreateICmpNE(expressionNode->generate(), Builder.getInt32(0), "ifCond");
		Builder.CreateCondBr(Condtn, TrueBB, FalseBB);
		Builder.SetInsertPoint(TrueBB);
		trueExpressionStatement->generate();
		llvm::Value *TrueVal = Builder.getInt32(0);
		Builder.CreateBr(MergeBB);
		Builder.SetInsertPoint(FalseBB);
		falseExpressionStatement->generate();
		llvm::Value *FalseVal = Builder.getInt32(1);
		Builder.CreateBr(MergeBB);
		unsigned PhiBBSize = 2;
		Builder.SetInsertPoint(MergeBB);
		llvm::PHINode *Phi = Builder.CreatePHI(llvm::Type::getInt32Ty(Context), PhiBBSize, "tertBlock");
		Phi->addIncoming(TrueVal, TrueBB);
		Phi->addIncoming(FalseVal, FalseBB);
		Builder.CreateBr(entry);
		Builder.SetInsertPoint(entry);
		return Phi;
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
llvm::Value* if_statement_node::generate(bool loadVariable){
		// std::cout << "This is generator function in if_statement_node" << std::endl;
		std::list<expression_node *>::iterator expIt;
		std::list<statement_list_node *>::iterator stmtIt;
		int i, total = expressionList->size();
		BBList expBB, stmtBB;
		for(i = 0; i < total; i++){
			expBB.push_back(createBB(mainFunc, "ifBlock"));
			stmtBB.push_back(createBB(mainFunc, "stmtBlock"));
		}
		llvm::BasicBlock *MergeBB = createBB(mainFunc, "mergeBlock");
		expBB.push_back(MergeBB);
		llvm::BasicBlock *entry = createBB(mainFunc, "entry");
		Builder.CreateBr(expBB[0]);
		for(i = 0, expIt = expressionList->begin(), stmtIt = statementList->begin(); expIt != expressionList->end() && stmtIt != statementList->end(); expIt++, stmtIt++, i++){
			Builder.SetInsertPoint(expBB[i]);
			llvm::Value *Condtn = Builder.CreateICmpNE((*expIt)->generate(), Builder.getInt32(0), "");
			Builder.CreateCondBr(Condtn, stmtBB[i], expBB[i+1]);
			Builder.SetInsertPoint(stmtBB[i]);
			(*stmtIt)->generate();
			Builder.CreateBr(MergeBB);
		}
		Builder.SetInsertPoint(MergeBB);
		unsigned PhiBBSize = statementList->size();
		llvm::PHINode *Phi = Builder.CreatePHI(llvm::Type::getInt32Ty(Context), PhiBBSize, "ifCond");
		for(i = 0; i < total; i++){
			Phi->addIncoming(Builder.getInt32(i), stmtBB[i]);
		}
		Builder.CreateBr(entry);
		Builder.SetInsertPoint(entry);
		return Phi;
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
llvm::Value* while_statement_node::generate(bool loadVariable){
		// std::cout << "This is generator function in while_statement_node" << std::endl;
		llvm::BasicBlock *LoopBB = createBB(mainFunc, "whileLoopBlock");
		llvm::BasicBlock *AfterBB = createBB(mainFunc, "entry");
		llvm::Value *count = Builder.getInt32(0);
		llvm::Value *NextVal = Builder.CreateICmpNE(expressionNode->generate(true), Builder.getInt32(0), "whileLoopcond");
		Builder.CreateCondBr(NextVal, LoopBB, AfterBB);
		Builder.SetInsertPoint(LoopBB);
		count = Builder.CreateAdd(count, Builder.getInt32(1));
		statementList->generate();
		NextVal = Builder.CreateICmpNE(expressionNode->generate(true), Builder.getInt32(0), "whileloopcond");
		Builder.CreateCondBr(NextVal, LoopBB, AfterBB);
		Builder.SetInsertPoint(AfterBB);
		return count;
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
llvm::Value* for_statement_node::generate(bool loadVariable){
		// std::cout << "This is generator function in for_statement_node" << std::endl;
		llvm::BasicBlock *PreheaderBB = Builder.GetInsertBlock();
		llvm::BasicBlock *LoopBB = createBB(mainFunc, "forLoopBlock");
		llvm::Value *count = Builder.getInt32(0);
		llvm::Value *StartVal = initNode->generate(true);
		Builder.CreateBr(LoopBB);
		Builder.SetInsertPoint(LoopBB);
		// Comment out the line having PHINode variable IndVar, as it is useless
		llvm::PHINode *IndVar = Builder.CreatePHI(llvm::Type::getInt32Ty(Context), 2, "i");
		IndVar->addIncoming(StartVal, PreheaderBB);
		count = Builder.CreateAdd(count, Builder.getInt32(1));
		statementList->generate();
		llvm::Value *NextVal = modfNode->generate(true);
		llvm::Value *EndCond = Builder.CreateICmpNE(termNode->generate(), Builder.getInt32(0), "forloopcond");
		llvm::BasicBlock *LoopEndBB = Builder.GetInsertBlock();
		llvm::BasicBlock *AfterBB = createBB(mainFunc, "entry");
		Builder.CreateCondBr(EndCond, LoopBB, AfterBB);
		Builder.SetInsertPoint(AfterBB);
		IndVar->addIncoming(NextVal, LoopEndBB);
		return count;
}


declaration_statement_node::declaration_statement_node(std::string data_type, std::string id, expression_node *expr_node, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), variableID(id), dataType(data_type), firstD(0), secondD(0), expressionNode(expr_node) {
	array_size[variableID] = std::make_pair(0, 0);
}
declaration_statement_node::declaration_statement_node(std::string data_type, std::string id, int N1, expression_node *expr_node, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), variableID(id), dataType(data_type), firstD(N1), secondD(0), expressionNode(expr_node) {
	array_size[variableID] = std::make_pair(N1, 0);
	array1_table[variableID].resize(N1);
	for(int i = 0; i < N1; i++){
		array1_table[variableID][i] = expressionNode->evaluate();
	}
}
declaration_statement_node::declaration_statement_node(std::string data_type, std::string id, int N1, int N2, expression_node *expr_node, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), variableID(id), dataType(data_type), firstD(N1), secondD(N2), expressionNode(expr_node) {
	array_size[variableID] = std::make_pair(firstD, secondD);
	array2_table[variableID].resize(firstD);
	for(int i = 0; i < firstD; i++){
		array2_table[variableID][i].resize(secondD);
		for(int j = 0; j < secondD; j++){
			array2_table[variableID][i][j] = expressionNode->evaluate();
		}
	}
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
llvm::Value* declaration_statement_node::generate(bool loadVariable){
		// std::cout << "This is generator function in declaration_statement_node" << std::endl;
		if(firstD && secondD){
			// 2D Array
			if(!variable_table[variableID]){
				variable_table[variableID] = createGlob(Builder, variableID, firstD, secondD);
				// llvm::ConstantInt* const_int_val = llvm::ConstantInt::get(ModuleOb->getContext(), llvm::APInt(32,0));
				// variable_table[variableID]->setInitializer(const_int_val);
				// symbol_table[variableID] = variable_table[variableID]->getInitializer();
			}
			symbol_table[variableID] = Builder.CreateStore(expressionNode->generate(), variable_table[variableID]);
			return symbol_table[variableID];
		}
		else if(firstD){
			// 1D Array
			if(!variable_table[variableID]){
				variable_table[variableID] = createGlob(Builder, variableID, firstD);
				// llvm::ConstantInt* const_int_val = llvm::ConstantInt::get(ModuleOb->getContext(), llvm::APInt(32,0));
				// variable_table[variableID]->setInitializer(const_int_val);
				// symbol_table[variableID] = variable_table[variableID]->getInitializer();
			}
			symbol_table[variableID] = Builder.CreateStore(expressionNode->generate(), variable_table[variableID]);
			return symbol_table[variableID];
		}
		else{
			if(!variable_table[variableID]){
				variable_table[variableID] = createGlob(Builder, variableID);
				// llvm::ConstantInt* const_int_val = llvm::ConstantInt::get(ModuleOb->getContext(), llvm::APInt(32,0));
				// variable_table[variableID]->setInitializer(const_int_val);
				// symbol_table[variableID] = variable_table[variableID]->getInitializer();
			}
			symbol_table[variableID] = Builder.CreateStore(expressionNode->generate(), variable_table[variableID]);
			return symbol_table[variableID];
		}
}


assignment_statement_node::assignment_statement_node(std::string id, expression_node *expr_node, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), variableID(id), expressionNode(expr_node) {
	dimNum = 0;
	// firstI = secondI = nullptr;
}
assignment_statement_node::assignment_statement_node(std::string id, expression_node *first_index, expression_node *expr_node, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), variableID(id), firstI(first_index), expressionNode(expr_node) {
	dimNum = 1;
	// secondI = nullptr;
}
assignment_statement_node::assignment_statement_node(std::string id, expression_node *first_index, expression_node *second_index, expression_node *expr_node, bool print, std::string term, std::string init) : initiatorChar(init), terminatorChar(term), printStatement(print), variableID(id), firstI(first_index), secondI(second_index), expressionNode(expr_node) {
	dimNum = 2;
}
void assignment_statement_node::print(){
	std::cout << initiatorChar;
	switch (dimNum) {
		case 2:{
			std::cout << "<# " << variableID << "["; firstI->print(); std::cout << "]["; secondI->print(); std::cout << "] = "; expressionNode->print(); std::cout << " #>";
			break;
		}
		case 1:{
			std::cout << "<# " << variableID << "["; firstI->print(); std::cout << "] = "; expressionNode->print(); std::cout << " #>";
			break;
		}
		default:{
			std::cout << "<# " << variableID << " = "; expressionNode->print(); std::cout << " #>";
		}
	}
	std::cout << terminatorChar;
	return;
}
void assignment_statement_node::evaluate(){
	switch (dimNum) {
		case 2:{
			array2_table[variableID][firstI->evaluate().dataValue.valueInteger][secondI->evaluate().dataValue.valueInteger] = expressionNode->evaluate();
			break;
		}
		case 1:{
			array1_table[variableID][firstI->evaluate().dataValue.valueInteger] = expressionNode->evaluate();
			break;
		}
		default:{
			value_table[variableID] = expressionNode->evaluate();
		}
	}
	if(printStatement){
		std::cout << initiatorChar;
		switch (dimNum) {
			case 2:{
				std::cout << "\t<$ assignment_statement_node: " << variableID << "["; firstI->evaluate().print(); std::cout << "]["; secondI->evaluate().print(); std::cout << "]" << " = "; array2_table[variableID][firstI->evaluate().dataValue.valueInteger][secondI->evaluate().dataValue.valueInteger].print(); std::cout << " $>" << std::endl;
				break;
			}
			case 1:{
				std::cout << "\t<$ assignment_statement_node: " << variableID << "["; firstI->evaluate().print(); std::cout << "]" << " = "; array1_table[variableID][firstI->evaluate().dataValue.valueInteger].print(); std::cout << " $>" << std::endl;
				break;
			}
			default:{
				std::cout << "\t<$ assignment_statement_node: " << variableID << " = "; value_table[variableID].print(); std::cout << " $>" << std::endl;
			}
		}
	}
	return;
}
void assignment_statement_node::print_evaluate(){
	std::cout << initiatorChar;
	switch (dimNum) {
		case 2:{
			std::cout << "<# " << variableID << "["; firstI->print(); std::cout << "]["; secondI->print(); std::cout << "] = "; expressionNode->print(); std::cout << " #>";
			break;
		}
		case 1:{
			std::cout << "<# " << variableID << "["; firstI->print(); std::cout << "] = "; expressionNode->print(); std::cout << " #>";
			break;
		}
		default:{
			std::cout << "<# " << variableID << " = "; expressionNode->print(); std::cout << " #>";
		}
	}
	std::cout << terminatorChar;
	switch (dimNum) {
		case 2:{
			array2_table[variableID][firstI->evaluate().dataValue.valueInteger][secondI->evaluate().dataValue.valueInteger] = expressionNode->evaluate();
			break;
		}
		case 1:{
			array1_table[variableID][firstI->evaluate().dataValue.valueInteger] = expressionNode->evaluate();
			break;
		}
		default:{
			value_table[variableID] = expressionNode->evaluate();
		}
	}
	if(printStatement){
		std::cout << initiatorChar;
		switch (dimNum) {
			case 2:{
				std::cout << "\t<$ assignment_statement_node: " << variableID << "["; firstI->evaluate().print(); std::cout << "]["; secondI->evaluate().print(); std::cout << "]" << " = "; array2_table[variableID][firstI->evaluate().dataValue.valueInteger][secondI->evaluate().dataValue.valueInteger].print(); std::cout << " $>" << std::endl;
				break;
			}
			case 1:{
				std::cout << "\t<$ assignment_statement_node: " << variableID << "["; firstI->evaluate().print(); std::cout << "]" << " = "; array1_table[variableID][firstI->evaluate().dataValue.valueInteger].print(); std::cout << " $>" << std::endl;
				break;
			}
			default:{
				std::cout << "\t<$ assignment_statement_node: " << variableID << " = "; value_table[variableID].print(); std::cout << " $>" << std::endl;
			}
		}
	}
	return;
}
llvm::Value* assignment_statement_node::generate(bool loadVariable){
		// std::cout << "This is generator function in assignment_statement_node" << std::endl;
		switch (dimNum) {
			case 2:{
				// 2D Array
				if(!variable_table[variableID]){
					variable_table[variableID] = createGlob(Builder, variableID, array_size[variableID].first, array_size[variableID].second);
					// llvm::ConstantInt* const_int_val = llvm::ConstantInt::get(ModuleOb->getContext(), llvm::APInt(32,0));
					// variable_table[variableID]->setInitializer(const_int_val);
					// symbol_table[variableID] = variable_table[variableID]->getInitializer();
				}
				symbol_table[variableID] = Builder.CreateStore(expressionNode->generate(), Builder.CreateGEP(variable_table[variableID], Builder.getInt32(firstI->evaluate().dataValue.valueInteger * array_size[variableID].first + secondI->evaluate().dataValue.valueInteger), "IDX"));
				return symbol_table[variableID];
			}
			case 1:{
				// 1D Array
				if(!variable_table[variableID]){
					variable_table[variableID] = createGlob(Builder, variableID, array_size[variableID].first);
					// llvm::ConstantInt* const_int_val = llvm::ConstantInt::get(ModuleOb->getContext(), llvm::APInt(32,0));
					// variable_table[variableID]->setInitializer(const_int_val);
					// symbol_table[variableID] = variable_table[variableID]->getInitializer();
				}
				symbol_table[variableID] = Builder.CreateStore(expressionNode->generate(), Builder.CreateGEP(variable_table[variableID], Builder.getInt32(firstI->evaluate().dataValue.valueInteger), "IDX"));
				return symbol_table[variableID];
			}
			default:{
				if(!variable_table[variableID]){
					variable_table[variableID] = createGlob(Builder, variableID);
					// llvm::ConstantInt* const_int_val = llvm::ConstantInt::get(ModuleOb->getContext(), llvm::APInt(32,0));
					// variable_table[variableID]->setInitializer(const_int_val);
					// symbol_table[variableID] = variable_table[variableID]->getInitializer();
				}
				symbol_table[variableID] = Builder.CreateStore(expressionNode->generate(), variable_table[variableID]);
				if(loadVariable){
					symbol_table[variableID] = Builder.CreateLoad(variable_table[variableID]);
				}
				return symbol_table[variableID];
			}
		}
}


program_node::program_node(statement_list_node *stmt_list, bool print, std::string term, std::string init): initiatorChar(init), terminatorChar(term), printStatement(print), statementList(stmt_list) {
}
void program_node::evaluate(){
	if(printStatement){
		std::cout << "===================================================PRINT========================================================================" << std::endl;
		statementList->print();
		std::cout << "================================================EVALUATE========================================================================" << std::endl;
		statementList->evaluate();
		std::cout << "================================================PRINT+EVALUATE==================================================================" << std::endl;
		statementList->print_evaluate();
		std::cout << "================================================================================================================================" << std::endl;
	}
	return;
}
void program_node::generate(bool loadVariable){
		std::cout << "================================================GENERATE========================================================================" << std::endl;
		// std::cout << "This is generator function in program_node" << std::endl;
		// FuncArgs.push_back("a");
		// FuncArgs.push_back("b");
		// FuncArgs.push_back("c");
		mainFunc = createFunc(Builder, "main");
		setFuncArgs(mainFunc, FuncArgs);
		llvm::BasicBlock *entry = createBB(mainFunc, "program_node");
		Builder.SetInsertPoint(entry);
		statementList->generate();
		Builder.CreateRet(Builder.getInt32(0));
		verifyFunction(*mainFunc);
		// llvm::Module::dump() const is depricated, so use the below print functions
		// ModuleOb->dump();
		ModuleOb->print(llvm::outs(), nullptr);
		std::cout << "================================================================================================================================" << std::endl;
		return;
}
