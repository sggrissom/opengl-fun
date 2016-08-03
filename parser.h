#if !defined(PARSER_H)
/* ========================================================================
   File: parser.h
   Date: 2016-07-30
   Creator: Steven Grissom
   ======================================================================== */

enum token_type
{
    Token_Unknown,
    
    Token_OpenParen,    
    Token_CloseParen,    
    Token_Colon,    
    Token_Semicolon,    
    Token_Asterisk,    
    Token_OpenBracket,    
    Token_CloseBracket,    
    Token_OpenBrace,    
    Token_CloseBrace,
    Token_Comment,

    Token_String,    
    Token_Identifier,    

    Token_EndOfStream,
};

struct token
{
    token_type Type;
    
    size_t TextLength;
    char *Text;
};

struct tokenizer
{
    char *At;
};

#define PARSER_H
#endif
