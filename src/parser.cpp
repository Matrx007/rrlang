#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <vector>
#include <memory.h>
#include <map>
#include <limits.h>
#include <any>

#include "logging.cpp"
#include "parsing.cpp"
#include "keywords.cpp"
#include "mem.cpp"
#include "fs.cpp"
#include "format.cpp"

int exitCode = 0;

class Node;

Node* parseType(EatableString *in);
Node* parseName(EatableString *in);
std::string parseString(EatableString *in);
Node* parseMemberAccess(EatableString *in);
Node* parseTarget(EatableString *in);
Node* parseLiteral(EatableString *in);
Node* parseCall(EatableString *in);
Node* parseFactor(EatableString *in);
Node* parseBitwise(EatableString *in);
Node* parseAddAndSub(EatableString *in);
Node* parseMulAndDiv(EatableString *in);
Node* parseValueComparison(EatableString *in);
Node* parseComparison(EatableString *in);
Node* parseAssignemnt(EatableString *in);
Node* parseExpression(EatableString *in);
Node* parseVariableDefinition(EatableString *in);
Node* parseReturn(EatableString *in);
Node* parseContinue(EatableString *in);
Node* parseBreak(EatableString *in);
Node* parseJoin(EatableString *in);
Node* parseIf(EatableString *in);
Node* parseWhile(EatableString *in);
Node* parseDoWhile(EatableString *in);
Node* parseFor(EatableString *in);
Node* parseStatement(EatableString *in);
Node* parseFunction(EatableString *in);
Node* parseSection(EatableString *in);
Node* parseStaticVariable(EatableString *in);
Node* parseDefinition(EatableString *in);
Node* parseFile(EatableString *in);
int parseScope(EatableString *in, std::vector<Node*>* body, char until);

void printError(EatableString *in, std::string msg);










#define START           const char* __original = in->c;
#define REVERT          {in->c = __original; return 0;}
#define RESET           in->c = __original;
#define ERR(MSG)        { printError(in, MSG); exit(1); return 0; }

enum DTYPE {
    u8, 
    i8, 
    u16, 
    i16, 
    u32, 
    i32, 
    f32,
    VOID,
    TUPLE
};

enum NodeType { EMPTY, STRING, NODE, INT, FLOAT, LIST };

struct NodeValue {
    enum NodeType type = EMPTY;

    void* data = 0;
};

class Node {
    private:

    std::string name;

    struct Parameter {
        std::string name;
        enum NodeType type = EMPTY;
        union {
            Node* nodeValue;
            std::string* stringValue;
            uint32_t* intValue;
            float* floatValue;
            std::vector<Node*>* listValue;

            void *data = 0;
        };
    };

    public:

    Node(std::string name) : name(name) {

    }

    std::map<std::string, NodeValue> children;

    Node* append(std::string key, std::string value) {
        if(children[key].data != 0) {
            abortf("err: node already has this key: %s\n", key.c_str());
        }

        struct NodeValue nodeValue = { NodeType::STRING, new std::string(value) };
        children[key] = nodeValue;

        return this;
    }

    Node* append(std::string key, Node value) {
        if(children[key].data != 0) {
            abortf("err: node already has this key: %s\n", key.c_str());
        }

        struct NodeValue nodeValue = { NodeType::NODE, new Node(value) };
        children[key] = nodeValue;

        return this;
    }

    Node* append(std::string key, Node* value) {
        if(children[key].data != 0) {
            abortf("err: node already has this key: %s\n", key.c_str());
        }

        struct NodeValue nodeValue = { NodeType::NODE, value };
        children[key] = nodeValue;

        return this;
    }

    Node* append(std::string key, uint32_t value) {
        if(children[key].data != 0) {
            abortf("err: node already has this key: %s\n", key.c_str());
        }

        struct NodeValue nodeValue = { NodeType::INT, malloc(sizeof(uint32_t)) };
        *(uint32_t*)nodeValue.data = value;
        children[key] = nodeValue;

        return this;
    }

    Node* append(std::string key, float value) {
        if(children[key].data != 0) {
            abortf("err: node already has this key: %s\n", key.c_str());
        }

        struct NodeValue nodeValue = { NodeType::FLOAT, malloc(sizeof(float)) };
        *(float*)nodeValue.data = value;
        children[key] = nodeValue;

        return this;
    }

