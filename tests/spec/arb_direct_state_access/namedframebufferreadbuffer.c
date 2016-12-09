/*
 * Copyright 2016 Intel Corporation
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

/** @file namedframebufferreadbuffer.c
 *
 * Tests NamedFramebufferReadBuffer function to see if they behaves in the
 * expected way, throwing the correct errors, etc.
 *
 * From OpenGL 4.5, section 18.2.1 "Selecting Buffers for Reading"", page 502:
 *
 * "void NamedFramebufferReadBuffer(uint framebuffer,
 *                                  enum src );
 *
 * [...]
 *
 * ERRORS:
 * An INVALID_OPERATION error is generated by NamedFramebuffer-
 *  ReadBuffer if framebuffer is not zero or the name of an existing
 *  framebuffer
 * An INVALID_ENUM error is generated if src is not one of the values
 *  in tables 17.4 or 17.5.
 * An INVALID_OPERATION error is generated if the default framebuffer
 *  is affected and src is a value (other than NONE ) that does not
 *  indicate any of the color buffers allocated to the default
 *  framebuffer.
 * An INVALID_OPERATION error is generated if a framebuffer object is
 *  affected, and src is one of the constants from table 17.4 (other
 *  than NONE , or COLOR_ATTACHMENTm where m is greater than or equal
 *  to the value of MAX_COLOR_ATTACHMENTS ).
 */

#include "piglit-util-gl.h"
#include "dsa-utils.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.supports_gl_core_version = 31;

	config.window_visual = PIGLIT_GL_VISUAL_RGBA |
		PIGLIT_GL_VISUAL_DOUBLE;

PIGLIT_GL_TEST_CONFIG_END

void
piglit_init(int argc, char **argv)
{
	piglit_require_extension("GL_ARB_direct_state_access");
	piglit_require_extension("GL_ARB_framebuffer_object");
}

static const GLenum table_17_4[] = {
	GL_NONE,
	GL_FRONT_LEFT,
	GL_FRONT_RIGHT,
	GL_BACK_LEFT,
	GL_BACK_RIGHT,
	GL_FRONT,
	GL_BACK,
	GL_LEFT,
	GL_RIGHT,
	GL_FRONT_AND_BACK
};

enum piglit_result
piglit_display(void)
{
	bool pass = true;
	GLuint framebuffer;
	bool subtest_pass = true;
	unsigned int i;

	glCreateFramebuffers(1, &framebuffer);
	piglit_check_gl_error(GL_NO_ERROR);

	/* Check some various cases of errors */

	/* From the error table:
	 *  "An INVALID_OPERATION error is generated if a framebuffer
	 *   object is affected, and src is one of the constants from
	 *   table 17.4 (other than NONE , or COLOR_ATTACHMENTm where
	 *   m is greater than or equal to the value of
	 *   MAX_COLOR_ATTACHMENTS ).
	 *
	 * So we will test against all the enums on 17.4 and
	 * COLOR_ATTACHMENTm with m >= MAX_COLOR_ATTACHMENTS for
	 * error. GL_NONE and MAX_COLOR_ATTACHMENTSm with
	 * m < MAX_COLOR_ATTACHMENTS (table 17.5) for no error.
	 */
	int max_attachments;

	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_attachments);

	subtest_pass = true;
	for (i = 0; i < ARRAY_SIZE (table_17_4); i++) {
		  glNamedFramebufferReadBuffer(framebuffer, table_17_4[i]);
		  subtest_pass = subtest_pass && piglit_check_gl_error(GL_INVALID_OPERATION);
	}

	glNamedFramebufferReadBuffer(framebuffer, GL_COLOR_ATTACHMENT0 + max_attachments);
	subtest_pass = subtest_pass && piglit_check_gl_error(GL_INVALID_OPERATION);

	for (i = 0; i < max_attachments; i++) {
		glNamedFramebufferReadBuffer(framebuffer, GL_COLOR_ATTACHMENT0 + i);
		subtest_pass = subtest_pass && piglit_check_gl_error(GL_NO_ERROR);
	}

	glNamedFramebufferReadBuffer(framebuffer, GL_NONE);
	subtest_pass = subtest_pass && piglit_check_gl_error(GL_NO_ERROR);

	piglit_report_subtest_result(subtest_pass ? PIGLIT_PASS : PIGLIT_FAIL,
				     "An INVALID_OPERATION error is generated if a "
				     "framebuffer object is affected, and src is one "
				     "of the constants from table 17.4");
	pass = pass & subtest_pass;
	/* clean up */
	glDeleteFramebuffers(1, &framebuffer);

	return pass ? PIGLIT_PASS : PIGLIT_FAIL;
}
