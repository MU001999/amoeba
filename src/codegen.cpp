#include <tuple>
#include <memory>
#include "ast.hpp"
#include "code.hpp"

using namespace std;

namespace ice_language
{
AST::~AST() = default;
Stmt::~Stmt() = default;
Expr::~Expr() = default;

void BlockExpr::codegen(Code &code)
{
    for (auto statement : statements)
    {
        statement->codegen(code);
    }
}

void NoneExpr::codegen(Code &code)
{
    code.add_ins<Op::Push>(nullptr);
}

void IntegerExpr::codegen(Code &code)
{
    code.add_ins<Op::Push>(value);
}

void FloatExpr::codegen(Code &code)
{
    code.add_ins<Op::Push>(value);
}

void BoolExpr::codegen(Code &code)
{
    code.add_ins<Op::Push>(value);
}

void StringExpr::codegen(Code &code)
{
    code.add_ins<Op::Push>(value);
}

void IdentifierExpr::codegen(Code &code)
{
    code.add_ins<Op::Load>(name);
}

void ParenOperatorExpr::codegen(Code &code)
{
    for (auto arg : args)
    {
        arg->codegen(code);
    }
    expr->codegen(code);
    code.add_ins<Op::Call>(args.size());
}

void UnaryOperatorExpr::codegen(Code &code)
{
    switch (op)
    {
    case TokenId::Sub:
        code.add_ins<Op::Neg>();
        break;

    default:
        break;
    }
}

void BinaryOperatorExpr::codegen(Code &code)
{
    switch (op)
    {
    case TokenId::Add:
        code.add_ins<Op::Add>();
        break;

    case TokenId::Sub:
        code.add_ins<Op::Sub>();
        break;

    default:
        break;
    }
}

void LambdaExpr::codegen(Code &code)
{

}

// [AFTER] [CLASS]
void NewExpr::codegen(Code &code)
{

}

// [AFTER] [CLASS]
void DotExpr::codegen(Code &code)
{

}

// [AFTER] [CLASS]
void EnumExpr::codegen(Code &code)
{

}

void MatchExpr::codegen(Code &code)
{

}

// [AFTER] [CLASS]
void ListExpr::codegen(Code &code)
{

}

// [AFTER] [CLASS]
void IndexExpr::codegen(Code &code)
{

}

// [AFTER] [CLASS]
void DictExpr::codegen(Code &code)
{

}

void UsingStmt::codegen(Code &code)
{

}

void ExprStmt::codegen(Code &code)
{
    expr->codegen(code);
    code.add_ins<Op::Pop>();
}

void VariableDeclarationStmt::codegen(Code &code)
{
    expr->codegen(code);
    code.add_ins<Op::Create>(id->name);
    code.add_ins<Op::Load>(id->name);
    code.add_ins<Op::Store>();
}

void VariableAssignStmt::codegen(Code &code)
{
    expr->codegen(code);
    code.add_ins<Op::Load>(id->name);
    code.add_ins<Op::Store>();
}

void NonVariableAssignStmt::codegen(Code &code)
{
    expr->codegen(code);
    left->codegen(code);
    code.add_ins<Op::Store>();
}

void FunctionDeclarationStmt::codegen(Code &code)
{
    // code will be gen in code
    // but there should be a function object after
    // the code's running
}

void ClassDeclarationStmt::codegen(Code &code)
{

}

void BreakStmt::codegen(Code &code)
{
    code.push_break(code.add_ins());
}

void ContinueStmt::codegen(Code &code)
{
    code.push_continue(code.add_ins());
}

void ReturnStmt::codegen(Code &code)
{
    expr->codegen(code);
    code.add_ins<Op::Return>();
}

void IfElseStmt::codegen(Code &code)
{
    cond->codegen(code);
    auto o1 = code.add_ins();
    block_true->codegen(code);
    code.set_ins<Op::JumpIfNot>(o1, code.size());
    auto o2 = code.add_ins();
    else_stmt->codegen(code);
    code.set_ins<Op::Jump>(o2, code.size());
}

void WhileStmt::codegen(Code &code)
{
    cond->codegen(code);
    auto o1 = code.add_ins();
    block->codegen(code);
    code.add_ins<Op::Jump>(o1);
    code.set_ins<Op::JumpIfNot>(o1, code.size());
    code.set_break_to(code.size());
    code.set_continue_to(o1);
}

void DoWhileStmt::codegen(Code &code)
{

}

void ForStmt::codegen(Code &code)
{

}

void ForeachStmt::codegen(Code &code)
{

}
}
