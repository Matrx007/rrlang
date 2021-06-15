#pragma once

#include "parsing.cpp"
#include "stdint.h"
#include <bits/stdint-uintn.h>
#include <string>

// ################
// ### KEYWORDS ###
// ################


#define HASH_u8         (uint64_t) 5863826
#define HASH_u16        (uint64_t) 193506081
#define HASH_u32        (uint64_t) 193506143
#define HASH_i8         (uint64_t) 5863430
#define HASH_i16        (uint64_t) 193493013
#define HASH_i32        (uint64_t) 193493075
#define HASH_f32        (uint64_t) 193489808
#define HASH_struct     (uint64_t) 6954031505834
#define HASH_enum       (uint64_t) 6385194298
#define HASH_tuple      (uint64_t) 210729446831

#define HASH_function   (uint64_t) 7572387384277067
#define HASH_return     (uint64_t) 6953974653989

#define HASH_break      (uint64_t) 210707980106
#define HASH_continue   (uint64_t) 7572251799911306

#define HASH_static     (uint64_t) 6954030893997
#define HASH_void       (uint64_t) 6385805911
// #define HASH_section    (uint64_t) 229482434843354

const uint64_t hash(const char *str) {
    uint64_t hash = 5381;  
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

bool __isKeyword(uint64_t hash) {
    switch (hash) {
        case HASH_u8:
        case HASH_u16:
        case HASH_u32:
        case HASH_i8:
        case HASH_i16:
        case HASH_i32:
        case HASH_f32:
        case HASH_struct:
        case HASH_enum:
        case HASH_tuple:

        case HASH_break:
        case HASH_continue:

        case HASH_function:
        case HASH_return:

        case HASH_static:

            abortf("err: didn't expected keyword\n");
            return true;
            break;
        default:
            return false;
    }
}

bool isKeyword(std::string word) {
    uint64_t wordHash = hash(word.c_str());
    bool isKeyword = __isKeyword(wordHash);

    if(isKeyword) {
        switch (wordHash) {
            case HASH_u8:
                return word.compare("u8");
            case HASH_u16:
                return word.compare("u16");
            case HASH_u32:
                return word.compare("u32");
            case HASH_i8:
                return word.compare("i8");
            case HASH_i16:
                return word.compare("i16");
            case HASH_i32:
                return word.compare("i32");
            case HASH_f32:
                return word.compare("f32");
            case HASH_struct:
                return word.compare("struct");
            case HASH_enum:
                return word.compare("enum");
            case HASH_tuple:
                return word.compare("tuple");
            case HASH_break:
                return word.compare("break");
            case HASH_continue:
                return word.compare("continue");
            case HASH_function:
                return word.compare("function");
            case HASH_return:
                return word.compare("return");
            case HASH_static:
                return word.compare("static");
            default:
                return false;
        }
    } else return false;
}