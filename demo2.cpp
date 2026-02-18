/******************************************************************************
 * This demo draws a somewhat familiar geometric object composed of three
 * triangles.
 *
 * The main differences between this demo and the previous demo are:
 * - Each vertex now defines two subcomponents: position and color.
 * - A uniform variable is introduced to the shader program, which is used to
 *   accomplish a glowing effect.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE  "Hello Trifxxxe"
GLFWwindow *pWindow;

// define a vertex array to hold our vertices
float vertices[] =
{
    // position (x, y, z)    color (r, g, b)
    -0.40f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //0
    -0.50f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //1
    -0.40f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //2
    -0.50f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //3

    -0.40f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //4
    -0.50f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //5
    -0.40f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //6
    -0.50f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //7

    -0.40f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //8
    -0.50f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //9
    -0.40f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //10
    -0.50f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //11

    -0.40f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //12
    -0.50f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //13
    -0.40f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //14
    -0.50f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //15

    // c2
    -0.25f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //16
    -0.35f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //17
    -0.25f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //18
    -0.35f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //19

    -0.25f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //20
    -0.35f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //21
    -0.25f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //22
    -0.35f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //23

    -0.25f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //24
    -0.35f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //25
    -0.25f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //26
    -0.35f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //27

    -0.25f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //28
    -0.35f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //29
    -0.25f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //30
    -0.35f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //31

    -0.25f,  0.65f,  0.00f,  1.0f, 1.0f, 0.0f,  //32
    -0.35f,  0.60f,  0.00f,  1.0f, 1.0f, 0.0f,  //33
    -0.25f,  0.60f,  0.00f,  1.0f, 1.0f, 0.0f,  //34
    -0.35f,  0.65f,  0.00f,  1.0f, 1.0f, 0.0f,  //35

    -0.25f,  0.75f,  0.00f,  1.0f, 1.0f, 0.0f,  //36
    -0.35f,  0.70f,  0.00f,  1.0f, 1.0f, 0.0f,  //37
    -0.25f,  0.70f,  0.00f,  1.0f, 1.0f, 0.0f,  //38
    -0.35f,  0.75f,  0.00f,  1.0f, 1.0f, 0.0f,  //39

    -0.25f,  0.85f,  0.00f,  1.0f, 1.0f, 0.0f,  //40
    -0.35f,  0.80f,  0.00f,  1.0f, 1.0f, 0.0f,  //41
    -0.25f,  0.80f,  0.00f,  1.0f, 1.0f, 0.0f,  //42
    -0.35f,  0.85f,  0.00f,  1.0f, 1.0f, 0.0f,  //43

    -0.25f,  0.95f,  0.00f,  1.0f, 1.0f, 0.0f,  //44
    -0.35f,  0.90f,  0.00f,  1.0f, 1.0f, 0.0f,  //45
    -0.25f,  0.90f,  0.00f,  1.0f, 1.0f, 0.0f,  //46
    -0.35f,  0.95f,  0.00f,  1.0f, 1.0f, 0.0f,  //47

        //c3
    -0.10f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //48
    -0.20f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //49
    -0.10f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //50
    -0.20f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //51

    -0.10f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //52
    -0.20f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //53
    -0.10f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //54
    -0.20f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //55

    -0.10f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //56
    -0.20f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //57
    -0.10f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //58
    -0.20f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //59

    -0.10f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //60
    -0.20f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //61
    -0.10f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //62
    -0.20f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //63

    -0.10f,  0.65f,  0.00f,  1.0f, 1.0f, 0.0f,  //64
    -0.20f,  0.60f,  0.00f,  1.0f, 1.0f, 0.0f,  //65
    -0.10f,  0.60f,  0.00f,  1.0f, 1.0f, 0.0f,  //66
    -0.20f,  0.65f,  0.00f,  1.0f, 1.0f, 0.0f,  //67

    -0.10f,  0.75f,  0.00f,  1.0f, 1.0f, 0.0f,  //68
    -0.20f,  0.70f,  0.00f,  1.0f, 1.0f, 0.0f,  //69
    -0.10f,  0.70f,  0.00f,  1.0f, 1.0f, 0.0f,  //70
    -0.20f,  0.75f,  0.00f,  1.0f, 1.0f, 0.0f,  //71

    //c4
     0.05f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //72
    -0.05f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //73
     0.05f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //74
    -0.05f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //75

     0.05f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //76
    -0.05f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //77
     0.05f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //78
    -0.05f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //79

     0.05f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //80
    -0.05f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //81
     0.05f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //82
    -0.05f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //83

     0.05f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //84
    -0.05f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //85
     0.05f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //86
    -0.05f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //87

     0.05f,  0.65f,  0.00f,  1.0f, 1.0f, 0.0f,  //88
    -0.05f,  0.60f,  0.00f,  1.0f, 1.0f, 0.0f,  //89
     0.05f,  0.60f,  0.00f,  1.0f, 1.0f, 0.0f,  //90
    -0.05f,  0.65f,  0.00f,  1.0f, 1.0f, 0.0f,  //91

     0.05f,  0.75f,  0.00f,  1.0f, 1.0f, 0.0f,  //92
    -0.05f,  0.70f,  0.00f,  1.0f, 1.0f, 0.0f,  //93
     0.05f,  0.70f,  0.00f,  1.0f, 1.0f, 0.0f,  //94
    -0.05f,  0.75f,  0.00f,  1.0f, 1.0f, 0.0f,  //95

     0.05f,  0.85f,  0.00f,  1.0f, 1.0f, 0.0f,  //96
    -0.05f,  0.80f,  0.00f,  1.0f, 1.0f, 0.0f,  //97
     0.05f,  0.80f,  0.00f,  1.0f, 1.0f, 0.0f,  //98
    -0.05f,  0.85f,  0.00f,  1.0f, 1.0f, 0.0f,  //99

    //c5
    0.20f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //100
    0.10f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //101
    0.20f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //102
    0.10f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //103

    0.20f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //104
    0.10f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //105
    0.20f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //106
    0.10f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //107

    0.20f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //108
    0.10f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //109
    0.20f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //110
    0.10f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //111

    0.20f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //112
    0.10f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //113
    0.20f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //114
    0.10f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //115

    0.20f,  0.65f,  0.00f,  1.0f, 1.0f, 0.0f,  //116
    0.10f,  0.60f,  0.00f,  1.0f, 1.0f, 0.0f,  //117
    0.20f,  0.60f,  0.00f,  1.0f, 1.0f, 0.0f,  //118
    0.10f,  0.65f,  0.00f,  1.0f, 1.0f, 0.0f,  //119

    0.20f,  0.75f,  0.00f,  1.0f, 1.0f, 0.0f,  //120
    0.10f,  0.70f,  0.00f,  1.0f, 1.0f, 0.0f,  //121
    0.20f,  0.70f,  0.00f,  1.0f, 1.0f, 0.0f,  //122
    0.10f,  0.75f,  0.00f,  1.0f, 1.0f, 0.0f,  //123

    0.20f,  0.85f,  0.00f,  1.0f, 1.0f, 0.0f,  //124
    0.10f,  0.80f,  0.00f,  1.0f, 1.0f, 0.0f,  //125
    0.20f,  0.80f,  0.00f,  1.0f, 1.0f, 0.0f,  //126
    0.10f,  0.85f,  0.00f,  1.0f, 1.0f, 0.0f,  //127

    0.20f,  0.95f,  0.00f,  1.0f, 1.0f, 0.0f,  //128
    0.10f,  0.90f,  0.00f,  1.0f, 1.0f, 0.0f,  //129
    0.20f,  0.90f,  0.00f,  1.0f, 1.0f, 0.0f,  //130
    0.10f,  0.95f,  0.00f,  1.0f, 1.0f, 0.0f,  //131

//c6
    0.35f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //132
    0.25f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //133
    0.35f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //134
    0.25f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //135

    0.35f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //136
    0.25f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //137
    0.35f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //138
    0.25f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //139

    0.35f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //140
    0.25f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //141
    0.35f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //142
    0.25f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //143

    0.35f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //144
    0.25f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //145
    0.35f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //146
    0.25f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //147

    0.35f,  0.65f,  0.00f,  1.0f, 1.0f, 0.0f,  //148
    0.25f,  0.60f,  0.00f,  1.0f, 1.0f, 0.0f,  //149
    0.35f,  0.60f,  0.00f,  1.0f, 1.0f, 0.0f,  //150
    0.25f,  0.65f,  0.00f,  1.0f, 1.0f, 0.0f,  //151

    0.35f,  0.75f,  0.00f,  1.0f, 1.0f, 0.0f,  //152
    0.25f,  0.70f,  0.00f,  1.0f, 1.0f, 0.0f,  //153
    0.35f,  0.70f,  0.00f,  1.0f, 1.0f, 0.0f,  //154
    0.25f,  0.75f,  0.00f,  1.0f, 1.0f, 0.0f,  //155

    0.35f,  0.85f,  0.00f,  1.0f, 1.0f, 0.0f,  //156
    0.25f,  0.80f,  0.00f,  1.0f, 1.0f, 0.0f,  //157
    0.35f,  0.80f,  0.00f,  1.0f, 1.0f, 0.0f,  //158
    0.25f,  0.85f,  0.00f,  1.0f, 1.0f, 0.0f,  //159

    //c7
    0.50f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //160
    0.40f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //161
    0.50f,  0.20f,  0.00f,  1.0f, 1.0f, 0.0f,  //162
    0.40f,  0.25f,  0.00f,  1.0f, 1.0f, 0.0f,  //163

    0.50f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //164
    0.40f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //165
    0.50f,  0.30f,  0.00f,  1.0f, 1.0f, 0.0f,  //166
    0.40f,  0.35f,  0.00f,  1.0f, 1.0f, 0.0f,  //167

    0.50f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //168
    0.40f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //169
    0.50f,  0.40f,  0.00f,  1.0f, 1.0f, 0.0f,  //170
    0.40f,  0.45f,  0.00f,  1.0f, 1.0f, 0.0f,  //171

    0.50f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //172
    0.40f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //173
    0.50f,  0.50f,  0.00f,  1.0f, 1.0f, 0.0f,  //174
    0.40f,  0.55f,  0.00f,  1.0f, 1.0f, 0.0f,  //175

    0.50f,  0.65f,  0.00f,  1.0f, 1.0f, 0.0f,  //176
    0.40f,  0.60f,  0.00f,  1.0f, 1.0f, 0.0f,  //177
    0.50f,  0.60f,  0.00f,  1.0f, 1.0f, 0.0f,  //178
    0.40f,  0.65f,  0.00f,  1.0f, 1.0f, 0.0f,  //179
};

GLuint indices[] {
    //c7
    176, 177, 178,   
    176, 179, 177,

    172, 173, 174,  
    172, 175, 173,

    168, 169, 170,   
    168, 171, 169,

    164, 165, 166,   
    164, 167, 165,

    160, 161, 162,   
    160, 163, 161,

    //c6
    156, 157, 158,  
    156, 159, 157,

    152, 153, 154,   
    152, 155, 153,

    148, 149, 150,   
    148, 151, 149,

    144, 145, 146,   
    144, 147, 145,

    140, 141, 142,   
    140, 143, 141,

    136, 137, 138,   
    136, 139, 137,

    132, 133, 134,   
    132, 135, 133,

    //c5
    128, 129, 130,   
    128, 131, 129,

    124, 125, 126,   
    124, 127, 125,

    120, 121, 122,  
    120, 123, 121,

    116, 117, 118,   
    116, 119, 117,

    112, 113, 114,   
    112, 115, 113,

    108, 109, 110,   
    108, 111, 109,

    104, 105, 106,   
    104, 107, 105,

    100, 101, 102,   
    100, 103, 101,

    //c4
    96, 97, 98, 
    96, 99, 97,

    92, 93, 94, 
    92, 95, 93,

    88, 89, 90, 
    88, 91, 89,

    84, 85, 86, 
    84, 87, 85,

    80, 81, 82, 
    80, 83, 81,

    76, 77, 78, 
    76, 79, 77,

    72, 73, 74, 
    72, 75, 73,

    //c3
    68, 69, 70,  
    68, 71, 69,

    64, 65, 66,   
    64, 67, 65,

    60, 61, 62,   
    60, 63, 61,

    56, 57, 58,   
    56, 59, 57,

    52, 53, 54,   
    52, 55, 53,

    48, 49, 50,   
    48, 51, 49,

    //c2
    44, 45, 46,
    44, 47, 45,

    40, 41, 42,
    40, 43, 41,

    36, 37, 38,
    36, 39, 37,

    32, 33, 34,
    32, 35, 33,

    28, 29, 30,
    28, 31, 29,

    24, 25, 26,
    24, 27, 25,

    20, 21, 22,
    20, 23, 21,

    16, 17, 18,
    16, 19, 17,

    //c1
    12, 13, 14,
    12, 15, 13,

    8, 9, 10,
    8, 11, 9,

    4, 5, 6,
    4, 7, 5,

    0, 1, 2,
    0, 3, 1,

};

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint ebo;
GLuint shader;      // combined vertex and fragment shader

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // generate the VAO and VBO objects and store their IDs in vao and vbo, respectively
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vao);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - the stride length of the vertex array is 6 floats (6 * sizeof(float))
    // - layout location 0 (position) is 3 floats and starts at the first float of the vertex array (offset 0)
    // - layout location 1 (color) is also 3 floats but starts at the fourth float (offset 3 * sizeof(float))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

    // enable the layout locations so they can be used by the vertex shader
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO and VBO IDs, and follow the same process above to upload them to the GPU

    // load our shader program
    shader = gdevLoadShader("demo2.vs", "demo2.fs");
    if (! shader)
        return false;

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // clear the whole frame
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // compute a value for the glow amount for this frame
    float glow = fabs(sin(glfwGetTime() / 3.0f)) / 2 + 0.5f;
    
    float XAxis = sin(glfwGetTime())/2;
    float YAxis = cos(glfwGetTime())/2;
    float red = fabs(sin(glfwGetTime()));
    float green = fabs(sin(glfwGetTime() / 4));
    float blue = fabs(sin(glfwGetTime() / 8));

    // using our shader program...
    glUseProgram(shader);

    glUniform1f(glGetUniformLocation(shader, "XAxis"), XAxis);
    glUniform1f(glGetUniformLocation(shader, "YAxis"), YAxis);
    glUniform3f(glGetUniformLocation(shader, "color"), red, green, blue);

    // ... set the uniform variables of the shader...
    // (in this case, simply set the value of the glow)
    glUniform1f(glGetUniformLocation(shader, "glow"), glow);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

/*****************************************************************************/

