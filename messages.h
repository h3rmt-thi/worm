#ifndef MESSAGES_H
#define MESSAGES_H

#include "worm_model.h"

void clearLineInMessageArea(int row);
void showStatus(struct Board *board, struct Worm *worm);
int showDialog(char *prompt1, char *prompt2);

#endif // #define MESSAGES_H
