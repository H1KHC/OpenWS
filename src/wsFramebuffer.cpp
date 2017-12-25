#define GLEW_STATIC
#include <GL/glew.h>

#define testEXT(func,...) do{ \
	if(func) func(__VA_ARGS__);\
	else func##EXT(__VA_ARGS__);} while(0)

int generateFramebuffer(GLuint &frameBuffer, GLuint &renderBuffer, GLuint &texture, const int width, const int height) {
	if (!width || !height)return false;
	do {
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		frameBuffer = (renderBuffer = (texture = 0));
		glGenFramebuffers(1, &frameBuffer);
		glGenRenderbuffers(1, &renderBuffer);
		glGenTextures(1, &texture);
		if (!frameBuffer || !renderBuffer || !texture) break;

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
		glBindTexture(GL_TEXTURE_2D, texture);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) break;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glPopAttrib();
		return true;
	} while (0);

	if (frameBuffer) glDeleteFramebuffers(1, &frameBuffer);
	if (renderBuffer) glDeleteRenderbuffers(1, &renderBuffer);
	if (texture) glDeleteTextures(1, &texture);
	glPopAttrib();
	return false;
}

int adjustFramebufferSize(GLuint frameBuffer, GLuint renderBuffer, GLuint texture, const int width, const int height) {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glBindTexture(GL_TEXTURE_2D, texture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	return true;
}


int generateFramebuffer(GLuint &frameBuffer, GLuint &texture, const int width, const int height) {
	if (!width || !height)return false;
	do {
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		frameBuffer = (texture = 0);
		glGenFramebuffers(1, &frameBuffer);
		glGenTextures(1, &texture);
		if (!frameBuffer || !texture) break;

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) break;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glPopAttrib();
		return true;
	} while (0);

	if (frameBuffer) glDeleteFramebuffers(1, &frameBuffer);
	if (texture) glDeleteTextures(1, &texture);
	glPopAttrib();
	return false;
}

int adjustFramebufferSize(GLuint frameBuffer, GLuint texture, const int width, const int height) {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	return true;
}
