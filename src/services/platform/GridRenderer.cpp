#include "GridRenderer.h"

/**
 * Constructor for a grid object
 */
dime::GridRenderer::GridRenderer(const Rectangle &rect,
	std::vector<Renderer*> &gridVector, int columns, int rows,
	std::vector<int> &lines) : 
	myGrid(gridVector),
	myLines(lines),
	myColumns(columns),
	myRows(rows)
{
	myRect = rect;
    updateGridDimensions();
}


/**
 * Updates dimensions of all the grid's rectangles.
 */
void dime::GridRenderer::updateGridDimensions()
{
    int xOffset, yOffset;
    int rectHeight, rectWidth;

    yOffset = 0;
    for (int i=0;i < myRows;i++)
    {
	if (i == myRows - 1)
	{
	    rectHeight = myRect.getHeight() - myLines[myRows + myColumns - 3];
	}
	else
	{
	    rectHeight = myLines[myColumns - 1 + i];
	}

	xOffset = 0;
	for (int j=0;j < myColumns;j++)
	{
	    if (j == myColumns - 1)
	    {
		rectWidth = myRect.getWidth() - myLines[myColumns - 2];
	    }
	    else
	    {
		rectWidth = myLines[j];
	    }

	    myGrid[(i * myColumns)+j]->setRect(dime::Rectangle(myRect.getX()
			+ xOffset, myRect.getY()+yOffset, rectWidth,
			rectHeight));

	    xOffset = rectWidth;
	}

	yOffset = rectHeight;
    }
}

void dime::GridRenderer::setRect(const Rectangle &rect)
{
	myRect = rect;
	updateGridDimensions();
}

/**
 * Renders a grid object
 */
void dime::GridRenderer::render(dime::DrawDevice *device)
                                        /*
                                          int nrOfColumns, int nrOfRows, float splitLineRelativePosition[2][2],
                                          int splitLineOffsetPosition[2][2],
                                          dime::Renderer *rectangleGrid[3][3])
                                        */
{
    /*int colOffset, rowOffset;
    if(rows >0 && cols >0)
        {
            colOffset = myRect.getWidth() / cols;
            rowOffset = myRect.getHeight() / rows;
        }
    else
        {
            return -1;
        }*/
    
    for (int i=0;i < myColumns*myRows;i++)
    {
	myGrid[i]->render(device);
    }
    
    /*int topY, bottomY; //The y values of the top line and bottom sides
    int rightX, leftX;  // the x values of the right and left sides
    topY = myRect.getY();
    bottomY = myRect.getY()+myRect.getHeight();
    leftX = myRect.getX();
    rightX = myRect.getX()+myRect.getWidth();
    
	int index;

    for(index = 0; index < cols; ++index)
        {
            device->drawLine(leftX+(index*colOffset), topY, leftX+(index*colOffset), bottomY, myColor);
        }
    for(index = 0; index < rows; ++index)
        {
            device->drawLine(leftX, topY+(index*rowOffset), rightX, topY+(index*rowOffset), myColor);
        }
    //this for loop purposely leaves off the last two lines to draw
    device->drawLine(rightX, topY, rightX, bottomY, myColor);
    device->drawLine(rightX, bottomY, leftX, bottomY, myColor);

	//TODO: What should be returned here?
	return 0;

	*/
}