    Node* append(std::string key, std::vector<Node*> value) {
        if(children[key].data != 0) {
            abortf("err: node already has this key: %s\n", key.c_str());
        }

        struct NodeValue nodeValue = { NodeType::LIST, new std::vector<Node*>(value) };
        children[key] = nodeValue;

        return this;
    }

    std::string print() {
        std::string output = name + "{";

        bool first = true;
        std::map<std::string, NodeValue>::iterator it;
        for(it = children.begin(); it != children.end(); it++) {
            if(it->second.data == 0) break;

            if(first) first = false;
            else output += ',';
            
            output += it->first + ": ";
            
            switch (it->second.type) {
                case NodeType::STRING:
                    output += "\"" + *((std::string*)it->second.data) + "\"";
                    break;
                case NodeType::NODE:
                    output += ((Node*)it->second.data)->print();
                    break;
                case NodeType::INT:
                    output += std::to_string(*(uint32_t*)it->second.data);
                    break;
                case NodeType::FLOAT:
                    output += std::to_string(*(float*)it->second.data);
                    break;
                case NodeType::LIST:
                    {
                        int first = 1;
                        for(Node* node : *(std::vector<Node*>*)it->second.data) {
                            if(first) first = 0;
                            else output += ", ";
                            output += node->print();
                        }
                    }
                    break;
                case NodeType::EMPTY:
                    // Don't print anything
                    break;
            }
        }

        output += "}";

        return output;
    }

};

std::string combine(std::string name, std::string* members...) {
    va_list args;
    va_start(args, members);

    

    std::string result = name + '{';
    
    bool isName = true;
    bool isFirst = true;
    while(members != 0) {
        std::string* member = va_arg(args, std::string*);

        if(isName) {
            result += *member + ": ";
        } else {
            if(isFirst) {
                result += *member;
                isFirst = false;
            } else {
                result += *member + ", ";
            }
        }

        isName = !isName;
    }

    result += '}';

    return result;
}

void prettyPrint(std::string string) {
    EatableString in = EatableString(string.c_str());
    
    std::string result;

    uint32_t indent = 0;
    while(*in.c != 0) {
        if(*in.c == '{' || *in.c == '[') {
            result += ' ';
            result += *in.c;
            indent++;
            result += '\n';

            for(uint32_t i = 0; i < indent; i++) {
                result += "    ";
            }
        } else if(*in.c == '}' || *in.c == ']') {
            
            result += '\n';
            indent--;

            for(uint32_t i = 0; i < indent; i++) {
                result += "    ";
            }
            
            result += *in.c;
            result += '\n';

            for(uint32_t i = 0; i < indent; i++) {
                result += "    ";
            }
        } else if(*in.c == ':') {
            result += ": ";
        } else if(*in.c == ',') {
            result += *in.c;
            result += '\n';

            for(uint32_t i = 0; i < indent; i++) {
                result += "    ";
            }
        } else if(*in.c != ' ' && *in.c != '\n') {
            result += *in.c;
        }

        in.c++;
    }

    printf("%s\n", result.c_str());
}

void printError(EatableString *in, std::string msg) {
    printf("error at line %d: %s\n", in->line, msg.c_str());
    
    int lineNumberWidth = numPlaces(in->line);
    const char* line = in->currentLine();
    int offset = in->c - line;
    
    printf(" %d | ", in->line);

    for(; *line != '\n'; line++) {
        fputc(*line, stdout);
    }
    fputc('\n', stdout);

    printIndent(lineNumberWidth+2);
    fputc('|', stdout);

    printIndent(offset+1);

    fputs("^..", stdout);
    fputc('\n', stdout);
}


