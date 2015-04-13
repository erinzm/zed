#include <string>
#include <vector>

class ExprAST {
public:
	virtual ~ExprAST();
};

class NumExprAST : public ExprAST {
	double _val;
public:
	NumExprAST(double v);
};

class VariableExprAST : public ExprAST {
	std::string _name;
public:
	VariableExprAST(std::string n);
};

class BinaryExprAST : public ExprAST {
	char _op;
	ExprAST *_LHS, *_RHS;
public:
	BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs);
};

class CallExprAST : public ExprAST {
	std::string _callee;
	std::vector<ExprAST*> _args;
public:
	CallExprAST(const std::string &callee, std::vector<ExprAST*> &args);
};

class PrototypeAST {
  std::string _name;
  std::vector<std::string> _args;
public:
  PrototypeAST(const std::string &name, const std::vector<std::string> &args);
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
public:
  FunctionAST(PrototypeAST *proto, ExprAST *body);
};