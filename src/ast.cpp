#include "ast.h"

ExprAST::~ExprAST() {}

NumExprAST::NumExprAST(double val)
	: _val(val) {}

VariableExprAST::VariableExprAST(std::string name) :
	_name(name) {}

BinaryExprAST::BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs)
	: _op(op), _LHS(lhs), _RHS(rhs) {}

CallExprAST::CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
	: _callee(callee), _args(args) {}

PrototypeAST::PrototypeAST(const std::string &name, const std::vector<std::string> &args)
	: _name(name), _args(args) {}

FunctionAST::FunctionAST(PrototypeAST *proto, ExprAST *body) {}