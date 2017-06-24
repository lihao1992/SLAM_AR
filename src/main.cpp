#include <iostream>
#include <stdio.h>
#include <vector>

#include <opencv2/opencv.hpp>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;
using namespace cv;

#include "../include/OPENGL/shader.h"
#include "../include/OPENGL/controls.h"
#include "../include/orb_slam.h"

cv::VideoCapture capture(0);

int main(){

    capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    bool success = true;

    cv::Mat R, tvec;

    //cv::Mat K = getCameraMatrix();

    ORB_SLAM2::System SLAM("Vocabulary/ORBvoc.bin", "../TUM_USB.yaml", ORBSLAM2::System::MONOCULAR, false);

    cout << endl << "---------------" << endl;
    cout << "Start to process video ..." << endl;

    cv::Mat img;
    cv::Mat Tcw;

    bool slamMode = 0;

    Ptr<ORB> orb = ORB::create();
    orb->setScoreType(cv::ORB::FAST_SCORE);
    orb->setMaxFeatures(1000);
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming(2)");

    if(!success)
        return 0;
    
    // Initialise GLFW
    if( !glfwInit() )
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;;
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "ORBSLAM-AR", NULL, NULL);
    if(window == NULL){

        cerr << "Failed to open GLFW window." << endl;
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    cout << "OpenGL version supported by this plantform" << glGetString(GL_VERSION) << endl;



    // Initialize GLEW
    glewExperimental = GL_TRUE; // Needed for core profile

    if (glewInit() != GLEW_OK) {

        cerr << "Failed to initialize GLEW" <<endl;
        getchar();
        glfwTerminate();
        return -1;
    }

    // ensure we can capture key for escape
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);  

    // Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    
    GLuint programID = LoadShaders("../Shaders/simpleTransform.vertexshader", "../Shaders/color.fragmentshader");

    // get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = { 
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

    GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


    

    do{

        

        capture.read(img);

        if(!img.empty()){
            
            // Clear the screen
		    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		    // Use our shader
		    glUseProgram(programID);

            if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                slamMode = true;

            success = orb_tracker.process(img, slamMode);

            if(!success){
                glfwPollEvents();

                if(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)&&
                    glfwWindowShouldClose(window) == 0{
                        glfwSwapBuffers(window);
                        continue;
                }
                else
                    break;
            }

            glm::mat4 ViewMatrix;

            if(!slamMode)
                ViewMatrix = getViewMatrix(slamMode);
            else{

                cv::Mat CameraPose = SLAM.TrackMonocular(img, 1);

                if(CameraPose.empty()){
                    return 0;
                }

                CameraPose.rowRange(0, 3).colRange(0, 3).copyTo(R);
                CameraPose.rowRange(0, 3).col(3).copyTo(tvec);

                ViewMatrix = getViewMatrix(slamMode);
            }

            computeMatricesFromInputs();

            glm::mat4 ProjectionMatrix = getProjectionMatrix();

            glm::mat4 ModelMatrix = getModelMatrix();

            MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            glEnable(GL_DEPTH_TEST);

		    // Send our transformation to the currently bound shader, 
		    // in the "MVP" uniform
		    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		    // 1rst attribute buffer : vertices
		    glEnableVertexAttribArray(0);
		    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		    glVertexAttribPointer(
			    0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			    3,                  // size
			    GL_FLOAT,           // type
			    GL_FALSE,           // normalized?
			    0,                  // stride
			    (void*)0            // array buffer offset
		    );

		    // 2nd attribute buffer : colors
		    glEnableVertexAttribArray(1);
		    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		    glVertexAttribPointer(
			    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			    3,                                // size
			    GL_FLOAT,                         // type
			    GL_FALSE,                         // normalized?
			    0,                                // stride
			    (void*)0                          // array buffer offset
		    );

		    // Draw the triangle !
		    glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

		    glDisableVertexAttribArray(0);
		    glDisableVertexAttribArray(1);

		    // Swap buffers
		    glfwSwapBuffers(window);
		    glfwPollEvents();
        }

		

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

    SLAM.Shutdown();

    glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}