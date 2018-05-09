#pragma once

#include <glm/glm.hpp>

//! Handles in-game camera (non-VR). 
/*!
Takes care of the correct projection and view matrix for the non-VR version of the camera.
*/
class Camera
{
public:
	Camera(Shader * shader, float w, float h);
	void SetShader(Shader *shader);

	/**
	* Sets the camera translation to a certain absolute value.
	*/
	void SetTranslation(float x, float y, float z);

	/**
	* Sets the camera rotation to a certain absolute value.
	* @param rotation float value of the rotation amount
	* @param  x set to 1 to rotate around x axis
	* @param y set to 1 to rotate around y axis
	* @param z set to 1 to rotate around z axis
	*/
	void SetRotation(float rotation, float x, float y, float z);

	/**
	* Translates the camera by a certain value.
	* Adds to current translation.
	*/
	void Translate(float x, float y, float z);

	/**
	* Rotates the camera by a certain value.
	* Adds to current rotation.
	* @see Camera::SetRotation(float rotation, float x, float y, float z)
	*/
	void Rotate(float rotation, float x, float y, float z);

	/**
	* Updates the view matrix in the shader.
	*/
	void Update();

	/**
	* Gets the eye position.
	* @return The position vector of the camera in the world.
	*/
	glm::vec3 getPosition();
	/**
	* Gets the eye direction.
	* @return The direction vector where the camera is looking.
	*/
	glm::vec3 getEyeDir();

	/**
	* Gets the view matrix.
	* @return View matrix in glm format
	*/
	glm::mat4 getViewMatrix();

	/**
	* Gets the projection matrix.
	* @return Projection matrix in glm format
	*/
	glm::mat4 getProjectionMatrix();

	/**
	* Sets the view matrix to a desired value.
	* @param m matrix in glm format to be set as view
	*/
	void setViewMatrix(glm::mat4 m);

private:
	// p * v * m

	glm::mat4 projMat;
	glm::mat4 viewMat;
	glm::mat4 modelMat = glm::mat4(1.0); //identity

	glm::mat4 translation;
	glm::mat4 rotation;

	Shader * shader;

	GLint vUniform;
	GLint pUniform;

	void UpdateViewMatrix();
};