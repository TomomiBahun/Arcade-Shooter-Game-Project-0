#include "GV.h"

void read_file() {
	int fileHandle;
	//text.index = 0; // if I initialize this property here, I don't get exception...
	text.y = 360;

	fileHandle = FileRead_open("message.txt");
	while (FileRead_eof(fileHandle) == 0) {
		FileRead_gets(text.message[text.index], 256, fileHandle);
		text.index++; // remember the number of lines
	}
	for (int i = 0; i < text.index; i++) {
		DrawString(32, text.y + i*20, text.message[i], GetColor(255, 0, 0));
		while (!(CheckHitKey(KEY_INPUT_Z))) {

		}
	}
	//WaitKey();
	FileRead_close(fileHandle);
}