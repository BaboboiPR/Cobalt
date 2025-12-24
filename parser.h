#pragma once
#include <string>
#include <fstream>
enum NodeType {
    NODE_DECLARATION,
    NODE_VARIABLE,
    NODE_ASSIGNMENT,
    NODE_VALUE,
    NODE_EXPRESSION,
    NODE_OPERATOR
};

struct ASTNode {
    NodeType type;
    std::string name;   // variable name, operator symbol
    std::string value;  // literal value
    ASTNode* children[10]; // pointers to child nodes
    int child_count = 0;
};
bool is_number(const std::string& s) {
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return !s.empty();
}
void printAST(ASTNode* node, int indent = 0) {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Type: " << node->type
              << " Name: " << node->name
              << " Value: " << node->value << std::endl;
    for (int i = 0; i < node->child_count; i++)
        printAST(node->children[i], indent + 1);
}

struct word {
    enum Type {
        INIT,VARIABLE,ASSIGN,VALUE,EXPRESSION,OPERATOR,COMMA,
    } type;
    std::string name;
    std::string value;

};

int pos_1 = 0;
word Pars[10000];
std::string unknown;
void generateCpp(ASTNode* node, std::ofstream& out) {
    if (!node) return;
    if (node->type == NODE_DECLARATION) {
        auto var = node->children[0];
        out << var->name;
        if (node->child_count > 1) {
            auto val = node->children[1];
            if (var->value!="") out << " = " << val->value;
        }
        out << ";" << std::endl;
    }

    for (int i = 0; i < node->child_count; i++)
        generateCpp(node->children[i], out);
}
void parser(std::fstream& file) {
while (file>>unknown) {
    if (unknown == "int") {
        Pars[pos_1].type =word::INIT;
        Pars[pos_1].name = "int";
        Pars[pos_1].value = "";
    }
    else if (unknown == "float") {
        Pars[pos_1].type =word::INIT;
        Pars[pos_1].name = "float";
        Pars[pos_1].value = "";
    }
    else if (unknown == "bool") {
        Pars[pos_1].type =word::INIT;
        Pars[pos_1].name = "bool";
        Pars[pos_1].value = "";
    }
    else if (unknown == "string") {
        Pars[pos_1].type =word::INIT;
        Pars[pos_1].name = "string";
        Pars[pos_1].value = "";
    }
    else if (unknown == "<-") {


            Pars[pos_1].type =word::ASSIGN;
            Pars[pos_1].name = "=";
            Pars[pos_1].value = "";
        }
    else if (unknown =="="){
            Pars[pos_1].type =word::EXPRESSION;
            Pars[pos_1].name = "==";
            Pars[pos_1].value = "";

    }
    else if (is_number(unknown)) {
        Pars[pos_1].type =word::VALUE;
        Pars[pos_1].name = unknown;
        Pars[pos_1].value = unknown;
    }
    else if (unknown != "") {
        Pars[pos_1].type =word::VARIABLE;
        Pars[pos_1].name = unknown;
    }

    pos_1++;
}
    for (long long i = 0; i < pos_1; i++) {
        std::cout << Pars[i].type<<" ";
        std::cout << Pars[i].name;
        std::cout<<std::endl;
    }
}

ASTNode* buildAST() {
    ASTNode* program = new ASTNode; // dummy root for all statements
    program->type = NODE_DECLARATION; // optional, just a container

    for (int f = 0; f < pos_1; ) {
        if (Pars[f].type == word::INIT) {
            ASTNode* root = new ASTNode;
            root->type = NODE_DECLARATION;

            ASTNode* var = new ASTNode;
            var->type = NODE_VARIABLE;
            var->name = Pars[f+1].name;
            root->children[root->child_count++] = var;

            if (Pars[f+2].type == word::ASSIGN || Pars[f+2].type == word::EXPRESSION) {
                ASTNode* val = new ASTNode;
                val->type = (Pars[f+3].type == word::VALUE) ? NODE_VALUE : NODE_VARIABLE;
                val->value = Pars[f+3].name;
                root->children[root->child_count++] = val;
                f += 4; // move past this statement
            } else {
                f += 2; // just declaration without assignment
            }

            // attach statement to program
            program->children[program->child_count++] = root;
        } else {
            f++; // skip unknown token
        }
    }
    printAST(program);
    std::ofstream file_co("co.cpp");
    generateCpp(program,file_co);
    file_co.close();
    return program;
}
