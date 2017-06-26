#include "skybox.h"
#include <iostream>
#include <iomanip>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLfloat skyboxVerticesRaw[] =  {
    // Positions
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
};

void SkyBox::init()
{
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices) * 108, skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    // Cubemap (Skybox)
    faces.push_back("../skybox/images/City/posx.jpg");
    faces.push_back("../skybox/images/City/negx.jpg");
    faces.push_back("../skybox/images/City/posy.jpg");
    faces.push_back("../skybox/images/City/negy.jpg");
    faces.push_back("../skybox/images/City/posz.jpg");
    faces.push_back("../skybox/images/City/negz.jpg");
//    faces.push_back("../skybox/images/Skies/posy.jpg");
//    faces.push_back("../skybox/images/Skies/posy.jpg");
//    faces.push_back("../skybox/images/Skies/posy.jpg");
//    faces.push_back("../skybox/images/Skies/posy.jpg");
//    faces.push_back("../skybox/images/Skies/posy.jpg");
//    faces.push_back("../skybox/images/Skies/posy.jpg");
    this->skyboxTexture = loadCubemap(faces);
}

SkyBox::SkyBox()
{
    this->skyboxVertices = skyboxVerticesRaw;

//    std::cout << std::endl << "SkyBox Vertices:" << std::endl << "[ ";
//    std::cout << std::setw(2);
//    for (int i = 0; i < 108; i++)
//    {
//        std::cout << this->skyboxVertices[i] << " ";
//        if (i == 107)    std::cout << " ]" << std::endl;
//        if ((i + 1) % 3 == 0)
//        {
//            std::cout << std::setw(4);
//            std::cout << std::endl;
//        }
//        else     std::cout << std::setw(2);
//        if ((i + 1) % 18 == 0) std::cout << std::endl;
//    }
}

GLuint SkyBox::loadCubemap(std::vector<std::string> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    Magick::Image*  image;
    Magick::Blob    m_blob;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (GLuint i = 0; i < faces.size(); i++)
    {
        try
        {
            image = new Magick::Image(faces[i]);
            image->write(&m_blob, "RGBA");
        }
        catch (Magick::Error& Error)
        {
            std::cout << "Error loading texture '" << faces[i] << "': " << Error.what() << std::endl;
            return false;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image->columns(), image->rows(), -0.5, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
//        if (i == 0)image->display();
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

void SkyBox::Render(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    // Remove any translation component of the view matrix
//    model = glm::rotate(model, 1.57f, glm::vec3(1, 0, 0));
    view = glm::mat4(glm::mat3(view));

    // Draw skybox as last
    glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content

    // Use our SkyBox shader program
    glUseProgram(this->SkyBoxProgramID);

    // Pass in shader view and projection values
    glUniformMatrix4fv(glGetUniformLocation(this->SkyBoxProgramID, "model"),         1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(this->SkyBoxProgramID, "view"),          1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(this->SkyBoxProgramID, "projection"),    1, GL_FALSE, glm::value_ptr(projection));

    // SkyBox cube
    glBindVertexArray(this->skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(this->SkyBoxProgramID, "skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // Set depth function back to default
}