// handler called by GLFW when there is a keyboard event
void handleKeys(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    // pressing Esc closes the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
}

// handler called by GLFW when the window is resized
void handleResize(GLFWwindow* pWindow, int width, int height)
{
    // tell OpenGL to do its drawing within the entire "client area" (area within the borders) of the window
    glViewport(0, 0, width, height);
}

// main function
int main(int argc, char** argv)
{
    // initialize GLFW and ask for OpenGL 3.3 core
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create a GLFW window with the specified width, height, and title
    pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (! pWindow)
    {
        // gracefully terminate if we cannot create the window
        std::cout << "Cannot create the GLFW window.\n";
        glfwTerminate();
        return -1;
    }

    // make the window the current context of subsequent OpenGL commands,
    // and enable vertical sync and aspect-ratio correction on the GLFW window
    glfwMakeContextCurrent(pWindow);
    glfwSwapInterval(1);
    glfwSetWindowAspectRatio(pWindow, WINDOW_WIDTH, WINDOW_HEIGHT);

    // set up callback functions to handle window system events
    glfwSetKeyCallback(pWindow, handleKeys);
    glfwSetFramebufferSizeCallback(pWindow, handleResize);

    // don't miss any momentary keypresses
    glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GLFW_TRUE);

    // initialize GLAD, which acts as a library loader for the current OS's native OpenGL library
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // if our initial setup is successful...
    if (setup())
    {
        // do rendering in a loop until the user closes the window
        while (! glfwWindowShouldClose(pWindow))
        {
            // render our next frame
            // (by default, GLFW uses double-buffering with a front and back buffer;
            // all drawing goes to the back buffer, so the frame does not get shown yet)
            render();

            // swap the GLFW front and back buffers to show the next frame
            glfwSwapBuffers(pWindow);

            // process any window events (such as moving, resizing, keyboard presses, etc.)
            glfwPollEvents();
        }
    }

    // gracefully terminate the program
    glfwTerminate();
    return 0;
}
