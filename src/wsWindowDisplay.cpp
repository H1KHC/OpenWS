#include "wsWindow.h"

void wsWindow::display(int lastX, int lastY,
					   int cutX, int cutY, int cutWidth, int cutHeight) {
	extern int debugOutput;
	//Update this window's global position
	if(fatherWindow != nullptr)
		lastX += position.x, lastY += position.y;

	//Update scissor range
	if (cutX < lastX) cutWidth -= lastX - cutX, cutX = lastX;
	if (cutY < lastY) cutHeight -= lastY - cutY, cutY = lastY;
	if (cutX + cutWidth > lastX + size.x) cutWidth = lastX + size.x - cutX;
	if (cutY + cutHeight > lastY + size.y) cutHeight = lastY + size.y - cutY;

	//Out of scissor range check
	if (lastX > cutX + cutWidth || lastY > cutY + cutHeight
		|| lastX + size.x < cutX || lastY + size.y < cutY) return;

	//Draw window
	if (!(styleMask & WS_STYLE_NODISPLAY)) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex2i(lastX, topWindow->size.y - lastY - size.y);
		glTexCoord2d(1, 0);
		glVertex2i(lastX + size.x, topWindow->size.y - lastY - size.y);
		glTexCoord2d(1, 1);
		glVertex2i(lastX + size.x, topWindow->size.y - lastY);
		glTexCoord2d(0, 1);
		glVertex2i(lastX, topWindow->size.y - lastY);
		glEnd();
		if (debugOutput) {
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_LINES);
			if (!subWindow.isEmpty()) {
				if ((debugOutput & WS_SDM_CURSORWINDOW) && windowUnderCursor) {
					glColor3d(1, 0, 0);
					glVertex2i(lastX + size.x, topWindow->size.y - lastY);
					glVertex2i((lastX + windowUnderCursor->data->position.x + windowUnderCursor->data->size.x),
						topWindow->size.y - (lastY + windowUnderCursor->data->position.y));
				}
				if (debugOutput && WS_SDM_FOCUSWINDOW) {
					glColor3d(1, 1, 1);
					glVertex2i(lastX, topWindow->size.y - lastY);
					glVertex2i((lastX + subWindow.front()->data->position.x),
						topWindow->size.y - (lastY + subWindow.front()->data->position.y));
				}
			}
			if ((debugOutput & WS_SDM_CURSORPOS) && underCursor && windowUnderCursor == nullptr) {
				glColor3d(1, 1, 1);
				glVertex2i(lastX + cursorPos.x, topWindow->size.y - lastY);
				glVertex2i(lastX + cursorPos.x, topWindow->size.y - (lastY + size.y + 1));
				glVertex2i(lastX, topWindow->size.y - (lastY + cursorPos.y));
				glVertex2i(lastX + size.x + 1, topWindow->size.y - (lastY + cursorPos.y));
			}
			glEnd();
			glEnable(GL_TEXTURE_2D);
		}
	}

	glPushAttrib(GL_SCISSOR_BIT);
	glScissor(cutX, topWindow->size.y - cutY - cutHeight, cutWidth, cutHeight);
	//Enum subwindows
	for (wsWindowListNode *node = subWindow.back(); node; node = node->last) {
		node->data->display(lastX, lastY, cutX, cutY, cutWidth, cutHeight);
	}
	glPopAttrib();
}
