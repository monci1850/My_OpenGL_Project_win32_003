#version 330                                                                  
                                                                              
layout (location = 0) in vec3 pos;											  

out vec4 myColor;															  

uniform	mat4 ModelMatrix;
uniform	mat4 ProjectionMatrix;												  
uniform mat4 ViewMatrix;

void main()                                                                   
{                                                                             
    /**
     * Using the View Matrix
     * Bind the View Matrix to a uniform on the shader.
     * Apply it between the projection and model matrices.
     * gl_Position = projection * view * model * vec4(pos, 1.0);
     * Remember: ORDER MATTERS.
     * Multiplying the projection, view and model matrices in a different order will not work
     */
    gl_Position = ProjectionMatrix *ViewMatrix* ModelMatrix * vec4(pos, 1.0);	          
	
    myColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0);							  
}