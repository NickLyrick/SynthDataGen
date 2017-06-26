#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include <algorithm>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <ImageMagick-7/Magick++.h>
#include <glm/glm.hpp>

class SkyBox
{
public:
    GLfloat*                    skyboxVertices;
    GLuint                      skyboxVAO;
    GLuint                      skyboxVBO;
    std::vector<std::string>    faces;
    GLuint                      skyboxTexture;
    GLuint                      SkyBoxProgramID;

    SkyBox();
    ~SkyBox();
    void    init();
    GLuint  loadCubemap(std::vector<std::string> faces);
    void    Render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
};


#endif // SKYBOX_H
