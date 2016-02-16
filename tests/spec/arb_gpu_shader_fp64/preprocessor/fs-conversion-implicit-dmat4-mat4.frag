// [config]
// expect_result: fail
// glsl_version: 1.50
// require_extensions: GL_ARB_gpu_shader_fp64
// [end config]
//
// GL_ARB_gpu_shader_fp64 spec states:
//
//     "No implicit conversions are
//      provided to convert from unsigned to signed integer types, from
//      floating-point to integer types, or from higher-precision to
//      lower-precision types.  There are no implicit array or structure
//      conversions."
//

#version 150
#extension GL_ARB_gpu_shader_fp64 : require

uniform dmat4 dmat4_value;
uniform mat4 mat4_value;

out vec4 color;

#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)

void main()
{
    mat4 converted = dmat4_value;
    bool match = converted == mat4_value;
    color = match ? GREEN : RED;
}