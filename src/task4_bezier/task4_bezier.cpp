// Task 4 - Render the letter R using OpenGL
//        - Implement a function to generate bezier curve points
//        - Generate multiple points on each bezier curve
//        - Draw lines connecting the generated points using OpenGL

#include <iostream>
#include <vector>

#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

const unsigned int SIZE = 512;

using namespace std;
using namespace glm;
using namespace ppgso;

class BezierWindow : public Window {
private:

    // Control points for the bezier curves
    // First curve is 4 control points
    // Rest of the curves are 3 control points, each reusing the last curve end
    // Defines the letter "R"
    vector<vec2> controlPoints = {
            { 0,  -1},
            { 0, -.3},
            { 0,  .3},
            { 0,   1},
            {.3,   1},
            {.5,   1},
            {.5,  .5},
            {.5,   0},
            {.3,   0},
            { 0,   0},
            {.3, -.3},
            {.5, -.5},
            {.5,  -1},
    };

    vector<vec2> omegaPoints = {
            { -0.3, 0},
            { -0.15, 0},
            { -0.15, 0},
            { -0.1, 0},
            {-0.5, 0.5},
            { 0.5, 0.5},
            { 0.1, 0},
            { 0.15, 0},
            { 0.15, 0},
            { 0.3, 0},
    };

    vec2 control[] ={
            { 0,  -1},
            { 0, -.3},
            { 0,  .3},
            { 0,   1},
            {.3,   1},
            {.5,   1},
            {.5,  .5},
            {.5,   0},
            {.3,   0},
            { 0,   0},
            {.3, -.3},
            {.5, -.5},
            {.5,  -1},
    };

    // This will hold the bezier curve geometry once we generate it
    vector<vec3> points;

    // GLSL Program to use for rendering
    Shader program = {color_vert_glsl, color_frag_glsl};

    // These numbers are used to pass buffer data to OpenGL
    GLuint vao = 0, vbo = 0;

    int getPt( int n1 , int n2 , float perc )
    {
        int diff = n2 - n1;

        return n1 + ( diff * perc );
    }
    // Compute points for Bezier curve using 4 control points
    vec2 bezierPoint(const vec2 &p0, const vec2 &p1, const vec2 &p2, const vec2 &p3, const float u) {
        // TODO: Compute point on the Bezier curve
        double x = 0,y = 0;
        vec2 temp(x,y);

        temp.x = (double) pow(1-u,3)*p0.x+3*u*pow(1-u,2)*p1.x+3*pow(u,2)*(1-u)*p2.x
                 +pow(u,3)*p3.x;
        temp.y = pow(1-u,3)*p0.y+3*u*pow(1-u,2)*p1.y+3*pow(u,2)*(1-u)*p2.y
                 +pow(u,3)*p3.y;

        return temp;
    }

    vec2 getBezierPoint( vec2* points, int numPoints, float t ) {
        vec2* tmp = new vec2[numPoints];
        memcpy(tmp, points, numPoints * sizeof(vec2));
        int i = numPoints - 1;
        while (i > 0) {
            for (int k = 0; k < i; k++)
                tmp[k] = tmp[k] + t * ( tmp[k+1] - tmp[k] );
            i--;
        }
        vec2 answer = tmp[0];
        delete[] tmp;
        return answer;
    }

     int Factorial(int x) {
        return (x == 1 ? x : x * Factorial(x - 1));
    }
    // Compute points for a sequence of Bezier curves defined by a vector of control points
    // Each bezier curve will reuse the end point of the previous curve
    // count - Number of points to generate on each curve
    void bezierShape(int count) {
        for(int i =0; i < count; i++)
        {
            vec2 temp = getBezierPoint(control, controlPoints.size(), i);
            points.emplace_back(temp, 0);
        }

//        for(int j = 0 ; j < (int)omegaPoints.size()-1; j+=3) {
//            for (int i = 0; i <= count; i++) {
//                float u = (float)i/count;
//                vec2 point = bezierPoint(omegaPoints[j], omegaPoints[j+1], omegaPoints[j+2],
//                                         omegaPoints[j+3], u);
//                points.emplace_back(point, 0);
//                //points.push_back(point);
//            }
//        }


    }

public:
    BezierWindow() : Window{"task4_bezier", SIZE, SIZE} {
        // Generate Bezier curve points
        bezierShape(486);

        // Generate a vertex array object
        // This keeps track of what attributes are associated with buffers
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Generate a vertex buffer object, this will feed data to the vertex shader
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // TODO: Pass the control points to the GPU
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), &points[0], GL_STATIC_DRAW);

        // Setup vertex array lookup, this tells the shader how to pick data for the "Position" input
        auto position_attrib = program.getAttribLocation("Position");
        glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(position_attrib);

        // Set model matrix to identity
        program.setUniform("ModelMatrix", mat4{});
        program.setUniform("ViewMatrix", mat4{});
        program.setUniform("ProjectionMatrix", mat4{});

        // Set the color uniform
        program.setUniform("OverallColor", vec3{1.0f,1.0f,1.0f});
    }

    ~BezierWindow() final {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void onIdle() final {
        // Set gray background
        glClearColor(0.5f,0.5f,0.5f,0);

        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw shape
        glBindVertexArray(vao);

        // TODO: Define the correct render mode
        glDrawArrays(GL_POINTS, 0, points.size());
    }
};

int main() {
    // Create our window
    BezierWindow window;

    // Main execution loop
    while (window.pollEvents()) {

    }

    return EXIT_SUCCESS;
}
