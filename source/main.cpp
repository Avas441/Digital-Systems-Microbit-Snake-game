#include "MicroBit.h"

MicroBit uBit;            // The MicroBit object

int length = 4; // The maximum length of the snake
int sleep_period = 400;   // The initial sleep time

// Create a structure for the coordinates of each pixel of the snake
struct pixel
{
    int x, y;
};

// Create a snake (array) that consists of length + 1 pixels.
// The actual (visible) length of the snake is length,
// i.e., from [0] to [length-1]. The last position
// (i.e., [length]) holds the previous position of the tail,
// which is erased when the snake moves.
struct pixel snake[25]; 

// Initialise the coordinates of each pixel of the snake,
// so that the head of the snake is in position (new_head.x, new_head.y)
// and the remaining co-ordinates are set to -1.
void initialize_snake(struct pixel new_head)
{
    int i;
    snake[0] = new_head;

    for (i = 1; i < length + 1; i++)
    {
        snake[i].x = -1;
        snake[i].y = -1;
    }
}

// Move the snake. Its head (element [0]) moves to the
// new coordinates (new_head.x, new_head.y)
// while pixel [i] of the snake moves to the co-ordinates that
// pixel [i-1] of the snake used to occupy.
void move_snake(struct pixel new_head)
{
    int i;

    for (i = length; i > 0; i--)
        snake[i] = snake[i - 1];

    snake[0] = new_head;
}

// Turn on pixel [0] of the snake (the head) and turn off pixel [length] of the snake,
// where the tail (the last pixel of the snake) used to be in the previous step.
void display_snake()
{
    // Move the head forward
    if (snake[0].x >=0 && snake[0].y >= 0)
        uBit.display.image.setPixelValue(snake[0].x, snake[0].y, 15);

    // Erase the tail
    if (snake[length].x >= 0 && snake[length].y >= 0 )
        uBit.display.image.setPixelValue(snake[length].x, snake[length].y, 0);
}

int direction;

void onButtonA(MicroBitEvent e)
{
    if(direction == 0)
    {
        direction = 3;
    } else if (direction == 3)
    {
        direction = 2;
    } else if (direction == 2)
    {
        direction = 1;
    } else if (direction == 1)
    {
        direction = 0;
    }

}

void onButtonB(MicroBitEvent e)
{
    if(direction == 0)
    {
        direction = 1;
    } else if (direction == 1)
    {
        direction = 2;
    } else if (direction == 2)
    {
        direction = 3;
    } else if (direction == 3)
    {
        direction = 0;
    }

}

int appleActive = 0;

struct pixel apple;

int score = 0;


int main()
{   
    // Initialise the coordinates of the snake head
    struct pixel snake_head = {0, 2};

    // Initialise the micro:bit
    uBit.init();
    // Set display mode to greyscale
    uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);
    // Reveal the head of the snake at the top-left corner of the 5x5 display
    initialize_snake(snake_head);

    direction = 0;

    while (1)
    {

        if(appleActive == 0)
        {
            struct pixel apple = {0,0};
            uBit.display.image.setPixelValue(apple.x, apple.y, 255);
            appleActive = 1;

        }

        if(snake_head.x == apple.x && snake_head.y == apple.y)
        {
            uBit.display.image.setPixelValue(apple.x, apple.y, 0);
            score++;
            appleActive = 0;
            uBit.display.scroll(score);
        }
        // Print the snake
        display_snake();
        // Wait for 100 ms
        uBit.sleep(sleep_period);

        uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
        uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);

        if(direction == 0)
        {
            snake_head.x++;
        } else if (direction == 1)
        {
            snake_head.y++;
        } else if (direction == 2)
        {
            snake_head.x--;
        } else if (direction == 3)
        {
            snake_head.y--;
        }

        
        
        // Calculate the position of each body part of the snake,
        // now that the position of its head is known.
        move_snake(snake_head);

    }
}