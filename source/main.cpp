#include "MicroBit.h"



MicroBit uBit; // The MicroBit object

const int MAX_LENGTH = 4; // Sets the starting length of the snake
int sleep_period = 400;   // The initial sleep time
int sleep_reduction = 25;
int sleep_max = 300;
int length = 1;
// Create a structure for the coordinates of each pixel of the snake
struct pixel
{
    int x, y;
};


struct pixel snake[MAX_LENGTH + 1]; // Create a snake (array) that consists of MAX_LENGTH + 1 empty elemnets

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

int direction; //Global variable that is used to store what current direction the snake is heading to


//direction 0 = Snake is moving right
//direction 1 = Snake is moving to the bottom
//direction 2 = Snake is moving left
//direction 3 = Snake is moving to the top

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

    direction = 0; //Sets the inital direction of the snake after being printed onto display

    while (1)
    {

        if(appleActive == 0) // Used to check if there is currently an apple on the LED board
        { // Creates a apple out of the struct "pixel" and sets its cordinates to 0,0 "top left"

            apple.x = microbit_random(5);


            apple.y = microbit_random(5);

            uBit.display.image.setPixelValue(apple.x, apple.y, 255); //Uses the apples x and y cordinates and prints a pixel there on the display
            appleActive = 1; // Updates flag so only one apple spawns

        }

        if(snake_head.x == apple.x && snake_head.y == apple.y) //Checks if the current location of snake_head is matching to the above created apple x and y and if true means it "ate" an apple
        {
            uBit.display.image.setPixelValue(apple.x, apple.y, 0);//Removes the pixel at apples x and y cordinates and updates the screen
            score++; //Incremenets the score variable
            length++; //Increments the length of the snake by 1
            appleActive = 0; //Resets the flag so once the while loop is done a new apple is spawned
            uBit.display.scroll(score); //DEBUG used to sh w the score after eating an apple

            while(sleep_period >= sleep_max)
            {
                sleep_period = sleep_period - sleep_reduction;
            }
            if(length == 5)
            {
                uBit.display.scroll("You Win!");
                uBit.sleep(10000000000);
            }
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