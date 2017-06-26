// Include standard headers
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <ctime>

// Include GLEW
#include <GL/glew.h>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>

// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include "shader.h"
#include "texture.h"
#include "libobj.h"
#include "skybox/skybox.h"

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 720

const char* texture_path;
const char* model_path;
const char* skybox_path;

GLMmodel*   objModel;

GLuint      programID;
GLuint      modelID;
GLuint      viewID;
GLuint      projectionID;


glm::mat4   model;
glm::mat4   view;
glm::mat4   projection;

Texture*    pTextures = NULL;
GLuint      gSampler;

GLuint      vertexbuffer;
GLuint      normalbuffer;

int         image_count = 0;

SkyBox* Sky_Box;
ofstream file;

// Rotation interface
float   xrot    = 0;
float   yrot    = 0;
float   xdiff   = 0;
float   ydiff   = 0;
float   zoom    = 3;
float   view_x  = 0;
float   view_y  = 0;
bool    mouseDown = false;
float   rot     = 0;


void mouseMotionCB(int x, int y)
{
    if (mouseDown)
    {
        yrot =  0.01 * (x - xdiff);
        xrot =  0.01 * (y + ydiff);
        glutPostRedisplay();
    }
}
void SaveScene()
{
    file.open ("../ForSegUltra/IMAGE_LIST.txt", std::ios_base::app);
    // Make the BYTE array, factor of 3 because it's RBG.
    unsigned char* pixels = new unsigned char[3 * WINDOW_WIDTH * WINDOW_HEIGHT];
    // Read pixwls from OpenGL scene
    glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    // Make Blob and Image
    Magick::Blob    blob(&pixels[0], WINDOW_WIDTH * WINDOW_HEIGHT * 3);
    Magick::Image   img;
    // Setup image parameters
    img.size(to_string(WINDOW_WIDTH) + "x" + to_string(WINDOW_HEIGHT));
    img.magick("RGB");
    img.depth(8);

    try{
        img.read(blob);
    }
    catch(std::exception e)
    {
        return;
    }

    img.flip();
    img.magick( "PNG" );
    img.type(Magick::GrayscaleType);
//    img.threshold(128);

    // Fill file name
    std::string image_name = "../ForSegUltra/images/abrams_" + std::to_string(image_count) + ".png";
//    file << "images/support_" << image_count << ".png " << 3 << std::endl;
    file << "images/abrams_" << image_count << ".png " << " labels/0/abrams_" << image_count << ".png " << std::endl;
//     Write original image
    img.write(image_name.data());
    image_count++;

    delete [] pixels;
    file.close();
    return;
}


void mouseCB(int button, int state, int x, int y)
{
    switch(button){
    case GLUT_LEFT_BUTTON:
        switch(state)
        {
        case GLUT_DOWN:
            mouseDown = true;
            xdiff = x - yrot;
            ydiff = -y + xrot;
            break;

        case GLUT_UP:
            mouseDown = false;
            break;

        default:
            break;
        }
        break;

    case GLUT_MIDDLE_BUTTON:
        break;

    case 3:
        //        printf("%s", "Wheel up\n");
        zoom -= 0.1;
        view = glm::lookAt(   glm::vec3(view_x, view_y, zoom),   // Camera position in World Space
                              glm::vec3(view_x, view_y, 0),     // and looks at the origin
                              glm::vec3(0, 1, 0));    // Head is up (set to 0,-1,0 to look upside-down)
        glutPostRedisplay();
        break;

    case 4:
        //        printf("%s", "Wheel down\n");
        zoom += 0.1;
        view = glm::lookAt(   glm::vec3(view_x, view_y, zoom),   // Camera position in World Space
                              glm::vec3(view_x, view_y, 0),     // and looks at the origin
                              glm::vec3(0, 1, 0));    // Head is up (set to 0,-1,0 to look upside-down)
        glutPostRedisplay();
        break;

    default:
        break;
    }
}

void specialKeyboardCB(int key, int x, int y)
{
    switch(key){
    case GLUT_KEY_LEFT:
        yrot -= 0.05;
        break;

    case GLUT_KEY_RIGHT:
        yrot += 0.05;
        break;

    case GLUT_KEY_UP:
        xrot += 0.05;
        break;

    case GLUT_KEY_DOWN:
        xrot -= 0.05;
        break;

    default:
        break;
    }
    glutPostRedisplay();
    x = x;
    y = y;
}

