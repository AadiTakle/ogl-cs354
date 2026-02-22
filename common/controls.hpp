#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
void computeMatricsFromObjectRotation(vec3 objectCenter);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif