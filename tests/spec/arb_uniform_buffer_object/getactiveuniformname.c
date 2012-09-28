/*
 * Copyright © 2012 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/** @file getactiveuniformname.c
 *
 * From the GL_ARB_uniform_buffer_object spec:
 *
 *     "The name of an active uniform may be queried from the
 *      corresponding uniform index by calling
 *
 *          void GetActiveUniformName(uint program,
 *                                    uint uniformIndex, 
 *                                   sizei bufSize, 
 *                                   sizei* length, 
 *                                   char* uniformName);
 *
 *
 *      The name of the uniform identified by <uniformIndex> is
 *      returned as a null-terminated string in <uniformName>.  The
 *      actual number of characters written into <uniformName>,
 *      excluding the null terminator, is returned in <length>. If
 *      <length> is NULL, no length is returned. The maximum number of
 *      characters that may be written into <uniformName>, including
 *      the null terminator, is specified by <bufSize>.  The returned
 *      uniform name can be the name of built-in uniform state as
 *      well. The complete list of built-in uniform state is described
 *      in section 7.5 of the OpenGL Shading Language
 *      specification. The length of the longest uniform name in
 *      <program> is given by the value of ACTIVE_UNIFORM_MAX_LENGTH,
 *      which can be queried with GetProgramiv.
 *
 *      ...
 *
 *
 *      The error INVALID_VALUE is generated by GetActiveUniformsiv,
 *      GetActiveUniformName, GetActiveUniformBlockiv,
 *      GetActiveUniformBlockName, and UniformBlockBinding if
 *      <program> is not a value generated by GL.
 *
 *      The error INVALID_VALUE is generated by GetActiveUniformName and
 *      GetActiveUniformBlockName if <bufSize> is less than zero.
 *
 *      The error INVALID_VALUE is generated by GetActiveUniformName if
 *      <uniformIndex> is greater than or equal to ACTIVE_UNIFORMS.
 */

#include "piglit-util-gl-common.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.window_width = 10;
	config.window_height = 10;
	config.window_visual = PIGLIT_GL_VISUAL_RGB | PIGLIT_GL_VISUAL_DOUBLE | PIGLIT_GL_VISUAL_ALPHA;

PIGLIT_GL_TEST_CONFIG_END

void
piglit_init(int argc, char **argv)
{
	unsigned int i;
	GLuint fs, prog;
	const char *source =
		"#extension GL_ARB_uniform_buffer_object : enable\n"
		"uniform ubo1 { float a; };\n"
		"uniform ubo2 { float bb; float c; };\n"
		"uniform float dddd;\n"
		"void main() {\n"
		"	gl_FragColor = vec4(a + bb + c + dddd);\n"
		"}\n";
	int uniforms;
	bool pass = true;
	const char *names[4] = {"a", "bb", "c", "dddd"};
	bool found[4] = {false, false, false, false};
	char no_write;
	char fill_char = 0xd0;

	piglit_require_extension("GL_ARB_uniform_buffer_object");

	fs = piglit_compile_shader_text(GL_FRAGMENT_SHADER, source);
	prog = piglit_link_simple_program(fs, 0);
	if (!fs || !prog) {
		fprintf(stderr, "Failed to compile shader:\n%s", source);
		piglit_report_result(PIGLIT_FAIL);
	}

	glGetProgramiv(prog, GL_ACTIVE_UNIFORMS, &uniforms);
	assert(uniforms == 4);

	for (i = 0; i < uniforms; i++) {
		GLint written_strlen = 0;
		GLint namelen = 9999;
		char name[1000];
		int name_index;

		/* This is the size including null terminator. */
		glGetActiveUniformsiv(prog, 1, &i,
				      GL_UNIFORM_NAME_LENGTH, &namelen);

		memset(name, 0xd0, sizeof(name));
		glGetActiveUniformName(prog, i, sizeof(name),
				       &written_strlen, name);
		if (written_strlen >= sizeof(name) - 1) {
			fprintf(stderr,
				"return strlen %d, longer than the buffer size\n",
				written_strlen);
			pass = false;
			continue;
		} else if (name[written_strlen] != 0) {
			fprintf(stderr, "return name[%d] was %d, expected 0\n",
				written_strlen, name[written_strlen]);
			pass = false;
			continue;
		} else if (strlen(name) != written_strlen) {
			fprintf(stderr, "return strlen was %d, but \"%s\" "
				"has strlen %d\n", written_strlen, name,
				(int)strlen(name));
			pass = false;
			continue;
		}

		for (name_index = 0; name_index < ARRAY_SIZE(names); name_index++) {
			if (strcmp(names[name_index], name) == 0) {
				if (found[name_index]) {
					fprintf(stderr,
						"Uniform name \"%s\" "
						"returned twice.\n", name);
					pass = false;
				}
				found[name_index] = true;
				break;
			}
		}
		if (name_index == ARRAY_SIZE(names)) {
			fprintf(stderr,
				"uniform \"%s\" is not a known name\n", name);
			pass = false;
			continue;
		}

		if (namelen != written_strlen + 1) {
			fprintf(stderr,
				"uniform \"%s\" had "
				"GL_UNIFORM_NAME_LENGTH %d, expected %d\n",
				name, namelen, written_strlen + 1);
			pass = false;
			continue;
		}

		/* Test for overflow by writing to a bufSize equal to
		 * strlen and checking if a null terminator or
		 * something landed past that.
		 */
		memset(name, fill_char, sizeof(name));
		glGetActiveUniformName(prog, i, written_strlen, NULL, name);
		if (name[written_strlen] != fill_char) {
			fprintf(stderr, "glGetActiveUniformName overflowed: "
				"name[%d] = 0x%02x instead of 0x%02x\n",
				written_strlen, name[written_strlen],
				fill_char);
			pass = false;
		}
	}

	no_write = fill_char;
	glGetActiveUniformName(0xd0d0, 0, 1, NULL, &no_write);
	pass = piglit_check_gl_error(GL_INVALID_VALUE) && pass;
	if (no_write != fill_char)
		pass = false;

	no_write = fill_char;
	glGetActiveUniformName(prog, 0, -1, NULL, &no_write);
	pass = piglit_check_gl_error(GL_INVALID_VALUE) && pass;
	if (no_write != fill_char)
		pass = false;

	no_write = fill_char;
	glGetActiveUniformName(prog, uniforms, 1, NULL, &no_write);
	pass = piglit_check_gl_error(GL_INVALID_VALUE) && pass;
	if (no_write != fill_char)
		pass = false;

	glDeleteProgram(prog);
	glDeleteShader(fs);

	piglit_report_result(pass ? PIGLIT_PASS : PIGLIT_FAIL);
}

enum piglit_result piglit_display(void)
{
	/* UNREACHED */
	return PIGLIT_FAIL;
}

