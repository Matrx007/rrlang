#pragma once

#include <bits/stdint-uintn.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>

#include "ast.cpp"
#include "parsing.cpp"
#include "keywords.cpp"

Node* parse(std::string line);

void printNode(Node *node, std::string indent) {
    switch (node->type) {
        case Node::Define:
            printf("%stype: \n", indent.c_str());
            printNode(node->nodeDefine.type, indent + "  ");

            printf("%sname: %s\n", indent.c_str(), node->nodeDefine.name.c_str());

            printf("%svalue: \n", indent.c_str());
            printNode(node->nodeDefine.value, indent + "  ");
            break;
        case Node::Assign:

            printf("%svariable: \n", indent.c_str());
            printNode(node->nodeAssign.variable, indent + "  ");

            printf("%svalue: \n", indent.c_str());
            printNode(node->nodeAssign.value, indent + "  ");

            break;
        case Node::Delete:

            printf("%svariable: \n", indent.c_str());
            printNode(node->nodeDelete.variable, indent + "  ");

            break;
        case Node::Variable:

            printf("%spointer level: %d\n", indent.c_str(), node->nodeVariable.pointerLevel);

            printf("%sname: %s\n", indent.c_str(), node->nodeVariable.name.c_str());

            printf("%stype: \n", indent.c_str());
            printNode(node->nodeVariable.type, indent + "  ");
            
            break;
        case Node::BinaryOperation:

            printf("%svariable: \n", indent.c_str());
            printNode(node->nodeBinaryOperation.variable, indent + "  ");

            switch (node->nodeBinaryOperation.operation) {
                case Op_Add:
                    printf("%stype: add\n", indent.c_str());
                    break;
                case Op_Subtract:
                    printf("%stype: subtract\n", indent.c_str());
                    break;
                case Op_Multiply:
                    printf("%stype: multiply\n", indent.c_str());
                    break;
                case Op_Divide:
                    printf("%stype: divide\n", indent.c_str());
                    break;
                case Op_Modulo:
                    printf("%stype: modulo\n", indent.c_str());
                    break;
                case Op_And:
                    printf("%stype: and\n", indent.c_str());
                    break;
                case Op_Or:
                    printf("%stype: or\n", indent.c_str());
                    break;
                case Op_Xor:
                    printf("%stype: xor\n", indent.c_str());
                    break;
                default:
                    printf("%stype: <unknown>\n", indent.c_str());
                    break;
            }

            printf("%svalue: \n", indent.c_str());
            printNode(node->nodeBinaryOperation.value, indent + "  ");

            break;
        case Node::Type:

            printf("%spointer level: %d\n", indent.c_str(), node->nodeType.pointerLevel);


            switch (node->nodeType.type) {
                case Dt_Struct:
                    printf("%stype: struct\n", indent.c_str());

                    printf("%sstruct type: \n", indent.c_str());
                    printNode(node->nodeType.structType, indent + "  ");
                    break;
                case Dt_Enum:
                    printf("%stype: enum\n", indent.c_str());

                    printf("%senum type: \n", indent.c_str());
                    printNode(node->nodeType.enumType, indent + "  ");
                    break;
                case Dt_u8:
                    printf("%stype: u8\n", indent.c_str());
                    break;
                case Dt_i8:
                    printf("%stype: i8\n", indent.c_str());
                    break;
                case Dt_u16:
                    printf("%stype: u16\n", indent.c_str());
                    break;
                case Dt_i16:
                    printf("%stype: i16\n", indent.c_str());
                    break;
                case Dt_u32:
                    printf("%stype: u32\n", indent.c_str());
                    break;
                case Dt_i32:
                    printf("%stype: i32\n", indent.c_str());
                    break;
                case Dt_f32:
                    printf("%stype: f32\n", indent.c_str());
                    break;
                default:
                    printf("%stype: <unknown>\n", indent.c_str());
                    break;
            }

            break;
        case Node::Constant:

            switch (node->nodeConstant.type) {
                case Dt_Struct:
                    printf("%stype: struct\n", indent.c_str());

                    printf("%sstruct members: \n", indent.c_str());

                    for(std::vector<Node*>::iterator it = std::begin(node->nodeConstant.structElements); 
                            it != std::end(node->nodeConstant.structElements); ++it) {

                        printNode(*it, indent + "  ");
                    }
                    break;
                case Dt_Enum:
                    printf("%stype: enum\n", indent.c_str());

                    printf("%senum value: %s\n", indent.c_str(), node->nodeConstant.value_enum.c_str());
                    break;
                case Dt_u8:
                    printf("%stype: u8\n", indent.c_str());

                    printf("%svalue: %ud\n", indent.c_str(), node->nodeConstant.value_u8);
                    break;
                case Dt_i8:
                    printf("%stype: i8\n", indent.c_str());

                    printf("%svalue: %d\n", indent.c_str(), node->nodeConstant.value_i8);
                    break;
                case Dt_u16:
                    printf("%stype: u16\n", indent.c_str());

                    printf("%svalue: %ud\n", indent.c_str(), node->nodeConstant.value_u16);
                    break;
                case Dt_i16:
                    printf("%stype: i16\n", indent.c_str());

                    printf("%svalue: %d\n", indent.c_str(), node->nodeConstant.value_i16);
                    break;
                case Dt_u32:
                    printf("%stype: u32\n", indent.c_str());

                    printf("%svalue: %ud\n", indent.c_str(), node->nodeConstant.value_u32);
                    break;
                case Dt_i32:
                    printf("%stype: i32\n", indent.c_str());

                    printf("%svalue: %d\n", indent.c_str(), node->nodeConstant.value_i32);
                    break;
                case Dt_f32:
                    printf("%stype: f32\n", indent.c_str());

                    printf("%svalue: %f\n", indent.c_str(), node->nodeConstant.value_f32);
                    break;
                default:
                    printf("%stype: <unknown>\n", indent.c_str());
                    break;
            }

            break;
        case Node::Call:
            printf("%sfunction: \n", indent.c_str());
            printNode(node->nodeCall.function, indent + "  ");

            printf("%sargs: \n", indent.c_str());
            for(std::vector<Node*>::iterator it = std::begin(node->nodeCall.args); 
                    it != std::end(node->nodeCall.args); ++it) {

                printNode(*it, indent + "  ");
            }
            break;
        case Node::Negate:
            printf("%snegate: \n", indent.c_str());
            printNode(node->nodeNegate.negate, indent + "  ");
            break;
        case Node::BinaryExpression:
            printf("%sleft: \n", indent.c_str());
            printNode(node->nodeBinaryExpression.left, indent + "  ");

            switch (node->nodeBinaryExpression.operation) {
                case Op_Add:
                    printf("%sop: add\n", indent.c_str());
                    break;
                case Op_Subtract:
                    printf("%sop: subtract\n", indent.c_str());
                    break;
                case Op_Multiply:
                    printf("%sop: multiply\n", indent.c_str());
                    break;
                case Op_Divide:
                    printf("%sop: divide\n", indent.c_str());
                    break;
                case Op_Modulo:
                    printf("%sop: modulo\n", indent.c_str());
                    break;
                case Op_And:
                    printf("%sop: and\n", indent.c_str());
                    break;
                case Op_Or:
                    printf("%sop: or\n", indent.c_str());
                    break;
                case Op_Xor:
                    printf("%sop: xor\n", indent.c_str());
                    break;
                default:
                    printf("%sop: <unknown>\n", indent.c_str());
                    break;
            }

            printf("%sright: \n", indent.c_str());
            printNode(node->nodeBinaryExpression.right, indent + "  ");

            break;
        case Node::Comparison:
            printf("%sleft: \n", indent.c_str());
            printNode(node->nodeComparison.left, indent + "  ");

            switch (node->nodeComparison.operation) {

                case Comp_Equal:
                    printf("%sop: Equal\n", indent.c_str());
                    break;
                case Comp_NotEqual:
                    printf("%sop: NotEqual\n", indent.c_str());
                    break;
                case Comp_Larger:
                    printf("%sop: Larger\n", indent.c_str());
                    break;
                case Comp_Smaller:
                    printf("%sop: Smaller\n", indent.c_str());
                    break;
                case Comp_LargerEqual:
                    printf("%sop: LargerEqual\n", indent.c_str());
                    break;
                case Comp_SmallerEqual:
                    printf("%sop: SmallerEqual\n", indent.c_str());
                    break;
                case Comp_And:
                    printf("%sop: And\n", indent.c_str());
                    break;
                case Comp_Or:
                    printf("%sop: Or\n", indent.c_str());
                    break;
                case Comp_Xor:
                    printf("%sop: Xor\n", indent.c_str());
                    break;
                default:
                    printf("%sop: <unknown>\n", indent.c_str());
                    break;
            }

            printf("%sright: \n", indent.c_str());
            printNode(node->nodeComparison.right, indent + "  ");

            break;
        case Node::FunctionDefinition:
            printf("%sname: %s\n", indent.c_str(), node->nodeFunctionDefinition.name.c_str());

            printf("%sreturns: \n", indent.c_str());
            printNode(node->nodeFunctionDefinition.returnType, indent + "  ");

            printf("%sargs: \n", indent.c_str());
            for(std::vector<Node*>::iterator it = std::begin(node->nodeFunctionDefinition.args); 
                    it != std::end(node->nodeFunctionDefinition.args); ++it) {

                printNode(*it, indent + "  ");
            }

            break;
        case Node::StaticVariableDefinition:
            printf("%stype: \n", indent.c_str());
            printNode(node->nodeStaticVariableDefinition.type, indent + "  ");

            break;
        case Node::StructDefinition:
            printf("%smembers: \n", indent.c_str());
            for(std::vector<Node*>::iterator it = std::begin(node->nodeStructDefinition.members); 
                    it != std::end(node->nodeStructDefinition.members); ++it) {

                printNode(*it, indent + "  ");
            }
            
            break;
        case Node::EnumDefinition:
            printf("%senum types: \n", indent.c_str());
            for(std::vector<std::string>::iterator it = std::begin(node->nodeEnumDefinition.enumList); 
                    it != std::end(node->nodeEnumDefinition.enumList); ++it) {

                printf("%s  %s\n", indent.c_str(), (*it).c_str());
            }

            break;
        case Node::Return:
            printf("%sreturn: \n", indent.c_str());
            printNode(node->nodeReturn.value, indent + "  ");

            break;
        case Node::For:
            printf("%sinitialization: \n", indent.c_str());
            printNode(node->nodeFor.initialization, indent + "  ");

            printf("%scondition: \n", indent.c_str());
            printNode(node->nodeFor.condition, indent + "  ");

            printf("%supdate: \n", indent.c_str());
            printNode(node->nodeFor.update, indent + "  ");

            printf("%sbody: \n", indent.c_str());
            for(std::vector<Node*>::iterator it = std::begin(node->nodeFor.body); 
                    it != std::end(node->nodeFor.body); ++it) {

                printNode(*it, indent + "  ");
            }

            break;
        case Node::While:
            printf("%scondition: \n", indent.c_str());
            printNode(node->nodeWhile.condition, indent + "  ");

            printf("%sbody: \n", indent.c_str());
            for(std::vector<Node*>::iterator it = std::begin(node->nodeWhile.body); 
                    it != std::end(node->nodeWhile.body); ++it) {

                printNode(*it, indent + "  ");
            }

            break;
        case Node::DoWhile:

            printf("%sbody: \n", indent.c_str());
            for(std::vector<Node*>::iterator it = std::begin(node->nodeWhile.body); 
                    it != std::end(node->nodeWhile.body); ++it) {

                printNode(*it, indent + "  ");
            }

            printf("%scondition: \n", indent.c_str());
            printNode(node->nodeWhile.condition, indent + "  ");

            break;
        case Node::Break:
            printf("%sbreak\n", indent.c_str());

            break;
        case Node::Continue:
            printf("%scontinue\n", indent.c_str());

            break;
    }
}

