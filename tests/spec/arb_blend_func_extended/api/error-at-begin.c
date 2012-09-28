/*
 * Copyright © 2011 Red Hat
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/**
 * \file error-at-begin.c
 *
 * \author Dave Airlie
 * ARB_blend_func_extended states in ERRORS section
 * "The error INVALID_OPERATION is generated by Begin or any procedure that
 * implicitly calls Begin if any draw buffer has a blend function requiring the
 * second color input (SRC1_COLOR, ONE_MINUS_SRC1_COLOR, SRC1_ALPHA or
 * ONE_MINUS_SRC1_ALPHA), and a framebuffer is bound that has more than
 * the value of MAX_DUAL_SOURCE_DRAW_BUFFERS-1 active color attachements."
 */
#include "piglit-util-gl-common.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.window_width = 100;
	config.window_height = 100;
	config.window_visual = PIGLIT_GL_VISUAL_RGB | PIGLIT_GL_VISUAL_DOUBLE;

PIGLIT_GL_TEST_CONFIG_END

enum piglit_result
piglit_display(void)
{
	return PIGLIT_FAIL;
}

static GLuint fbo;
static GLuint max_buffers;

static void
create_fbo(void)
{
	GLuint rb[32];
	int i;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, fbo);

	glGenRenderbuffers(max_buffers, rb);

	for (i = 0; i < max_buffers; i++) {
		glBindRenderbuffer(GL_RENDERBUFFER_EXT, rb[i]);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT,
					  GL_COLOR_ATTACHMENT0 + i,
					  GL_RENDERBUFFER_EXT,
					  rb[i]);
		glRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_RGBA,
					 piglit_width, piglit_height);
	}
}

void piglit_init(int argc, char **argv)
{
	GLint max_dual_source;
	GLenum buffers[32];
	int i;
	piglit_require_gl_version(30);
	piglit_require_extension("GL_ARB_blend_func_extended");
	piglit_require_extension("GL_ARB_draw_buffers_blend");


	/* This test needs some number of draw buffers, so make sure the
	 * implementation isn't broken. This enables the test to generate a
	 * useful failure message.
	 */
	glGetIntegerv(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, &max_dual_source);
	if (max_dual_source < 1) {
		fprintf(stderr,
			"ARB_blend_func_extended requires GL_MAX_DUAL_SOURCE_DRAW_BUFFERS >= 1.  "
			"Only got %d!\n",
			max_dual_source);
		piglit_report_result(PIGLIT_FAIL);
	}

	max_buffers = max_dual_source + 1;

	create_fbo();

	for (i = 0; i < max_buffers; i++)
		buffers[i] = GL_COLOR_ATTACHMENT0_EXT + i;

	glDrawBuffersARB(max_buffers, buffers);

	for (i = 0; i < max_buffers; i++) {
		if (i >= max_dual_source)
			glBlendFunciARB(i, GL_SRC1_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);
		else
			glBlendFunciARB(i, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnableIndexedEXT(GL_BLEND, i);
	}

	glBegin(GL_QUADS);

	if (!piglit_check_gl_error(GL_INVALID_OPERATION))
		piglit_report_result(PIGLIT_FAIL);
	else
		piglit_report_result(PIGLIT_PASS);
}