Node* parseType(EatableString *in) {
    START;

    int8_t pointerLevel = 0;

    DTYPE dtype;
    std::string dtypeName;

    std::vector<Node*> fields;

    if(in->eat('[')) {
        dtype = TUPLE;
        dtypeName = "tuple";

        do {
            fields.push_back(parseType(in));
        } while(in->eat(','));
        
        if(!in->eat(']')) {
            ERR("expected closing brackets");
        }
    } else {
        std::string word = readWord(in);
        uint64_t wordHash = hash(word.c_str());
        switch (wordHash) {
            case HASH_u8:
                if(word.compare("u8")) REVERT;

                dtype = u8;
                dtypeName ="u8"; 
                break;
            case HASH_u16:
                if(word.compare("u16")) REVERT;

                dtype = u16;
                dtypeName = "u16";
                break;
            case HASH_u32:
                if(word.compare("u32")) REVERT;

                dtype = u32;
                dtypeName = "u32";
                break;
            case HASH_i8:
                if(word.compare("i8")) REVERT;

                dtype = i8;
                dtypeName ="i8"; 
                break;
            case HASH_i16:
                if(word.compare("i16")) REVERT;

                dtype = i16;
                dtypeName = "i16";
                break;
            case HASH_i32:
                if(word.compare("i32")) REVERT;

                dtype = i32;
                dtypeName = "i32";
                break;
            case HASH_f32:
                if(word.compare("f32")) REVERT;

                dtype = f32;
                dtypeName = "f32";
                break;
            case HASH_void:
                if(word.compare("void")) REVERT;

                dtype = VOID;
                dtypeName = "VOID";
                break;
            default:
                ERR("expected a type");
        }
    }

    uint64_t sizeMultiplier = 1;
    if(in->eat('[')) {
        sizeMultiplier = readInteger(in);

        if(sizeMultiplier == 0) {
            ERR("array size can't be zero");
        }

        if(!in->eat(']')) {
            ERR("expected closing brackets");
        }
    }

    while(in->eat('*')) {
        pointerLevel++;
    }

    Node* node = (new Node("type"))
                    ->append("dtype", dtypeName)
                    ->append("pointerLevel", (uint32_t)pointerLevel)
                    ->append("sizeMultiplier", (uint32_t)sizeMultiplier);
    
    if(dtype == TUPLE) {
        node->append("fields", fields);
    }

    return node;
}

Node* parseName(EatableString *in) {
    START;
    std::string word = readWord(in);

    if(word.empty()) {
        REVERT;    
    }

    return (new Node("name"))->append("value", word);
}

std::string parseString(EatableString *in) {
    START;
    std::string word = readWord(in);

    return word;
}

Node* parseTarget(EatableString *in) {
    START;

    Node* node = 0;

    if(in->eat('&')) {
        std::string name = parseString(in);

        if(name.empty()) REVERT;

        node = (new Node("variableAccess"))
                ->append("target", name)
                ->append("pointerLevel", (uint32_t)0)
                ->append("returnAddress", (uint32_t)1);
    } else {
        int8_t pointerLevel = 0;

        while(in->next('*')) {
            pointerLevel++;
        }

        std::string name = parseString(in);
        if(name.empty()) REVERT;

        node = (new Node("variableAccess"))
                ->append("name", name)
                ->append("pointerLevel", (uint32_t)pointerLevel)
                ->append("returnAddress", (uint32_t)0);

        if(*in->c == '.' || *in->c == '-') {
            Node* member = 0;
            Node* last = member;

            do {
                int8_t memberPointerLevel;
                
                if(*in->c == '.') {
                    in->nextChar();
                    memberPointerLevel = 0;
                } else if(*in->c == '-') {
                    while(*in->c == '-') {
                        in->nextChar();
                        memberPointerLevel++;
                    }

                    if(!in->next('>')) {
                        return node;
                    }
                }

                std::string memberName = parseString(in);

                if(member == 0) {
                    member = (new Node("memberAccess"))
                                ->append("pointerLevel", (uint32_t)memberPointerLevel)
                                ->append("name", memberName);
                    last = member;
                } else {
                    Node *next = (new Node("memberAccess"))
                                ->append("pointerLevel", (uint32_t)memberPointerLevel)
                                ->append("name", memberName);
                    last->append("member", next);
                    last = next;
                }
            } while(in->is('-') || in->is('.'));

            node->append("member", member);
        }

    }

    return node;
}

Node* parseLiteral(EatableString *in, int temp) {
    START;

    in->trim();
    enum { INT, HEX, BIN, FLT, STR, CHR } type;
    const char* typeName = 0;

    if(*in->c == '0') {
        
    }

    return 0;
}

