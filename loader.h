#if !defined(IMAGE_H)

/* ========================================================================
   File: image.h
   Date: 2016-07-27
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

internal bool
loadOBJ(char *ObjPath,
        glm::vec3 *Vertices,
        glm::vec2 *UVs,
        glm::vec3 *Normals)
{
    char *ObjFileContents = ReadEntireFileIntoMemoryAndNullTerminate(ObjPath);

    tokenizer Tokenizer = {};
    Tokenizer.At = ObjFileContents;

    bool Parsing = true;
    while(Parsing)
    {
        token Token = GetToken(&Tokenizer);
        switch(Token.Type)
        {
            case Token_EndOfStream:
            {
                Parsing = false;
            } break;
            case Token_Comment:
            {
                EatUntilEndOfLine(&Tokenizer);
            } break;

            case Token_Identifier:
            {                
                if(TokenEquals(Token, "v"))
                {
                }            
                if(TokenEquals(Token, "vt"))
                {
                }            
                if(TokenEquals(Token, "vn"))
                {
                }            
                if(TokenEquals(Token, "false"))
                {
                }
            } break;
                        
            default:
            {
            } break;
        }
    }
        
    return true;
}

internal GLuint
LoadBMP(const char *ImagePath)
{
    unsigned char Header[54];
    unsigned int DataPos;
    unsigned int Width, Height;
    unsigned int ImageSize;
    unsigned char *Data;

    FILE * File = fopen(ImagePath,"rb");
    if (!File){printf("Image could not be opened\n"); return 0;}
    
    if(fread(Header, 1, 54, File)!=54)
    { // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return 0;
    }
    
    if ( Header[0]!='B' || Header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Read ints from the byte array
    DataPos = *(int*)&(Header[0x0A]);
    ImageSize = *(int*)&(Header[0x22]);
    Width = *(int*)&(Header[0x12]);
    Height = *(int*)&(Header[0x16]);

    if(ImageSize==0) ImageSize=Width*Height*3;
    if(DataPos==0) DataPos=54;

    // Create a buffer
    Data = (unsigned char *)malloc(sizeof(char) * ImageSize);
    fread(Data,1,ImageSize,File);
    fclose(File);

    GLuint TextureID;
    glGenTextures(1, &TextureID);

    glBindTexture(GL_TEXTURE_2D, TextureID);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, Width, Height,
                 0, GL_BGR, GL_UNSIGNED_BYTE, Data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return TextureID;
}

#define IMAGE_H
#endif