void keyboardCB(unsigned char key, int x, int y)
{
    switch(key){
    case 27:
        exit(0);
        break;
    case 'w':
        view_y -= 0.03;
        view = glm::lookAt(   glm::vec3(view_x, view_y, zoom),   // Camera position in World Space
                              glm::vec3(view_x, view_y, 0),     // and looks at the origin
                              glm::vec3(0, 1, 0));    // Head is up (set to 0,-1,0 to look upside-down)
        glutPostRedisplay();
        break;
    case 'a':
        view_x += 0.03;
        view = glm::lookAt(   glm::vec3(view_x, view_y, zoom),   // Camera position in World Space
                              glm::vec3(view_x, view_y, 0),     // and looks at the origin
                              glm::vec3(0, 1, 0));    // Head is up (set to 0,-1,0 to look upside-down)
        glutPostRedisplay();
        break;
    case 's':
        view_y += 0.03;
        view = glm::lookAt(   glm::vec3(view_x, view_y, zoom),   // Camera position in World Space
                              glm::vec3(view_x, view_y, 0),     // and looks at the origin
                              glm::vec3(0, 1, 0));    // Head is up (set to 0,-1,0 to look upside-down)
        glutPostRedisplay();
        break;
    case 'd':
        view_x -= 0.03;
        view = glm::lookAt(   glm::vec3(view_x, view_y, zoom),   // Camera position in World Space
                              glm::vec3(view_x, view_y, 0),     // and looks at the origin
                              glm::vec3(0, 1, 0));    // Head is up (set to 0,-1,0 to look upside-down)
        glutPostRedisplay();
        break;
    case 'f':
        SaveScene();
        break;
    default: break;
    }

    x = x;
    y = y;
}

void init(const char* fname)
{
    std::cout << std::endl << "Loading .obj model: " << fname;
    objModel = glmReadOBJ(fname);
    if (!objModel)
    {
        std::cout << " - FAIL ON LOAD" << std::endl;
        exit(0);
    }
    else std::cout << " - LOADED" << std::endl;

    // Normilize vertices
    glmUnitize(objModel);
    // Compute facet normals
    glmFacetNormals(objModel);
    // Comput vertex normals
    glmVertexNormals(objModel, 90.0);
    // Load the model (vertices and normals) into a vertex buffer
    glmLoadInVBO(objModel);

    std::cout << std::endl << "Loading textures for model: " << fname << std::endl;

    // Load textures into array. Need to put path to folder with textures
    pTextures = new Texture[objModel->numgroups];
    LoadTextures(objModel, pTextures, texture_path);

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    std::cout << "Loading shader program for model: " << fname << std::endl << std::endl;

    // Create and compile our GLSL program from the shaders
    programID       = LoadShaders( "../shaders/vertShader.glsl",
                                   "../shaders/fragShader.glsl" );

    std::cout << std::endl;

    // Get a handle for our model, view and projection uniforms
    modelID       = glGetUniformLocation(programID, "model");
    viewID        = glGetUniformLocation(programID, "view");
    projectionID  = glGetUniformLocation(programID, "projection");

    glm::vec4 light_ambient   = glm::vec4 ( 0.1, 0.1, 0.1, 0.5 );
    glm::vec4 light_diffuse   = glm::vec4 ( 0.8, 1.0, 1.0, 1.0 );
    glm::vec4 light_specular  = glm::vec4 ( 0.8, 1.0, 1.0, 1.0 );

    glUseProgram (programID);
    glUniform4fv (glGetUniformLocation(programID, "light_ambient"),   1, &light_ambient[0]);
    glUniform4fv (glGetUniformLocation(programID, "light_diffuse"),   1, &light_diffuse[0]);
    glUniform4fv (glGetUniformLocation(programID, "light_specular"),  1, &light_specular[0]);

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    projection = glm::perspective(45.0f, 16.0f / 9.0f, 5.0f, 500.0f);

    // Camera matrix
    view = glm::lookAt(   glm::vec3(view_x, view_y, zoom),   // Camera position in World Space
                          glm::vec3(0, 0, 0),     // and looks at the origin
                          glm::vec3(0, 1, 0));    // Head is up (set to 0,-1,0 to look upside-down)

    // Model matrix : an identity matrix (model will be at the origin)
    model = glm::mat4(.0f);

    // Initialize a light
    glm::vec4 lightPosition = glm::vec4(-20, -10, 0, 0);
    glUniform4fv( glGetUniformLocation(programID, "lightPos"),1, &lightPosition[0]);

    std::cout << "Groups and materials of model: " << fname << std::endl << std::endl;

    glmDisplayVBOMaterials(objModel);

    std::cout << "Load SkyBox with SkyBoxShader: " << fname << std::endl << std::endl;

    Sky_Box = new SkyBox();
    Sky_Box->init();
    Sky_Box->SkyBoxProgramID = LoadShaders( "../skybox/shaders/vertSkyBox.glsl",
                                            "../skybox/shaders/fragSkyBox.glsl" );

    std::cout << std::endl << "DISPLAYING RESULT...\n"
                              "1) Use WASD for translation; \n"
                              "2) Use ARROW_KEYS or MOUSE for rotation; \n"
                              "3) Use MOUSE_WHEEL for scaling\n"
                              "4) Use F for save render \n"
                              "5) Injoy the result" << std::endl ;
}

