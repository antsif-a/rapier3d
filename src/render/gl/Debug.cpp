#include <cstdio>
#include <glad/gl.h>

const char * map_source(GLenum source)
{
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
            return "OTHER";
        default:
            return "UNKNOWN";
    }
}

const char * map_type(GLenum type)
{
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            return  "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEPRECATED BEHAVIOUR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "UNDEFINED BEHAVIOUR";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "PERFORMANCE";
        case GL_DEBUG_SOURCE_OTHER:
            return "OTHER";
        default:
            return "UNKNOWN";
    }
}

const char * map_severity(GLenum severity)
{
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW:
            return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
        default:
            return "UNKNOWN";
    }
}

void gl_debug_message_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar * message,
    const void * data
)
{
    printf("%d: %s of %s severity, raised from %s: %s\n",
            id, map_type(type), map_severity(severity), map_source(source), message);
}