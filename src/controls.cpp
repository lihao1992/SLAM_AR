
#include <GL/glew.h>
// include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window;

// include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../include/OPENGL/controls.h"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 ModelMatrix;
glm::mat4 TranslationMatrix;
glm::mat4 RotationMatrix;
glm::mat4 ScalingMatrix;

glm::mat4 getModelMatrix(){
    return ModelMatrix;
}
glm::mat4 getProjectionMatrix(){
    return ProjectionMatrix;
}


float scale_factor = 1;
float rotation = 0;

float initialFOV = 45.0f;

float speed = 3.0f;
float rotate_speed = 90.0f;


void computeMatricesFromInputs(){

    static double lastTime = glfwGetTime();

    double currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;


        // Move backward
        if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
                scale_factor += deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
                scale_factor -= deltaTime * speed; 
        }
        // Strafe right
        if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
                rotation += deltaTime * rotate_speed;
        }
        // Strafe left
        if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        rotation -= deltaTime * rotate_speed; 
        }
    
    float FoV = initialFOV;

    // ProjectionMatrix 
    ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f/3.0f, 0.1f, 100.0f);

    // ModelMatrix
    RotationMatrix = glm::rotate(RotationMatrix, glm::radians(rotation), glm::vec3(0, 1, 0));
    TranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.5, 3, 0));
    ScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor));

    ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;

    lastTime = currentTime;

}
