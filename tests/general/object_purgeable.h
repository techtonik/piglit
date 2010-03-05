/*
 * Copyright © 2010 Intel Corporation
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
 *
 * Authors:
 *    Shuang He <shuang.he@intel.com>
 *    Ian Romanick <ian.d.romanick@intel.com>
 */

#ifndef OBJECT_PURGEABLE_H
#define OBJECT_PURGEABLE_H

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif

#ifndef GL_APPLE_object_purgeable
#define GL_APPLE_object_purgeable 1

#define GL_RELEASED_APPLE                 0x8A19
#define GL_VOLATILE_APPLE                 0x8A1A
#define GL_RETAINED_APPLE                 0x8A1B
#define GL_UNDEFINED_APPLE                0x8A1C
#define GL_PURGEABLE_APPLE                0x8A1D
#define GL_BUFFER_OBJECT_APPLE            0x85B3


typedef GLenum (APIENTRYP PFNGLOBJECTPURGEABLEAPPLEPROC) (GLenum objectType, GLuint name, GLenum option);
typedef GLenum (APIENTRYP PFNGLOBJECTUNPURGEABLEAPPLEPROC) (GLenum objectType, GLuint name, GLenum option);
typedef void (APIENTRYP PFNGLGETOBJECTPARAMETERIVAPPLEPROC) (GLenum objectType, GLuint name, GLenum pname, GLint* params);

#endif

extern PFNGLOBJECTPURGEABLEAPPLEPROC pglObjectPurgeableAPPLE;
extern PFNGLOBJECTUNPURGEABLEAPPLEPROC pglObjectUnpurgeableAPPLE;
extern PFNGLGETOBJECTPARAMETERIVAPPLEPROC pglGetObjectParameterivAPPLE;

extern void init_ObjectPurgeableAPI(void);

extern GLboolean test_ObjectpurgeableAPPLE(GLenum objectType, GLuint name,
					   GLenum option);

extern GLboolean test_ObjectunpurgeableAPPLE(GLenum objectType, GLuint name,
					     GLenum option);

extern GLboolean test_GetObjectParameterivAPPLE(GLenum objectType, GLuint name,
						GLenum expect);

#endif /* OBJECT_PURGEABLE_H */
