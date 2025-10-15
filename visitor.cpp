#include <iostream>
#include <fstream>
#include <cmath>
#include "ast.h"
#include "visitor.h"
#include "environment.h"

using namespace std;

Environment memoria;

///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int SqrtExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}


int PrintStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IfStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int WhileStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Body::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int VarDec::accept(Visitor* visitor) {
    return visitor->visit(this);
}

///////////////////////////////////////////////////////////////////////////////////

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(SqrtExp* exp) {
    cout << "sqrt(";
    exp->value->accept(this);
    cout <<  ")";
    return 0;
}


void PrintVisitor::imprimir(Program* programa){
    if (programa)
    {
        cout << "Codigo:" << endl; 
        programa->accept(this);
        cout << endl;
    }
    return ;
}

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp* exp) {
    int result;
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    switch (exp->op) {
        case LE_OP:
            result = v1 < v2;
            break;
        case PLUS_OP:
            result = v1 + v2;
            break;
        case MINUS_OP:
            result = v1 - v2;
            break;
        case MUL_OP:
            result = v1 * v2;
            break;
        case DIV_OP:
            if (v2 != 0)
                result = v1 / v2;
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        case POW_OP:
            result = pow(v1,v2);
            break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

int EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}

int EVALVisitor::visit(SqrtExp* exp) {
    return floor(sqrt( exp->value->accept(this)));
}


void EVALVisitor::interprete(Program* programa){
    if (programa)
    {
        cout << "Interprete:" << endl;
        programa->accept(this);
    }
    return;

}

//////////////////////////////////////////

int PrintVisitor::visit(Program* p) {
    p->cuerpo->accept(this);
    return 0;
}

int PrintVisitor::visit(PrintStm* stm) {
    cout << "print(";
    stm->e->accept(this);
    cout << ")" << endl;
    return 0;
}

int PrintVisitor::visit(AssignStm* stm) {
    cout << stm->id << "=";
    stm->e->accept(this);
    cout << endl;
    return 0;
}

int PrintVisitor::visit(IdExp* p) {
    cout << p->value;
    return 0;
}

int EVALVisitor::visit(Program* p) {
    memoria.add_level();
    p->cuerpo->accept(this);
    memoria.remove_level();
    return 0;
}

int EVALVisitor::visit(VarDec* v) {
    for (auto i : v->variables) {
        memoria.add_var(i, v->tipo);
    }
    return 0;
}

int EVALVisitor::visit(Body* b) {
    for (auto i : b->dcList) {
        i->accept(this);
    }
    for (auto i : b->stmList) {
        i->accept(this);
    }
    return 0;
}

int EVALVisitor::visit(PrintStm* p) {
    cout << p->e->accept(this) << endl; 
    return 0;
}

int EVALVisitor::visit(AssignStm* p) {
    memoria.update(p->id, p->e->accept(this));
    return 0;
}

int EVALVisitor::visit(IdExp* p) {
    return memoria.lookup(p->value);
}



int EVALVisitor::visit(IfStm* stm) {
    if (stm->condicion->accept(this)){
        for(auto i:stm->caso1->dcList){
            i->accept(this);
        }
        for(auto i:stm->caso1->stmList){
            i->accept(this);
        }
    }
    else{
        for(auto i:stm->caso2->dcList){
            i->accept(this);
        }
        for(auto i:stm->caso2->stmList){
            i->accept(this);
        } 
    }
    return 0;
}

int EVALVisitor::visit(WhileStm* stm) {
    while(stm->condicion->accept(this)){
        for(auto i:stm->caso1->dcList){
            i->accept(this);
        }
        for(auto i:stm->caso1->stmList){
            i->accept(this);
        }
    }
    return 0;
}

int PrintVisitor::visit(VarDec* v) {
    cout << "var " << v->tipo << " ";
    bool primero = true;
    for (auto var : v->variables) {
        if (!primero) {
            cout << ", ";
        }
        cout << var;
        primero = false;
    }
    cout << ";" << endl;
    return 0;
}

int PrintVisitor::visit(Body* b) {
    for (auto i : b->dcList) {
        i->accept(this);
    }
    for (auto i : b->stmList) {
        i->accept(this);
    }
    return 0;
}

int PrintVisitor::visit(IfStm* stm) {
    cout << "if " ;
    stm->condicion->accept(this);
    cout  << " then" << endl;
    for (auto i:stm->caso1->dcList){
        cout << "\t";
        i->accept(this);
    }
    for (auto i:stm->caso1->stmList){
        cout << "\t";
        i->accept(this);
    }
    if (stm->parteelse){
        cout << "else"  << endl;;
        for (auto i:stm->caso2->dcList){
            cout << "\t";
            i->accept(this);
        }
        for (auto i:stm->caso2->stmList){
            cout << "\t";
            i->accept(this);
        }
    }   
    cout << "endif" << endl;
    return 0;
}

int PrintVisitor::visit(WhileStm* stm) {
    cout << "while " ;
    stm->condicion->accept(this);
    cout  << " do" << endl;
    for (auto i:stm->caso1->dcList){
        cout << "\t";
        i->accept(this);
    }
    for (auto i:stm->caso1->stmList){
        cout << "\t";
        i->accept(this);
    }
    cout << "endwhile" << endl;
    return 0;
}