Node* parseLiteral(EatableString *in) {
    START;

    in->trim();
    // H E X
    if(*in->c == '0' && *(in->c + 1) == 'x') {
        uint32_t integerValue = 0;
        
        in->nextChar();
        in->nextChar();
        
        while((*in->c >= '0' && *in->c <= '9') || (*in->c >= 'a' && *in->c <= 'f') || (*in->c >= 'A' && *in->c <= 'F')) {
            integerValue <<= 4;
            if(*in->c >= '0' && *in->c <= '9') {
                integerValue += *in->c - '0';
            }
            if(*in->c >= 'a' && *in->c <= 'f') {
                integerValue += *in->c - 'a' + 10;
            }
            if(*in->c >= 'A' && *in->c <= 'F') {
                integerValue += *in->c - 'A' + 10;
            }
            
            in->nextChar();
        }
    
        return (new Node("literal"))
                ->append("type", "integer")
                ->append("value", (uint32_t)integerValue);
    } else 
    
    // B I N A R Y
    if(*in->c == '0' && *(in->c + 1) == 'b') {
        uint32_t integerValue = 0;
        
        in->nextChar();
        in->nextChar();
        
        while(*in->c >= '0' && *in->c <= '1') {
            integerValue <<= 1;
            integerValue += *in->c - '0';
            
            in->nextChar();
        }
    
        return (new Node("literal"))
                ->append("type", "integer")
                ->append("value", (uint32_t)integerValue);
    } else 
    
    // C H A R A C T E R
    if(*in->c == '\'') {
        uint32_t integerValue = 0;
        
        in->nextChar();
        integerValue = *in->c;
        in->nextChar();
        
        if(!in->eat('\'')) ERR("expected ' after character literal");
    
        return (new Node("literal"))
                ->append("type", "integer")
                ->append("value", (uint32_t)integerValue);
    } else 
    
    // S T R I N G
    if(*in->c == '"') {
        in->nextChar();
        
        std::string stringValue;
        
        while(*in->c != '"') {
            if(*in->c == '\\') {
                in->nextChar();
                switch (*in->c) {
                    case '\a':
                        stringValue += 0x07;
                        break;
                    case '\b':
                        stringValue += 0x08;
                        break;
                    case '\e':
                        stringValue += 0x1B;
                        break;
                    case '\f':
                        stringValue += 0x0C;
                        break;
                    case '\n':
                        stringValue += 0x0A;
                        break;
                    case '\r':
                        stringValue += 0x0D;
                        break;
                    case '\t':
                        stringValue += 0x09;
                        break;
                    case '\v':
                        stringValue += 0x0B;
                        break;
                    default:
                        stringValue += *in->c;
                        break;
                }
            } else {
                stringValue += *in->c;
            }
            in->nextChar();
        }
        
        if(!in->eat('"')) ERR("expected \" after string literal");
    
        return (new Node("literal"))
                ->append("type", "string")
                ->append("value", stringValue);
    } else 
    
    // I N T E G E R   &   F L O A T
    if(*in->c >= '0' && *in->c <= '9') {
        uint32_t integerValue = 0;
        float floatValue = 0;
        
        bool isFloat = false;
        
        while(*in->c >= '0' && *in->c <= '9') {
            integerValue += *in->c - '0';
            integerValue *= 10;
            
            in->nextChar();
        }
        
        if(*in->c == '.') {
            isFloat = true;
            float fractionPart = 0;
            floatValue = integerValue;
        
            while(*in->c >= '0' && *in->c <= '9') {
                fractionPart /= 10.0;
                fractionPart += (*in->c - '0') / 10.0;
                
                in->nextChar();
            }
            
            floatValue += fractionPart;
    
            return (new Node("literal"))
                    ->append("type", "float")
                    ->append("value", floatValue);
        }
    
        return (new Node("literal"))
                ->append("type", "integer")
                ->append("value", (uint32_t)integerValue);
    }

    REVERT;
}

Node* parseCall(EatableString *in) {
    START;
    Node* target = parseTarget(in);
    if(!target) REVERT;

    std::vector<Node*> parameters;

    if(!in->eat('(')) {
        REVERT;
    }

    if(!in->eat(')')) {
        do {
            Node* parameter = parseExpression(in);
            if(!parameter) REVERT;

            parameters.push_back(parameter);
        } while(in->eat(','));

        if(!in->eat(')')) {
            REVERT;
        }
    }

    return (new Node("call"))
            ->append("target", target)
            ->append("parameters", parameters);
}

