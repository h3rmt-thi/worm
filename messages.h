#ifndef _MESSAGES_H
#define _MESSAGES_H

#include "worm_model.h"

void clearLineInMessageArea(int row);
void showBorderLine();
void showStatus(const struct Worm *);
int showDialog(char *prompt1, char *prompt2);

#endif // #define _MESSAGES_H
