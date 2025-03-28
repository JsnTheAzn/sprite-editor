#ifndef TOOL_H
#define TOOL_H

#include <QObject>

using std::vector;

class Sprite;

class Tool : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Tool - Default Tool constructor.
     */
    Tool(QObject* parent = nullptr);

    /**
     * @brief Tools - An enum class consisting of the available tools.
     */
    enum class Tools {
        Pen,
        Eraser,
        PaintBucket,
    };

    /**
     * @brief getCurrentTool - Gets the current tool used by the user.
     * @return The tool used by the user.
     */
    Tools getCurrentTool();

    /**
     * @brief setCurrentTool - Sets the tool to the new tool clicked by the user.
     * @param tool - The new tool selected.
     */
    void setCurrentTool(Tools tool);

    /**
     * @brief useTool - Uses the specified tool at the given x & y coordinates, alongside utilizing the RGBA values given.
     * @param sprite - The sprite instance utilized.
     * @param x - The x coordinate.
     * @param y - The y coordinate.
     * @param rgba - The RGBA values.
     */
    void useTool(Sprite *sprite, int x, int y, const int (&rgba)[4]);

private:
    /**
     * @brief currentTool - The current tool used by the user.
     */
    Tools currentTool;

    /**
     * @brief fillToolCall - Fills an area at the specified x & y coordinates with RGBA values.
     * @param sprite - The sprite instance utilized.
     * @param x - The x coordinate.
     * @param y - The y coordinate.
     * @param rgba - The RGBA values.
     */
    void fillToolCall (Sprite *sprite, int x, int y, const int *rgba);

    /**
     * @brief recursiveFill - Fills an area at the specified x & y, replacing old colors with the new RGBA values.
     * @param sprite - The sprite instance utilized.
     * @param x - The x coordinate.
     * @param y - The y coordinate.
     * @param newColor - The new color to fill at the x,y pixel location.
     * @param oldColor - The old color at the x,y pixel location.
     */
    void recursiveFill (Sprite *sprite, int x, int y, const int (*newColor), const int (*oldColor));
};

#endif // TOOL_H