Node* parseFactor(EatableString *in) {
    START;
    Node* factor;

    
    if((factor = parseCall(in))) {
        return (new Node("factor"))
                ->append("call", factor);
    } else RESET;

    if((factor = parseTarget(in))) {
        return (new Node("factor"))
                ->append("variableAccess", factor);
    } else RESET;

    if((factor = parseLiteral(in))) {
        return (new Node("factor"))
                ->append("literal", factor);
    } else REVERT;
}

Node *parseArrayAccess(EatableString *in) {
    START;

    Node* left = parseFactor(in);
    
    while(in->eat('[')) {

        uint32_t index = readInteger(in);

        left = (new Node("arrayAccess"))
                ->append("left", left)
                ->append("index", (uint32_t)index);

        if(!in->eat(']')) REVERT;
    }
    
    return left;
}

Node* parseIncAndDec(EatableString *in) {
    START;

    in->trim();
    if(*in->c == '+' || *in->c == '-') {
        if(*(in->c+1) == *in->c) {
            std::string operation;
            operation.push_back(*in->c);
            operation.push_back(*in->c);

            in->nextChar();
            in->nextChar();

            Node* right = parseTarget(in);

            if(right == 0) ERR("syntax error in expression");

            return (new Node("preOperation"))
                    ->append("operation", operation)
                    ->append("variable", right);
        }
    }

    Node* left = parseArrayAccess(in);

    in->trim();
    if(*in->c == '+' || *in->c == '-') {
        if(*(in->c+1) == *in->c) {
            std::string operation;
            operation.push_back(*in->c);
            operation.push_back(*in->c);

            in->nextChar();
            in->nextChar();

            return (new Node("postOperation"))
                    ->append("operation", operation)
                    ->append("variable", left);
        }
    }
    
    return left;
}

Node* parseMulAndDiv(EatableString *in) {
    START;

    Node* left = parseIncAndDec(in);

    in->trim();
    char c = *in->c;
    while(*in->c == '*' || *in->c == '/') {
        if(*(in->c+1) == '=') return left;
        
        in->nextChar();

        Node* right = parseIncAndDec(in);

        if(right == 0) ERR("syntax error in expression");

        left = (new Node("binaryOperator"))
                    ->append("operation", std::string(1, c))
                    ->append("left", left)
                    ->append("right", right);
    }

    return left;
}

Node* parseAddAndSub(EatableString *in) {
    START;

    Node* left = parseMulAndDiv(in);

    in->trim();
    char c = *in->c;
    while(*in->c == '+' || *in->c == '-') {
        if(*(in->c+1) == '=') return left;
        
        in->nextChar();

        Node* right = parseMulAndDiv(in);

        if(right == 0) ERR("syntax error in expression");

        left = (new Node("binaryOperator"))
                    ->append("operation", std::string(1, c))
                    ->append("left", left)
                    ->append("right", right);
    }

    return left;
}

Node* parseBitwise(EatableString *in) {
    START;

    Node* left = parseAddAndSub(in);

    in->trim();
    char c = *in->c;
    while(*in->c == '&' || *in->c == '|' || *in->c == '^') {
        if(*(in->c+1) == '=') return left;
        
        in->nextChar();

        Node* right = parseAddAndSub(in);

        if(right == 0) ERR("syntax error in expression");

        left = (new Node("binaryOperator"))
                    ->append("operation", std::string(1, c))
                    ->append("left", left)
                    ->append("right", right);
    }

    return left;
}

