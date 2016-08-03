/* ========================================================================
   File: scene.cpp
   Date: 2016-07-26
   Creator: Steven Grissom
   ======================================================================== */

#include "scene.h"
#include "loader.h"

internal void
UpdateColors()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0,1);
    
    for (int v = 0; v < 12*3 ; ++v){
        CubeColors[3*v+0] = dis(gen);
        CubeColors[3*v+1] = dis(gen);
        CubeColors[3*v+2] = dis(gen);
    }
    
    for (int v = 0; v < 3 ; ++v){
        TriangleColors[3*v+0] = dis(gen);
        TriangleColors[3*v+1] = dis(gen);
        TriangleColors[3*v+2] = dis(gen);
    }
}

internal render_scene
SetupScene()
{
    render_scene Scene = {};

    Scene.Width = 1024;
    Scene.Height = 768;
    Scene.Timestamp = SDL_GetTicks();

    UpdateColors();

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
//    glEnable(GL_CULL_FACE);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //init camera
    Scene.Camera.Position = glm::vec3(0,2,10);
    Scene.Camera.Direction = glm::vec3(0,0,0);
    Scene.Camera.HorizontalAngle = 3.14f;
    Scene.Camera.VerticalAngle = 0;
    Scene.Camera.FieldOfView = 45;
    Scene.Camera.Speed = 3.0f;
    Scene.Camera.MouseSpeed = 0.005f;

    //init Triangle

    glGenBuffers(1, &Scene.Triangle.Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, Scene.Triangle.Buffer);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(TriangleVerts),
        TriangleVerts,
        GL_STATIC_DRAW);

    glGenBuffers(1, &Scene.Triangle.ColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, Scene.Triangle.ColorBuffer);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(TriangleColors),
        TriangleColors,
                 GL_STATIC_DRAW);

    Scene.Triangle.Position = glm::vec3(-3,0,0);
    Scene.Triangle.VerticesCount = 3;
    Scene.Triangle.ProgramID = LoadShaders((char *)Shaders[0], (char *)Shaders[1]);
    
    //init Cube
    
    glGenBuffers(1, &Scene.Cube.Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, Scene.Cube.Buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(CubeVerts),
                 CubeVerts,
                 GL_STATIC_DRAW);
    
    glGenBuffers(1, &Scene.Cube.ColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, Scene.Cube.ColorBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(CubeColors),
                 CubeColors,
                 GL_STATIC_DRAW);
    
    glGenBuffers(1, &Scene.Cube.UVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, Scene.Cube.UVBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(CubeUV),
                 CubeUV,
                 GL_STATIC_DRAW);

    Scene.Cube.Position = glm::vec3(3,0,0);
    Scene.Cube.VerticesCount = 3*12;
    Scene.Cube.ProgramID = LoadShaders((char *)Shaders[2], (char *)Shaders[3]);

    const char *path = "uvtemplate.bmp";    
    Scene.Cube.TextureBuffer = LoadBMP((char *)path);
    Scene.Cube.TextureID = glGetUniformLocation(Scene.Cube.ProgramID, "myTextureSampler");
    
    return Scene;
}

internal glm::vec3
GetCameraDirection(camera *Camera)
{
    return glm::vec3(
        cos(Camera->VerticalAngle) * sin(Camera->HorizontalAngle),
        sin(Camera->VerticalAngle),
        cos(Camera->VerticalAngle) * cos(Camera->HorizontalAngle)
                     );
}

internal glm::mat4
GetProjectionView(camera *Camera)
{
    glm::mat4 Projection = glm::perspective(glm::radians(Camera->FieldOfView),
                                            4.0f / 3.0f,
                                            0.1f, 100.0f);

    glm::vec3 Direction = GetCameraDirection(Camera);
    glm::vec3 Up = glm::cross(Camera->Right, Direction);
  
    glm::mat4 View = glm::lookAt(
        Camera->Position,
//        Camera->Position + Direction,
        glm::vec3(0,0,0),
        Up);


    return Projection * View;
}

internal glm::mat4
GetObjectModel(render_object RenderObject, glm::mat4 ProjectionView)
{
    RenderObject.MatrixID = glGetUniformLocation(RenderObject.ProgramID, "MVP");
    
    glm::mat4 Model = glm::translate(glm::mat4(1.0f), RenderObject.Position);

    return ProjectionView * Model;
}

internal void
UpdateScene(render_scene *Scene)
{
    uint32_t Timestamp = SDL_GetTicks();
    uint32_t dx = Timestamp - Scene->Timestamp;
    Scene->Timestamp = Timestamp;

    int MouseX = Scene->Camera.MouseX;
    int MouseY = Scene->Camera.MouseY;

    SDL_GetMouseState(&Scene->Camera.MouseX,
                      &Scene->Camera.MouseY);

    Scene->Camera.HorizontalAngle += Scene->Camera.MouseSpeed * dx
        * (float)(Scene->Camera.MouseX - MouseX);
    Scene->Camera.VerticalAngle += Scene->Camera.MouseSpeed * dx
        * (float)(Scene->Camera.MouseY - MouseY);

    
    Scene->Camera.Right = glm::vec3(
        sin(Scene->Camera.HorizontalAngle - 3.14f/2.0f),
        0,
        cos(Scene->Camera.HorizontalAngle - 3.14f/2.0f));

    int Radius = 10;
    Scene->Camera.Position = glm::vec3(0,0,0) +
        glm::vec3(Radius * cos(Scene->Camera.HorizontalAngle),
                  0,
                  Radius * sin(Scene->Camera.HorizontalAngle));

    const uint8_t *KeyState = SDL_GetKeyboardState(0);
        
    if (KeyState[SDL_SCANCODE_W])
    {
    }
    if(KeyState[SDL_SCANCODE_D])
    {
    }
    if(KeyState[SDL_SCANCODE_D])
    {
    }
    if(KeyState[SDL_SCANCODE_A])
    {
    }
    
    if(KeyState[SDL_SCANCODE_SPACE])
    {
        UpdateColors();

        glBindBuffer(GL_ARRAY_BUFFER, Scene->Triangle.ColorBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(TriangleColors),
                     TriangleColors,
                     GL_STATIC_DRAW);

    }
}

internal void
RenderScene(render_scene *Scene)
{
    UpdateScene(Scene);
    
    glm::mat4 ProjectionView = GetProjectionView(&Scene->Camera);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    //render triangle
	Scene->Triangle.MatrixID = glGetUniformLocation(Scene->Triangle.ProgramID, "MVP");

    Scene->Triangle.MVP = GetObjectModel(Scene->Triangle, ProjectionView);

    glUseProgram(Scene->Triangle.ProgramID);

    glUniformMatrix4fv(Scene->Triangle.MatrixID, 1, GL_FALSE, &Scene->Triangle.MVP[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, Scene->Triangle.Buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, Scene->Triangle.ColorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, Scene->Triangle.VerticesCount);
    //end render triangle

    //render cube
	Scene->Cube.MatrixID = glGetUniformLocation(Scene->Cube.ProgramID, "MVP");

    Scene->Cube.MVP = GetObjectModel(Scene->Cube, ProjectionView);

    glUseProgram(Scene->Cube.ProgramID);
    
    glUniformMatrix4fv(Scene->Cube.MatrixID, 1, GL_FALSE, &Scene->Cube.MVP[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Scene->Cube.TextureBuffer);
    glUniform1i(Scene->Cube.TextureID, 0);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, Scene->Cube.Buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, Scene->Cube.UVBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
    glDrawArrays(GL_TRIANGLES, 0, Scene->Cube.VerticesCount);
    //end render cube
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
