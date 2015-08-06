//=========================================================================
// GlobalTokens.h header file implimentation

#ifndef __GLOBALTOKENS_H_
#define __GLOBALTOKENS_H_

#pragma once

// Define Symbol States
#define TOKEN_DONE 0
#define SPACE 1
#define LESSTHAN 2
#define GREATERTHAN 3
#define COMMA 4
#define OPENP 5
#define CLOSEP 6
#define COLON 7
#define COMMENT 8
#define CLOSEB 9
#define OPENB 10
#define EMPTY 11
#define SEMICOLON 12
#define LEFT 13
#define RIGHT 14
#define UP 15
#define DOWN 16
#define ADVANCE 17
#define ROTATE 18
#define SHOOT 19
#define ANGLE 20
#define DISTANCE 21
#define GREATER 22
#define SMALLER 23
#define EQUAL 24
#define LESS_EQUAL 25
#define GREATER_EQUAL 26
#define NOT_EQUAL 27
#define NOT 28
#define LIFE 29
#define TALK 30
#define ATTACK 31
#define GIVE 32
#define GOLD 33
#define LEVEL 34
#define MAGIC 35
#define RANDOM 36
#define STOP 37
#define INPUT 38
#define CHASE 39
#define INC 40
#define DEC 41
#define PLUS_EQUAL 42
#define MINUS_EQUAL 43
#define OPENCB 44
#define CLOSECB 45
#define NOTEQUALS 46
#define QUOTE 47
#define TIMES 48
#define PERIOD 49
#define ALPHA 50
#define UNDERSCORE 51
#define DIGIT 52
#define WRONG_TOKEN -1

// Define Symbols
const char EqualsSign       = '=';
const char SpaceSign        = ' ';
const char MinusSign        = '-';
const char PlusSign         = '+';
const char TimesSign        = '*';
const char NotEqualsTo      = '!';
const char LessThanSign     = '<';
const char GreaterThanSign  = '>';
const char CommaSign        = ',';
const char QuoteSign        = '"';
const char OpenParSign      = '(';
const char ClosedParSign    = ')';
const char ColonSign        = ':';
const char CommentSign      = '/';
const char ClosedBraketSign = ']';
const char OpenBraketSign   = '[';
const char OpenCurlyBrace   = '{';
const char ClosedCurlyBrace = '}';
const char EmptySign        = '\n';
const char SemicolonSign    = ';';
const char PeriodSign       = '.';
const char UnderscoreSign   = '_';

#endif /* __GLOBALTOKENS_H_ */
