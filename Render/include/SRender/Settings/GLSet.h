#pragma once
#include <glad/glad.h>
namespace SRender::Setting{
enum class SCullFace{
    FRONT = GL_FRONT,
    BACK = GL_BACK,
    FRONT_AND_BACK = GL_FRONT_AND_BACK
};    

enum class SRasterization{
    POINT = GL_POINT,
    LINE = GL_LINE,
    FILL = GL_FILL
};

enum class SPrimitive{
    POINTS						= GL_POINTS,
    LINES						= GL_LINES,
    LINE_LOOP					= 0x0002,
    LINE_STRIP					= 0x0003,
    TRIANGLES					= 0x0004,
    TRIANGLE_STRIP				= 0x0005,
    TRIANGLE_FAN				= 0x0006,
    LINES_ADJACENCY				= 0x000A,
    LINE_STRIP_ADJACENCY		= 0x000B,
    TRIANGLES_ADJACENCY			= 0x000C,
    TRIANGLE_STRIP_ADJACENCY	= 0x000D,
    PATCHES						= 0xE,
};

}