Node* parseValueComparison(EatableString *in) {
    START;

    Node* left = parseBitwise(in);

    in->trim();
    char c = *in->c;
    while(*in->c == '>' || *in->c == '<' || *in->c == '=' || *in->c == '!') {
        if(*in->c == '>' || *in->c == '<') {
            if(*(in->c+1) == '=') {
                std::string operation;
                operation.push_back(c);
                operation.push_back('=');

                in->nextChar();
                in->nextChar();

                Node* right = parseBitwise(in);

                if(right == 0) ERR("syntax error in expression");

                left = (new Node("binaryOperator"))
                            ->append("operation", operation)
                            ->append("left", left)
                            ->append("right", right);
            } else {
                std::string operation;
                operation.push_back(c);

                in->nextChar();

                Node* right = parseBitwise(in);

                if(right == 0) ERR("syntax error in expression");

                left = (new Node("binaryOperator"))
                            ->append("operation", operation)
                            ->append("left", left)
                            ->append("right", right);
            }
        } else if(*in->c == '=' && *(in->c+1) == '=') {
            in->nextChar();
            in->nextChar();

            Node* right = parseBitwise(in);

            if(right == 0) ERR("syntax error in expression");

            left = (new Node("binaryOperator"))
                        ->append("operation", "==")
                        ->append("left", left)
                        ->append("right", right);
        } else if(*in->c == '!' && *(in->c+1) == '=') {
            in->nextChar();
            in->nextChar();

            Node* right = parseBitwise(in);

            if(right == 0) ERR("syntax error in expression");

            left = (new Node("binaryOperator"))
                        ->append("operation", "!=")
                        ->append("left", left)
                        ->append("right", right);
        } else break;
    }

    return left;
}

Node* parseComparison(EatableString *in) {
    START;

    Node* left = parseValueComparison(in);

    while(*in->c == '&' || *in->c == '|' || *in->c == '^') {
        if(*(in->c+1) == *in->c) {
            std::string operation;
            operation.push_back(*in->c);
            operation.push_back(*in->c);

            in->nextChar();
            in->nextChar();

            Node* right = parseValueComparison(in);

            if(right == 0) ERR("syntax error in expression");

            left = (new Node("binaryOperator"))
                        ->append("operation", operation)
                        ->append("left", left)
                        ->append("right", right);
        }
    }

    return left;
}

Node* parseAssignment(EatableString *in) {
    START;

    Node* left = parseComparison(in);
    
    if(*in->c == '=') {
        in->nextChar();

        Node* right = parseComparison(in);

        if(right == 0) ERR("syntax error in expression");

        left = (new Node("binaryOperator"))
                    ->append("operation", "=")
                    ->append("left", left)
                    ->append("right", right);
    } else {
        char operation = 0;
        std::string operationString;
        
        switch (*in->c) {
            case '+':    
            case '-':    
            case '*':    
            // case '/':
                operation = *in->c;
                operationString += operation;
                in->nextChar();
                break;
            
            case '>':
            case '<':
                operation = *in->c;
                operationString += operation;
                operationString += operation;
                in->nextChar();
                
                if(operation != *in->c) ERR("unknown assignment operation");
                in->nextChar();
                break;
            default:
                return left;
                break;
        }
        operationString += '=';
                
        if(!in->eat('=')) ERR("expected '=' after assignment operation");

        Node* right = parseComparison(in);

        if(right == 0) ERR("syntax error in expression");

        left = (new Node("binaryOperator"))
                    ->append("operation", operationString)
                    ->append("left", left)
                    ->append("right", right);
    }
    
    return left;
}

// Alias to parseComparison() as this is the first arithmetic operation that should be parser when parsing an expression
Node* parseExpression(EatableString *in) {
    in->trim();
    Node* value = parseAssignment(in);

    return value;
}

Node* parseVariableDefinition(EatableString *in) {
    START;

    std::string firstWord = readWord(in);
    if(firstWord.compare("let") != 0) {
        REVERT;
    }

    std::vector<Node*> variables;

    do {
        std::string name = parseString(in);
        if(name.empty()) REVERT;

        if(in->eat('=')) {
            Node* value = parseExpression(in);

            variables.push_back((new Node("variable"))
                                ->append("name", name)
                                ->append("value", value));
        } else {
            variables.push_back((new Node("variable"))  
                                ->append("name", name));
        }
    } while(in->eat(','));

    if(!in->eat(':')) REVERT;

    Node* type = parseType(in);
    if(!type) ERR("expected a type");

    return (new Node("variableDefinition"))
            ->append("type", type)
            ->append("variables", variables);
}

Node* parseReturn(EatableString *in) {
    START;

    std::string firstWord = readWord(in);
    if(firstWord.compare("return") != 0) {
        REVERT;
    }

    Node* returnValue = parseExpression(in);
    if(returnValue != 0) {
        return (new Node("return"))
                ->append("value", returnValue);
    }
    
    return new Node("return");
}

