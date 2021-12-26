#pragma once
#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GLFW\glfw3.h>
#include <iostream>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed,
		GLfloat startTurnSpeed);

	/**
	 * Input: Moving the Camera
	 * Just need to change camera position!
	 * GLFW: glfwGetKey(window, GLFW_KEY_W)
	 * SDL: Check for event, check if KEYDOWN event, check which key pressed
	 * Then add value to camera position while key held.
	 * Different CPU speeds? Will move fast on some computers, slow on others.
	 * Using delta time to avoid this.
	 */
	void keyControl(bool* keys, GLfloat deltaTime);


	void mouseControl(GLfloat xChange, GLfloat yChange);

	/**
	 * View matrix applied to a vertex will convert it to View Space(Camera Space).
	 * Place values in matrices to calculate view matrix.
	 * GLM uses glm::lookAt(position, target, up) function to calculate this.
	 * @return A view matrix as glm::mat4 type.
	 */
	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;	// camera position
	glm::vec3 front;	// the direction the camera is looking in, the opposite of the view dir.
	glm::vec3 up;	// upwards relative to where camera is facing.
	glm::vec3 right;	// facing right of the camera, defines x axis. From cross(front, up) 
	glm::vec3 worldUp;

	GLfloat yaw;	// Yaw: Looking left and right.
	GLfloat pitch;	// Pitch: Looking up and down.

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	/**
	 * Input: Turning ¨C Pitch and Yaw
	 * Combine the values from pitch and yaw to get front vector with those properties.
	 * x = cos(pitch) x cos(yaw)
	 * y = sin(pitch)
	 * z = cos(pitch) x sin(yaw)
	 * Vector front [x, y, z] will have the given pitch and yaw!
	 * Update Camera direction with new vector
	 * Then calculate right and up using cross product.
	 */
	void update();
};