int main(int argc, char **args) {
    for(std::string line; std::getline(std::cin, line);) {
        
    }
}

Node* parseExpression(EatableString *in) {
    return 0;
}

Node* parseMod(EatableString *in) {
    return 0;
}

Node* parseMulDiv(EatableString *in) {
    return 0;
}

Node* parseAddSub(EatableString *in) {
    return 0;
}

Node* parseBitwise(EatableString *in) {
    return 0;
}

Node* parseFactor(EatableString *in) {
    if(isNameFirstCharacter(*in->c)) {
        std::string word = readWord(in);
    }
    return 0;
}

Node* parseContant(EatableString *in) {
    return 0;
}

Node* parseVariable(EatableString *in) {
    return 0;
}

Node* parseDefinition(EatableString *in, DATA_TYPE dataType) {
    Node *node = (Node *) malloc(sizeof(Node)); 

    node->type = Node::Define;

    node->nodeDefine.type = (Node *) malloc(sizeof(Node));
    node->nodeDefine.type->type = Node::Type;
    node->nodeDefine.type->nodeType.type = dataType;

    node->nodeDefine.name = readWord(in);
    if(node->nodeDefine.name.empty()) {
        abortf("err: expected variable name\n");
    }

    if(in->eat('=')) {
        node->nodeDefine.value = parseExpression(in);
    }

    return node;
}

Node* parseAssignment(EatableString *in, DATA_TYPE dataType) {
    Node *node = (Node *) malloc(sizeof(Node)); 

    node->type = Node::Define;

    node->nodeDefine.type = (Node *) malloc(sizeof(Node));
    node->nodeDefine.type->type = Node::Type;
    node->nodeDefine.type->nodeType.type = dataType;

    node->nodeDefine.name = readWord(in);
    if(node->nodeDefine.name.empty()) {
        abortf("err: expected variable name\n");
    }

    if(in->eat('=')) {
        node->nodeDefine.value = parseExpression(in);
    }

    return node;
}

Node* parseType(EatableString *in) {
    if(in->eat('[')) {
        abortf("err: tuples not supported yet");
        return 0;
    }

    
    return 0;
}

Node* parse(std::string line) {
    EatableString in = EatableString(line.c_str());

    Node *result;

    Node *type;
    if((type = parseType(&in)) == nullptr) {

    }

    return result;
}