Node* parseContinue(EatableString *in) {
    START;

    std::string firstWord = readWord(in);
    if(firstWord.compare("continue") != 0) {
        REVERT;
    }

    std::string label = parseString(in);
    if(!label.empty()) {
        return (new Node("continue"))
                ->append("label", label);
    }
    
    return new Node("continue");
}

Node* parseBreak(EatableString *in) {
    START;

    std::string firstWord = readWord(in);
    if(firstWord.compare("break") != 0) {
        REVERT;
    }

    std::string label = parseString(in);
    if(!label.empty()) {
        return (new Node("break"))
                ->append("label", label);
    }
    
    return new Node("break");
}

Node* parseJoin(EatableString *in) {
    START;
    
    REVERT;
}

Node* parseIf(EatableString *in) {
    START;

    std::string firstWord = readWord(in);
    if(firstWord.compare("if") != 0) {
        REVERT;
    }

    if(!in->eat('(')) ERR("expected an '('");

    Node* condition = parseExpression(in);

    if(condition == 0) ERR("expected a condition");

    if(!in->eat(')')) ERR("expected a ')'");

    std::vector<Node*> body;

    if(!in->eat('{')) ERR("expected an '{'");

    parseScope(in, &body, '}');
    
    return (new Node("if"))
            ->append("condition", condition)
            ->append("body", body);
}

Node* parseWhile(EatableString *in) {
    START;

    std::string firstWord = readWord(in);
    if(firstWord.compare("while") != 0) {
        REVERT;
    }

    if(!in->eat('(')) ERR("expected an '('");

    Node* condition = parseExpression(in);

    if(condition == 0) ERR("expected a condition");

    if(!in->eat(')')) ERR("expected a ')'");

    std::vector<Node*> body;

    if(!in->eat('{')) ERR("expected an '{'");

    parseScope(in, &body, '}');
    
    return (new Node("while"))
            ->append("condition", condition)
            ->append("body", body);
}

Node* parseDoWhile(EatableString *in) {
    START;

    std::string firstWord = readWord(in);
    if(firstWord.compare("do") != 0) {
        REVERT;
    }

    std::vector<Node*> body;

    if(!in->eat('{')) REVERT;

    parseScope(in, &body, '}');

    firstWord = readWord(in);
    if(firstWord.compare("while") != 0) {
        REVERT;
    }

    if(!in->eat('(')) ERR("expected an '('");

    Node* condition = parseExpression(in);

    if(condition == 0) ERR("expected a condition");

    if(!in->eat(')')) ERR("expected a ')'");
    
    return (new Node("doWhile"))
            ->append("condition", condition)
            ->append("body", body);
}

Node* parseFor(EatableString *in) {
    START;

    std::string firstWord = readWord(in);
    if(firstWord.compare("for") != 0) {
        REVERT;
    }



    if(!in->eat('(')) ERR("expected an '('");

    Node* initialization = parseStatement(in);

    if(!in->eat(';')) ERR("expected a ';'");
    
    Node* condition = parseExpression(in);
    if(condition == 0) ERR("expected a condition");
    
    if(!in->eat(';')) ERR("expected a ';'");
    
    Node* update = parseStatement(in);

    if(!in->eat(')')) ERR("expected a ')'");
    


    std::vector<Node*> body;

    if(!in->eat('{')) ERR("expected an '{'");

    parseScope(in, &body, '}');
    
    return (new Node("for"))
            ->append("initialization", initialization)
            ->append("condition", condition)
            ->append("update", update)
            ->append("body", body);
}

Node* parseStatement(EatableString *in) {
    START;

    Node* statement = 0;

    statement = parseFor(in);
    if(statement != 0) return statement;
    statement = parseDoWhile(in);
    if(statement != 0) return statement;
    statement = parseWhile(in);
    if(statement != 0) return statement;
    statement = parseIf(in);
    if(statement != 0) return statement;
    statement = parseJoin(in);
    if(statement != 0) return statement;
    statement = parseBreak(in);
    if(statement != 0) return statement;
    statement = parseContinue(in);
    if(statement != 0) return statement;
    statement = parseReturn(in);
    if(statement != 0) return statement;
    statement = parseVariableDefinition(in);
    if(statement != 0) return statement;
    statement = parseExpression(in);
    if(statement != 0) return statement;
    
    // Maybe this should be used instead?
    // return new Node("nullStatement");

    REVERT;
}

