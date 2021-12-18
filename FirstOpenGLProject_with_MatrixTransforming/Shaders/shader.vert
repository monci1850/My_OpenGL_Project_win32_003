#version 330                                                                  
                                                                              
layout (location = 0) in vec3 pos;											  
out vec4 myColor;															  
uniform	mat4 ModelMatrix;													  
uniform	mat4 ProjectionMatrix;												  
void main()                                                                   
{                                                                             
    gl_Position = ProjectionMatrix * ModelMatrix * vec4(pos, 1.0);	          
	myColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0);							  
}