void display(void)
{
    // Clear the screne
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model = glm::mat4(1.0f);
    glm::mat4 xRotMat = glm::rotate(glm::mat4(1.0f), xrot, glm::normalize(glm::vec3(glm::inverse(model) * glm::vec4(1, 0, 0, 1))) );
    model = model * xRotMat;
    glm::mat4 yRotMat = glm::rotate(glm::mat4(1.0f), yrot, glm::normalize(glm::vec3(glm::inverse(model) * glm::vec4(0, 1, 0, 1))) );
    model = model * yRotMat;

    glm::mat4 modelViewMatrix = view * model;
    glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix)); // Normal Matrix

    // Use our shader
    glUseProgram(programID);
    // Send the model, view and projection matrices to the shader
    glUniformMatrix4fv(modelID,         1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(viewID,          1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionID,    1, GL_FALSE, &projection[0][0]);

    glUniformMatrix3fv( glGetUniformLocation(programID, "normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

    glmDrawVBO(objModel, pTextures,  programID);
    Sky_Box->Render(model, view, projection);
    // Swap buffers
    glutSwapBuffers();

//    for (int j = 0; j <= 4; j++ )
//    {
//        for (int i = 0; i < 20; i++ )
//        {
//            float zoom_zoom = zoom;
//            for (int k = 0; k <= 5; k++ )
//            {
//                // Clear the screne
//                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//                model = glm::mat4(1.0f);
//                glm::mat4 xRotMat = glm::rotate(glm::mat4(1.0f), xrot /*- 1.57f*/, glm::normalize(glm::vec3(glm::inverse(model) * glm::vec4(1, 0, 0, 1))) );
//                model = model * xRotMat;
//                glm::mat4 yRotMat = glm::rotate(glm::mat4(1.0f), yrot, glm::normalize(glm::vec3(glm::inverse(model) * glm::vec4(0, 1, 0, 1))) );
//                model = model * yRotMat;

//                glm::mat4 modelViewMatrix = view * model;
//                glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix)); // Normal Matrix

//                // Use our shader
//                glUseProgram(programID);

//                // Send the model, view and projection matrices to the shader
//                glUniformMatrix4fv(modelID,         1, GL_FALSE, &model[0][0]);
//                glUniformMatrix4fv(viewID,          1, GL_FALSE, &view[0][0]);
//                glUniformMatrix4fv(projectionID,    1, GL_FALSE, &projection[0][0]);

//                glUniformMatrix3fv( glGetUniformLocation(programID, "normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

//                glmDrawVBO(objModel, pTextures,  programID);
//                Sky_Box->Render(model, view, projection);
////                // Swap buffers
//                glutSwapBuffers();

//                view = glm::lookAt(   glm::vec3(view_x, view_y, zoom_zoom),  // Camera position in World Space
//                                      glm::vec3(0, -0.5, 0),               // and looks at the origin
////                                      glm::vec3(rot, rot, rot));              // Head is up (set to 0,-1,0 to look upside-down)
//                                      glm::vec3(0, 1, 0));              // Head is up (set to 0,-1,0 to look upside-down)
//                SaveScene();
//                rot += 0.5;
//                zoom_zoom -= 0.8;
//                std::cout << "Progress: [" << std::setw(8) << std::left << (float) (image_count) * 100/ (100 * 6) << "]%" << std::endl;
//            }

//            yrot += 0.4;
//        }
//        view_y += 1;
//    }
}

// Get OpenGL version information
void getGLinfo()
{
    std::cout << "GL Vendor   : " << glGetString(GL_VENDOR)     << std::endl;
    std::cout << "GL Renderer : " << glGetString(GL_RENDERER)   << std::endl;
    std::cout << "GL Version  : " << glGetString(GL_VERSION)    << std::endl;
}

void reshape( int width, int height )
{
    glViewport(0, 0, width, height);
    GLfloat aspectRatio = GLfloat(width)/height;
    projection = glm::perspective(45.0f, aspectRatio, 0.1f, 500.0f);
    glutPostRedisplay();
}

int main(  int argc, char **argv  )
{
    texture_path    = "../Models/T-90/";
    model_path      = "../Models/T-90/T-90.obj";
    glutInitContextVersion (3, 3);
    glewExperimental = GL_TRUE;
//    //  TODO: Command-line parsing and error checking
//    if (argc == 2)
//    {
//        model_path = argv[1];
//    }
//    else
//    {
//        std::cerr << "ERROR: You have to specify an OBJ file as first argument.\n"
//                     "For example \"./Support/Support.obj\"" << std::endl;
//        return 1;
//    }

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    glutCreateWindow( "objview" );
    glewInit();
    getGLinfo();
    init(model_path);

    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);

    glutSpecialFunc(specialKeyboardCB);
    glutKeyboardFunc(keyboardCB);

    glutMainLoop();
    return 0;
}
