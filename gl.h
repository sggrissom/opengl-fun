#if !defined(GL_H)
/* ========================================================================
   File: gl.h
   Date: 2016-07-26
   Creator: Steven Grissom
   ======================================================================== */


#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

void _check_gl_error(const char *file, int line) {
        GLenum err (glGetError());
 
        while(err!=GL_NO_ERROR) {
                switch(err) {
                    case GL_INVALID_OPERATION:      printf("INVALID_OPERATION");      break;
                    case GL_INVALID_ENUM:           printf("INVALID_ENUM");           break;
                    case GL_INVALID_VALUE:          printf("INVALID_VALUE");          break;
                    case GL_OUT_OF_MEMORY:          printf("OUT_OF_MEMORY");          break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION:  printf("INVALID_FRAMEBUFFER_OPERATION");  break;
                }

                printf(" - %s:%d\n", file, line);
                err=glGetError();
        }
}

internal char *
ReadEntireFileIntoMemoryAndNullTerminate(char *FileName)
{
    char *Result = 0;
    
    FILE *File = fopen(FileName, "r");
    if(File)
    {
        fseek(File, 0, SEEK_END);
        size_t FileSize = ftell(File);
        fseek(File, 0, SEEK_SET);

        Result = (char *)malloc(FileSize + 1);
        fread(Result, FileSize, 1, File);
        Result[FileSize] = 0;
        
        fclose(File);
    }

    return(Result);
}

internal GLuint
LoadShaders(char *VertexShaderPath, char *FragmentShaderPath)
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", VertexShaderPath);
	char *VertexSourcePointer = ReadEntireFileIntoMemoryAndNullTerminate(VertexShaderPath);
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , 0);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(InfoLogLength > 0)
    {
		char VertexShaderErrorMessage[InfoLogLength+1];
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, 0, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", FragmentShaderPath);
	char *FragmentSourcePointer = ReadEntireFileIntoMemoryAndNullTerminate(FragmentShaderPath);
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, 0);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(InfoLogLength > 0)
    {
		char FragmentShaderErrorMessage[InfoLogLength+1];
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, 0, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(InfoLogLength > 0)
    {
	    char ProgramErrorMessage[InfoLogLength+1];
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

#define GL_H
#endif
