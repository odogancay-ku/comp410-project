//
// Created by ofaru on 12.03.2024.
//

#include "drawer.h"

void drawRectangle(const vec3& point1, const vec3& point2, const vec3& color)
{
    GLfloat vertices[] = {
            point1.x, point1.y, point1.z, // bottom-left corner
            point2.x, point1.y, point1.z, // bottom-right corner
            point2.x, point2.y, point2.z, // top-right corner
            point1.x, point2.y, point2.z  // top-left corner
    };

    // Set the uniform objectColor
    GLint objectColorLoc = glGetUniformLocation(programID, "objectColor");
    glUniform3f(objectColorLoc, color.x, color.y, color.z);

    // Set the uniform translation
    GLint translationLoc = glGetUniformLocation(programID, "translation");
    glUniform3f(translationLoc, 0.0f, 0.0f, 0.0f);

    // Draw the rectangle
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; ++i)
    {
        glVertex3f(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
    }
    glEnd();
}
