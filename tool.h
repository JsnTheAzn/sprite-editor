#ifndef TOOL_H
#define TOOL_H

#include <QObject>

using std::vector;

class Sprite;

class Tool : public QObject
{
    Q_OBJECT

public:

    //Default constructor
    Tool(QObject* parent = nullptr);

    enum class Tools {
        Pen,
        Eraser,
        PaintBucket,
    };

    //Gets the current tool that is selected.
    Tools getCurrentTool();

    //Sets the tool to the one currently being used.
    void setCurrentTool(Tools tool);

    //Uses the tool at the specific area on the canvas.
    void useTool(Sprite *sprite, int x, int y, const int *rgba);

private:
    Tools currentTool;

    void fillToolCall (Sprite *sprite, int x, int y, const int *rgba);

    void recursiveFill (Sprite *sprite, int x, int y, const int (*newColor), const int (*oldColor));
};

#endif // TOOL_H
