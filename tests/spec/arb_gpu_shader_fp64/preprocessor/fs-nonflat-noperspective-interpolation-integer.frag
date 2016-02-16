// [config]
// expect_result: fail
// glsl_version: 1.50
// [end config]
//
// Declare a noperspective double fragment input.
//
// GL_ARB_gpu_shader_fp64 spec states:
//
//    "Modifications to The OpenGL Shading Language Specification, Version 1.50
//     (Revision 09)
//    ...
//        Modify Section 4.3.4, Inputs, p. 31
//    ...
//        (modify third paragraph, p. 32, allowing doubles as inputs and disallowing
//        as non-flat fragment inputs) ... Fragment inputs can only be signed and
//        unsigned integers and integer vectors, float, floating-point vectors,
//        double, double-precision vectors, single- or double-precision matrices, or
//        arrays or structures of these. Fragment shader inputs that are signed or
//        unsigned integers, integer vectors, doubles, double-precision vectors, or
//        double-precision matrices must be qualified with the interpolation
//        qualifier flat."
//

#version 150

noperspective in int i;

void main()
{
    gl_FragColor = vec4(i, 0.0, 0.0, 0.0);
}