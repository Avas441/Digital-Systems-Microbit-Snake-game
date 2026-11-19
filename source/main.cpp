#include "MicroBit.h"



MicroBit uBit; // The MicroBit object

struct pixel// Create a structure for the coordinates of each pixel of the snake
{
    int x, y;
};

//The following are all global variable declared outside main so that all functions within the file can access them without passing pointers or referneces

int length = 1; //Tracks how long the snake is and is intitally, set at one to represent the snake being 1 "pixel" long at the start and is a
const int MAX_LENGTH = 4; //Sets the max length the snake can grow to as set out by the docs


int sleep_period = 400;   // The initial sleep time that is used to set the "speed" of the game
int sleep_reduction = 25; // Specifies by how much in ms the sleep_period should be decreased by therefore increasing the games speed
int sleep_max = 300; //Sets the hard limit for how fast the game can go to avoid it being unreadable

struct pixel apple; //Declares a new variable apple with the struct "pixel" and has the memebers x and y to represent the cordinates on the mcirobit
int appleActive = 0; //A simple flag that tracks if there is currently a apple on the "map"
int score = 0; //Stores the current score of the whole game

int direction; // Stores the direction the snake is heading towards with numbers from 0-3 representing:

//direction 0 = Snake is moving right
//direction 1 = Snake is moving to the bottom
//direction 2 = Snake is moving left
//direction 3 = Snake is moving to the top

int gameLoop = 1; //Used to set whether the game should contiune to loop

struct pixel snake[MAX_LENGTH + 1]; // Create a snake (array) that consists of MAX_LENGTH + 1 elements


//The below functions and implementation are taken from the week 7 lab exercise and is allowed under the guidance in the documentation

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




void onButtonA(MicroBitEvent e) //Custom event handler that is trigered when the player presses the button "A" and is used to move the snake left relative to its current direction
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

void onButtonB(MicroBitEvent e) //Custom event handler that is trigered when the player presses the button "B" and is used to move the snake left relative to its current direction
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


int main()
{   

    struct pixel snake_head = {0, 2}; // Initialise the coordinates of the snake head

    
    uBit.init();// Initialise the micro:bit
    
    uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE); // Set display mode to greyscale
    
    initialize_snake(snake_head); //Draws the snake at the starting cordinates

    direction = 0; //Sets the inital direction of the snake to move right 

    while (gameLoop == 1) //Establishes the main game loop
    {

        if(appleActive == 0) // Used to check if there is currently an apple on the LED board
        {

            //Uses the microbit function "microbit_random" to set a random x and y value between 0 and 5 for the apple
            apple.x = microbit_random(4); 
            apple.y = microbit_random(4);

            uBit.display.image.setPixelValue(apple.x, apple.y, 255); //Uses the apples x and y cordinates and prints a pixel there on the display
            appleActive = 1; // Updates flag so only one apple spawns
        }

        if(snake_head.x == apple.x && snake_head.y == apple.y) //Checks if the current location of snake_head is matching to the above created apple x and y and if true means it "ate" an apple
        {
            uBit.display.image.setPixelValue(apple.x, apple.y, 0);//Removes the pixel at apples x and y cordinates and updates the screen
            
            score++; //Incremenets the score variable
            length++; //Increments the length of the snake by 1
            appleActive = 0; //Resets the flag so once the while loop is done a new apple is spawned
            
            uBit.display.scroll(score); //DEBUG used to show the score after eating an apple

            while(sleep_period >= sleep_max)
            {
                sleep_period = sleep_period - sleep_reduction;
            }
            if(length == 5)
            {
                uBit.display.scroll("You Win!"); //Displays a message to the user
                gameLoop = 0; //Stops the game loop

            }
        }
        
        display_snake();// Print the snake
        
        uBit.sleep(sleep_period);// Wait for starting sleep_period value of 400 ms

        //Sets up a event listener for button "A" and "B" and listens out for when the user clicks on the button to trigger the corresponding event handler
        
        uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
        uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);

        //Uses the direction integer to keep the snake constally moving
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