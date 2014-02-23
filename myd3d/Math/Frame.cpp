#include "Frame.h"

#include <math.h>
#include <iostream>

#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/matrix_access.hpp"
#include "../glm/gtx/rotate_vector.hpp"


Frame::Frame(void)
	:	m_Matrix(1.0f), m_Position(glm::vec3(0.0f, 0.0f, 0.0f)), m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
        m_Forward(glm::vec3(0.0f, 0.0f, 1.0f)), m_Rotation(1.0f), m_Scale(1.0f, 1.0f, 1.0f)
{
}

Frame::Frame(const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up)
	: m_Matrix(1.0f), m_Position(position), m_Up(up), m_Forward(forward), m_Rotation(1.0f),
      m_Scale(1.0f, 1.0f, 1.0f)
{
}


Frame::~Frame(void)
{
}


const glm::mat4 Frame::GetMatrix() const
{
	glm::vec3 xAxis = glm::cross(m_Up, m_Forward);

	glm::mat4 returnMatrix;

	returnMatrix = glm::column(returnMatrix, 0, glm::vec4(xAxis, 0.0f));

	returnMatrix = glm::column(returnMatrix, 1, glm::vec4(m_Up, 0.0f));

	returnMatrix = glm::column(returnMatrix, 2, glm::vec4(m_Forward, 0.0f));

	returnMatrix = glm::column(returnMatrix, 3, glm::vec4(m_Position, 1.0f));

    returnMatrix = glm::scale(returnMatrix, m_Scale);

	return returnMatrix;
}


// Move along the local z axis.
void Frame::MoveForward(float delta)
{
	m_Position += m_Forward * delta;
}

// Move along the local Y Axis
void Frame::MoveUp(float delta)
{
	m_Position += m_Up * delta;

}

// Move along the local X Axis
void Frame::MoveRight(float delta)
{
	glm::vec3 cross	 = glm::cross(m_Forward, m_Up); //calculate x axis
	m_Position		+= cross * delta;
}


void Frame::MoveGlobalX(float delta)
{
    m_Position += glm::vec3(delta, 0.0f, 0.0f);
}

void Frame::MoveGlobalY(float delta)
{
    m_Position += glm::vec3(0.0f, delta, 0.0f);
}

void Frame::MoveGlobalZ(float delta)
{
    m_Position += glm::vec3(0.0f, 0.0f, delta);
}


// Rotates around the current X axis [cross(m_Up, m_Forward)].
void Frame::RotateLocalX(const float delta)
{
	// Calculate X axis.
	glm::vec3 cross = glm::normalize(glm::cross(m_Up, m_Forward)); 

	// Create rotation matrix
	glm::mat4 Rotation	= glm::rotate(glm::mat4(1.0f), delta, cross);

	// Rotate forward and up vectors by new rotation.
	m_Forward			= glm::normalize(
		glm::vec3(Rotation * glm::vec4(m_Forward, 0.0f))); 
	m_Up				= glm::normalize(
		glm::vec3(Rotation * glm::vec4(m_Up, 0.0f))); 
}

// Rotates around the current up axis (m_Up).
void Frame::RotateLocalY(const float delta)
{
	glm::mat4 Rotation	= glm::rotate(glm::mat4(1.0f), delta,  m_Up);	
	
	m_Forward			= glm::normalize(
		glm::vec3(Rotation * glm::vec4(m_Forward, 0.0f)));
}

// Rotates around the local z axis defined by m_Forward.
void Frame::RotateLocalZ(const float delta)
{
	// Z axis is current forward vector.
	glm::mat4 Rotation	= glm::rotate(glm::mat4(1.0f), delta, m_Forward);
	m_Up				= glm::normalize(
		glm::vec3(Rotation * glm::vec4(m_Up, 0.0f)));
}

// Rotates around the global x axis [1.0, 0.0, 0.0].
void Frame::RotateGlobalX(const float delta)
{
	glm::mat4 Rotation	= glm::rotate(glm::mat4(1.0f), delta, glm::vec3(1.0, 0.0, 0.0));

	m_Up				= glm::normalize(glm::vec3(Rotation * glm::vec4(m_Up, 0.0f)));
	m_Forward			= glm::normalize(glm::vec3(Rotation * glm::vec4(m_Forward, 0.0f)));
}

// Rotates around the global Y axis  (0.0, 1.0, 0.0).
void Frame::RotateGlobalY(const float delta)
{
	glm::mat4 Rotation	= glm::rotate(glm::mat4(1.0f), delta,  glm::vec3(0.0f, 1.0f, 0.0f));	

	m_Forward			= glm::normalize(glm::vec3(Rotation * glm::vec4(m_Forward, 0.0f))); 
	m_Up				= glm::normalize(glm::vec3(Rotation * glm::vec4(m_Up, 0.0f))); 
}

// Rotates around the global z axis [0.0, 0.0, 1.0]
void Frame::RotateGlobalZ(const float delta)
{
	glm::mat4 Rotation	= glm::rotate(glm::mat4(1.0f), delta,  glm::vec3(0.0f, 0.0f, 1.0f));	

	m_Forward			= glm::normalize(glm::vec3(Rotation * glm::vec4(m_Forward, 0.0f))); 
	m_Up				= glm::normalize(glm::vec3(Rotation * glm::vec4(m_Up, 0.0f))); 
}