Node* parseFunction(EatableString *in) {
    START;

    std::string firstWord = readWord(in);
    if(firstWord.compare("fn") != 0) {
        REVERT;
    }

    std::string name = parseString(in);
    if(name.empty()) ERR("expected a function name");

    std::vector<Node*> parameters;

    if(!in->eat('(')) ERR("expected an '('");

    if(!in->eat(')')) {
        do {
            std::string name = parseString(in);
            if(name.empty()) ERR("expected an argument");

            if(!in->eat(':')) ERR("expected a ':'");

            Node* type = parseType(in);
            if(!type) ERR("expected a type");

            parameters.push_back((new Node("parameter"))
                                    ->append("type", type)
                                    ->append("name", name));
        } while(in->eat(','));

        if(!in->eat(')')) ERR("expected a ')'");
    }

    if(!in->eat(':')) ERR("expected a ':");

    Node* type = parseType(in);
    if(!type) ERR("expected a return type");

    if(in->eat('{')) {

        std::vector<Node*> body;

        parseScope(in, &body, '}');

        return (new Node("function"))
                ->append("type", type)
                ->append("name", name)
                ->append("parameters", parameters)
                ->append("body", body);
    }

    return (new Node("function"))
            ->append("type", type)
            ->append("name", name)
            ->append("parameters", parameters);
}

Node* parseSection(EatableString *in) {
    START;
    
    REVERT;
}

Node* parseStaticVariable(EatableString *in) {
    START;

    std::string firstWord = readWord(in);
    if(firstWord.compare("static") != 0) {
        REVERT;
    }

    std::vector<Node*> variables;

    do {
        std::string name = parseString(in);
        if(name.empty()) ERR("expected a name");

        if(in->eat('=')) {
            Node* value = parseExpression(in);

            if(!value) ERR("expected an initialization value");

            variables.push_back((new Node("variable"))
                                ->append("name", name)
                                ->append("value", value));
        } else {
            variables.push_back((new Node("variable"))  
                                ->append("name", name));
        }
    } while(in->eat(','));

    if(!in->eat(':')) ERR("expected a ':");

    Node* type = parseType(in);
    if(!type) ERR("expected a type");

    return (new Node("staticVariableDefinition"))
            ->append("type", type)
            ->append("variables", variables);
}

Node* parseDefinition(EatableString *in) {
    START;

    Node* statement = 0;

    statement = parseFunction(in);
    if(statement != 0) return statement;
    statement = parseSection(in);
    if(statement != 0) return statement;
    statement = parseStaticVariable(in);
    if(statement != 0) return statement;
    
    REVERT;
}

Node* parseFile(EatableString *in) {
    std::vector<Node*> definitions;
    
    while((*in->c) != 0) {
        Node* definition = parseDefinition(in);
        
        if(!in->eat(';')) ERR("expected a ';'");
        in->trim();
        
        definitions.push_back(definition);
    }

    Node* root = (new Node("file"))
                    ->append("definitions", definitions);

    return root;
}

int parseScope(EatableString *in, std::vector<Node*>* body, char until) {

    while(!in->eat(until)) {
        Node* line = parseStatement(in);

        if(line == 0) {
            printError(in, "syntax error in statement");
            continue;
        }

        const char* beforeTrimming = in->c;

        // If semi-colon wasn't found: undo trim, print error, redo trim
        if(!in->eat(';')) {
            const char* afterTrimming = in->c;
            
            // undo trim
            in->c = beforeTrimming-1;
            while((*in->c == ' ' || *in->c == '\n') && in->c > in->string) {
                in->c--;
            }

            ERR("missing ';' after statement");

            in->c = afterTrimming;
        }

        body->push_back(line);
    }

    return 0;
}


void parse(std::string line) {
    EatableString in = EatableString(line.c_str());
    const char *start = in.c;

    printf("\n\nParsing line: %s\n\n", line.c_str());
    
    Node* result;

    result = parseFile(&in);
    if(result) return prettyPrint(result->print());
    in.c = start;
}

int main(int argc, char **args) {

    if(argc == 2) {
        char* file = readEntireFile(args[1]);
        std::string fileString = std::string(file);

        parse(fileString);
    }

    return 0;
}