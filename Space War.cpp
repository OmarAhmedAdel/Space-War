#include <stdlib.h>
#include <Windows.h>
#include <playsoundapi.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <glut.h>
#include <math.h>

double playerSpaceshipX = 500.0;
double playerSpaceshipY = 150.0;
double playerSpaceshipWidth = 100.0;
double playerSpaceshipHeight = 40.0;
double playerLaserGunHeight = 40.0; // Increased laser gun height for the player spaceship
double playerMovementSpeed = 5.0;
//double playerMaxY = 150.0; // Maximum Y-coordinate for the player
//double playerMinY = 50.0; // Minimum Y-coordinate for the player

double computerSpaceshipX = 500.0;
double computerSpaceshipY = 450.0;
double computerSpaceshipWidth = 100.0;
double computerSpaceshipHeight = 40.0;
double computerLaserGunHeight = 40.0; 
double computerMovementSpeed = 3.0; 
double computerMaxY = 600.0; // Maximum Y-coordinate for the computer
double computerMinY = 300.0; // Minimum Y-coordinate for the computer

double windowWidth = 1000.0;
double windowHeight = 600.0;

double playerHealth = 100.0; 
double maxPlayerHealth = 100.0; 

double computerHealth = 200.0; 
double maxComputerHealth = 200.0; 

bool isGameOver = false; 

int playerScore = 0; 

int gameDuration = 120;  // Total game duration in seconds




// Power-Up Variables
bool powerUpActive1 = false; // Indicates whether the Speed Up power-up is active
bool powerUpActive2 = false; // Indicates whether the Time Extend power-up is active
int powerUpType1 = 0; // Type 1 is Speed Up
int powerUpType2 = 0; // Type 2 is Time Extend
double powerUpX1 = 0.0; // X-coordinate of the Speed Up power-up
double powerUpY1 = 0.0; // Y-coordinate of the Speed Up power-up
double powerUpX2 = 0.0; // X-coordinate of the Time Extend power-up
double powerUpY2 = 0.0; // Y-coordinate of the Time Extend power-up
double powerUpSize = 20.0; // Size of the power-ups
double powerUpRotationAngle1 = 0.0; // Rotation angle for the Speed Up power-up
double powerUpRotationAngle2 = 0.0; // Rotation angle for the Time Extend power-up
int speedUpDuration = 0; // Define a variable to track the remaining duration of the Speed Up power-up
int powerUpCollectedTime = 0;


// Animated Background Variables
double radius = 0.0; 
int numDots = 0; 
double angleIncrement = 0.0; // Angle between each dot
int numSquares = 6;   // Number of squares
double squareSize = 30.0;  
double rotationAngle = 0.0;  
double rotationSpeed = 1.0; 





#define MAX_COLLECTABLES 5 // Maximum number of collectables

double collectablePositionsX[MAX_COLLECTABLES]; // X-coordinates of collectables
double collectablePositionsY[MAX_COLLECTABLES]; // Y-coordinates of collectables
double collectableSizes[MAX_COLLECTABLES]; // Sizes of collectables
double collectableRotations[MAX_COLLECTABLES]; // Rotation angles for collectables


// Obstacle Variables
const int maxObstacles = 10; // Maximum number of obstacles
double obstacleSize = 20.0; // Fixed size for obstacles
double obstaclesX[maxObstacles];
double obstaclesY[maxObstacles];
bool isObstacleActive[maxObstacles];

// Door Parameters
double doorX = windowWidth - 50; // Adjust as needed
double doorY = 0; // Adjust as needed
double doorWidth = 50; // Adjust as needed
double doorHeight = 100; // Adjust as needed
bool playerWon = false;

double Pi = 3.14159265358979323846;


// This is the method used to print text in OpenGL
// There are three parameters:
// The first two are the coordinates where the text is displayed,
// The third coordinate is the string containing the text to display
void print(int x, int y, char* string)
{
    int len, i;

    // Set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x, y);

    // Get the length of the string to display
    len = (int)strlen(string);

    // Loop to display character by character
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}

void drawSpaceship(double x, double y, double width, double height, bool isPlayer)
{
    // Set the spaceship color
    glColor3d(0, 0, 1);

    // Draw the spaceship body
    glBegin(GL_POLYGON);
    glVertex2d(x - width / 2, y);
    glVertex2d(x - width / 4, y + (isPlayer ? height / 2 : -height / 2));
    glVertex2d(x + width / 4, y + (isPlayer ? height / 2 : -height / 2));
    glVertex2d(x + width / 2, y);
    glVertex2d(x + width / 4, y - (isPlayer ? height / 2 : -height / 2));
    glVertex2d(x - width / 4, y - (isPlayer ? height / 2 : -height / 2));
    glEnd();

    // Draw the spaceship window
    glColor3d(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2d(x - width / 4, y + (isPlayer ? height / 4 : -height / 4));
    glVertex2d(x + width / 4, y + (isPlayer ? height / 4 : -height / 4));
    glVertex2d(x + width / 8, y + (isPlayer ? height / 2 : -height / 2));
    glVertex2d(x - width / 8, y + (isPlayer ? height / 2 : -height / 2));
    glEnd();

    // Draw the left laser gun as a line
    glColor3d(1, 0, 0); // Set laser gun color to red
    glLineWidth(5.0);   // Increase line width for the laser gun
    glBegin(GL_LINES);
    glVertex2d(x - width / 4, y + (isPlayer ? height / 4 : -height / 4));
    glVertex2d(x - width / 4, y + (isPlayer ? (isPlayer ? playerLaserGunHeight : computerLaserGunHeight) : -height / 4));
    glEnd();

    // Draw the right laser gun as a line
    glBegin(GL_LINES);
    glVertex2d(x + width / 4, y + (isPlayer ? height / 4 : -height / 4));
    glVertex2d(x + width / 4, y + (isPlayer ? (isPlayer ? playerLaserGunHeight : computerLaserGunHeight) : -height / 4));
    glEnd();


    // Add the redish-yellow fire at the back of the player's spaceship (triangle)
        glColor3d(1, 0.5, 0); // Redish-yellow color for the fire
        glBegin(GL_POLYGON);
        glVertex2d(x - width / 8, y + (isPlayer ? -height / 2 : height / 2)); // Bottom-left point
        glVertex2d(x + width / 8, y + (isPlayer ? -height / 2 : height / 2)); // Bottom-right point
        glVertex2d(x, y + (isPlayer ? -height / 1.5 : height / 1.5)); // Top point
        glEnd();
    

        glColor3d(1, 0, 0); // Red color for the point
        double pointSize = 5.0; // Size of the point
        glBegin(GL_POINTS);
        glVertex2d(x, y); // Specify the position of the point (center of the spaceship)
        glEnd();
  


    // Adjust the laser guns for the computer's ship to start from the head of the ship and point downward
    if (!isPlayer) {
        double laserGunY = y + (isPlayer ? height / 4 : -height / 4);

        glColor3d(1, 0, 0); // Set laser gun color to red
        glLineWidth(5.0);   // Increase line width for the laser gun

        // Draw the left laser gun as a line
        glBegin(GL_LINES);
        glVertex2d(x - width / 4, laserGunY);
        glVertex2d(x - width / 4, laserGunY - (isPlayer ? playerLaserGunHeight : computerLaserGunHeight)); // Point the laser gun downward
        glEnd();

        // Draw the right laser gun as a line
        glBegin(GL_LINES);
        glVertex2d(x + width / 4, laserGunY);
        glVertex2d(x + width / 4, laserGunY - (isPlayer ? playerLaserGunHeight : computerLaserGunHeight)); // Point the laser gun downward
        glEnd();
    }

}







void drawHealthBar(double x, double y, double width, double height, double currentHealth, double maxHealth, const char* label) {
    // Calculate the percentage of health remaining
    double healthPercentage = currentHealth / maxHealth;

    // Calculate the width of the health bar based on the percentage
    double healthBarWidth = width * healthPercentage;

    // Draw the "Player" label closer to the health bar
    glColor3d(1, 1, 1); // White text
    glRasterPos2f(x - 55, y + (height / 2) - 5);
    int len = (int)strlen(label);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, label[i]);
    }

    // Draw the health bar background (gray)
    glColor3d(0.5, 0.5, 0.5);
    glBegin(GL_POLYGON);
    glVertex2d(x, y);
    glVertex2d(x + width, y);
    glVertex2d(x + width, y + height);
    glVertex2d(x, y + height);
    glEnd();

    // Draw the health part of the health bar (green)
    glColor3d(0, 1, 0);
    glBegin(GL_POLYGON);
    glVertex2d(x, y);
    glVertex2d(x + healthBarWidth, y);
    glVertex2d(x + healthBarWidth, y + height);
    glVertex2d(x, y + height);
    glEnd();
}



void drawObstacles() {
    glColor3d(1, 0, 0); // Red color for obstacles

    for (int i = 0; i < maxObstacles; i++) {
        if (isObstacleActive[i]) {
            glBegin(GL_POLYGON);
            glVertex2d(obstaclesX[i] - obstacleSize / 2, obstaclesY[i] - obstacleSize / 2);
            glVertex2d(obstaclesX[i] + obstacleSize / 2, obstaclesY[i] - obstacleSize / 2);
            glVertex2d(obstaclesX[i] + obstacleSize / 2, obstaclesY[i] + obstacleSize / 2);
            glVertex2d(obstaclesX[i] - obstacleSize / 2, obstaclesY[i] + obstacleSize / 2);
            glEnd();
        }
    }
}

// Function to initialize obstacles
void initializeObstacles() {
    // Initialize random seed
    srand(time(NULL));

    for (int i = 0; i < maxObstacles; i++) {
        obstaclesX[i] = (double)(rand() % (int)(windowWidth - obstacleSize));
        obstaclesY[i] = 200.0 + (rand() % 150);
        isObstacleActive[i] = true;
    }
}

bool isCollision(double playerX, double playerY, double playerWidth, double playerHeight, double obstacleX, double obstacleY, double obstacleSize) {
    // Calculate the boundaries of the player
    double playerLeft = playerX - playerWidth / 2;
    double playerRight = playerX + playerWidth / 2;
    double playerTop = playerY + playerHeight / 2;
    double playerBottom = playerY - playerHeight / 2;

    // Calculate the boundaries of the obstacle
    double obstacleLeft = obstacleX - obstacleSize / 2;
    double obstacleRight = obstacleX + obstacleSize / 2;
    double obstacleTop = obstacleY + obstacleSize / 2;
    double obstacleBottom = obstacleY - obstacleSize / 2;

    // Check for collision by comparing boundaries
    if (playerRight < obstacleLeft || playerLeft > obstacleRight || playerTop < obstacleBottom || playerBottom > obstacleTop) {
        return false; // No collision
    }

    // If the player's boundaries intersect with the obstacle's boundaries, it's a collision
    return true;

}



void initializeCollectables() {
    // Initialize random seed
    srand(time(NULL));

    for (int i = 0; i < MAX_COLLECTABLES; i++) {
        // Generate random positions for the collectables
        collectablePositionsX[i] = (double)(rand() % (int)windowWidth);
        collectablePositionsY[i] = 200.0 + (rand() % 150);
        collectableSizes[i] = 20.0; // Set a fixed size for the triangles
        collectableRotations[i] = 0.0; // Start with no rotation
    }
}

void drawCollectables() {
    glColor3d(0.8, 0.6, 0.2); // Color for collectables (yellow bas drga mno kda msh yellow fa23 w 5las)

    for (int i = 0; i < MAX_COLLECTABLES; i++) {
        if (collectableSizes[i] > 0) {
            glPushMatrix();

            // Translate to the collectable's position
            glTranslated(collectablePositionsX[i], collectablePositionsY[i], 0);

            // Apply rotation around the y-axis
            glRotatef(collectableRotations[i], 0.0, 1.0, 0.0);

            // Draw a triangle for the collectable
            glBegin(GL_TRIANGLES);
            glVertex2d(0, 0);
            glVertex2d(10, 0);
            glVertex2d(5, 20);
            glEnd();

            glPopMatrix();
        }
    }
}

bool isCollectable(double playerX, double playerY, double playerWidth, double playerHeight, double collectableX, double collectableY, double collectableSize) {
    // Calculate the boundaries of the player
    double playerLeft = playerX - playerWidth / 2;
    double playerRight = playerX + playerWidth / 2;
    double playerTop = playerY + playerHeight / 2;
    double playerBottom = playerY - playerHeight / 2;

    // Calculate the boundaries of the collectable
    double collectableLeft = collectableX - collectableSize / 2;
    double collectableRight = collectableX + collectableSize / 2;
    double collectableTop = collectableY + collectableSize / 2;
    double collectableBottom = collectableY - collectableSize / 2;

    // Check for collision by comparing boundaries
    if (playerRight < collectableLeft || playerLeft > collectableRight || playerTop < collectableBottom || playerBottom > collectableTop) {
        return false; // No collision
    }

    // If the player's boundaries intersect with the collectable's boundaries, it's collectable
    return true;
}


// Function to draw power-ups as rotating squares
void drawPowerUp() {
    if (powerUpActive1) {
        glColor3d(0.5, 0, 0.5); // Purple color for Speed Up
        glPushMatrix();
        glTranslated(powerUpX1, powerUpY1, 0);
        glRotatef(powerUpRotationAngle1, 0.0, 0.0, 1.0);
        glBegin(GL_POLYGON);
        glVertex2d(-powerUpSize / 2, -powerUpSize / 2);
        glVertex2d(powerUpSize / 2, -powerUpSize / 2);
        glVertex2d(powerUpSize / 2, powerUpSize / 2);
        glVertex2d(-powerUpSize / 2, powerUpSize / 2);
        glEnd();
        glPopMatrix();
    }

    if (powerUpActive2) {
        glColor3d(1, 0.5, 0); // Orange color for Time Extend
        glPushMatrix();
        glTranslated(powerUpX2, powerUpY2, 0);
        glRotatef(powerUpRotationAngle2, 0.0, 0.0, 1.0);
        glBegin(GL_POLYGON);
        glVertex2d(-powerUpSize / 2, -powerUpSize / 2);
        glVertex2d(powerUpSize / 2, -powerUpSize / 2);
        glVertex2d(powerUpSize / 2, powerUpSize / 2);
        glVertex2d(-powerUpSize / 2, powerUpSize / 2);
        glEnd();
        glPopMatrix();
    }
}

// Function to handle power-up collection and effects
void handlePowerUp() {
    double distance1 = sqrt(pow(playerSpaceshipX - powerUpX1, 2) + pow(playerSpaceshipY - powerUpY1, 2));
    double distance2 = sqrt(pow(playerSpaceshipX - powerUpX2, 2) + pow(playerSpaceshipY - powerUpY2, 2));

    if (powerUpActive1 && distance1 < (playerSpaceshipWidth + powerUpSize) / 2) {
        // Player collected the Speed Up power-up
        powerUpActive1 = false; // Deactivate the power-up
        powerUpCollectedTime = glutGet(GLUT_ELAPSED_TIME); // Record the time of collection
        speedUpDuration = 10; // Set the duration of the Speed Up power-up to 10 seconds
        playerMovementSpeed *= 2; // Double the movement speed
    }

    if (powerUpActive2 && distance2 < (playerSpaceshipWidth + powerUpSize) / 2) {
        // Player collected the Time Extend power-up
        powerUpActive2 = false; // Deactivate the power-up
        // Apply Time Extend power-up effects
        gameDuration += 10; // Add 10 seconds to the game timer
    }


}

// Function to update the power-up rotation angle
void updatePowerUpRotation() {
    powerUpRotationAngle1 += 2.0; // Adjust the rotation speed as needed
    if (powerUpRotationAngle1 >= 360.0) {
        powerUpRotationAngle1 -= 360.0;
    }

    powerUpRotationAngle2 += 2.0; // Adjust the rotation speed as needed
    if (powerUpRotationAngle2 >= 360.0) {
        powerUpRotationAngle2 -= 360.0;
    }
}



// Initialize power-ups
void initializePowerUps() {
    // Initialize random seed
    srand(time(NULL));

    // Ensure that both Speed Up (Type 1) and Time Extend (Type 2) power-ups are available
    powerUpType1 = 1; // Type 1 is Speed Up
    powerUpType2 = 2; // Type 2 is Time Extend

    // Generate random positions for the two power-ups
    powerUpX1 = (double)(rand() % (int)(windowWidth - powerUpSize));
    powerUpY1 = 200.0 + (rand() % 150);
    powerUpX2 = (double)(rand() % (int)(windowWidth - powerUpSize));
    powerUpY2 = 200.0 + (rand() % 150);

    powerUpSize = 20.0; // Set a fixed size for the power-ups
    powerUpActive1 = true;
    powerUpActive2 = true;
    powerUpRotationAngle1 = 0.0;
    powerUpRotationAngle2 = 0.0;
}



void drawRedDotsBorder() {
    // Set the dot color (red)
    glColor3d(1, 0, 0);

    // Define the size of the dots
    double dotSize = 3.0;

    // Spacing between dots
    double spacing = 20.0;

    // Draw red dots outside of the border
    for (double x = 0; x <= windowWidth; x += spacing) {
        for (double y = 0; y <= windowHeight; y += spacing) {
            if (x < spacing || x > windowWidth - spacing || y < spacing || y > windowHeight - spacing) {
                glBegin(GL_POLYGON);
                for (int i = 0; i < 360; i += 30) {
                    double angle = i * 3.14159265 / 180.0;
                    double dotX = x + dotSize * cos(angle);
                    double dotY = y + dotSize * sin(angle);
                    glVertex2d(dotX, dotY);
                }
                glEnd();
            }
        }
    }
}

void drawBorder() {
    // Set the border color (blue)
    glColor3d(0, 0, 1);

    // Set the line width for the border
    glLineWidth(2.0);

    // Draw the border lines
    glBegin(GL_LINE_LOOP);
    glVertex2d(0, 0);                   // Bottom-left corner
    glVertex2d(windowWidth, 0);         // Bottom-right corner
    glVertex2d(windowWidth, windowHeight);  // Top-right corner
    glVertex2d(0, windowHeight);        // Top-left corner
    glEnd();


}

// Function to draw the door
void drawDoor() {
    glColor3d(1, 1, 1); // White color
    glBegin(GL_POLYGON);
    glVertex2d(doorX, doorY);
    glVertex2d(doorX + doorWidth, doorY);
    glVertex2d(doorX + doorWidth, doorY + doorHeight);
    glVertex2d(doorX, doorY + doorHeight);
    glEnd();

    // Draw a line inside the door's borders
    glColor3d(1, 0, 0); 
    glBegin(GL_LINES);


    glVertex2d(doorX, doorY + doorHeight / 2);
    glVertex2d(doorX + doorWidth, doorY + doorHeight / 2);

    glEnd();

    

    static double angle = 0.0; 
    angle += 1.0; // Increase the angle and adjusting the rotation speed
    // Define the coordinates of the triangle
    double centerX = doorX + doorWidth / 2;
    double centerY = doorY + doorHeight / 2;
    double triangleSize = doorWidth / 3; 


    glColor3d(0, 0, 1); 
    glBegin(GL_TRIANGLES); 

    for (int i = 0; i < 3; ++i) {
        double x = centerX + triangleSize * cos(angle + i * (2 * Pi / 3));
        double y = centerY + triangleSize * sin(angle + i * (2 * Pi / 3));
        glVertex2d(x, y);
    }

    glEnd();


    
    glColor3d(1, 0, 0); 
    glBegin(GL_POLYGON);

    // circle's properties
    int numSegments = 50; 
    double radius = doorWidth / 6; 

    for (int i = 0; i < numSegments; ++i) {
        double theta = 2.0 * Pi * i / numSegments;
        double x = centerX + radius * cos(theta);
        double y = centerY + radius * sin(theta);
        glVertex2d(x, y);
    }

    glEnd();

    
    static double pointAngle = 0.0; 
    pointAngle += 1.0; 

    // Calculate the position of the black point
    double pointX = centerX;
    double pointY = doorY + doorHeight - 10; 

    // Calculate the rotated position of the black point
    double pointRadius = doorWidth / 6;
    double rotatedPointX = centerX + pointRadius * cos(pointAngle * (Pi / 180));
    double rotatedPointY = pointY; 

    // Draw the rotated black point
    glColor3d(0, 0, 0); // Black color for the point
    glPointSize(5); // Set the point size
    glBegin(GL_POINTS);
    glVertex2d(rotatedPointX, rotatedPointY);
    glEnd();

    // Draw a line strip in the bottom half of the door
    glColor3d(1, 0, 0); // Red color for the line strip
    glBegin(GL_LINE_STRIP);

    double stripY = doorY + doorHeight / 4;
    for (double x = doorX; x <= doorX + doorWidth; x += 5) {
        glVertex2d(x, stripY);
    }

    glEnd();

}





// Function to check if the player has won
void checkPlayerWin() {
    // Define the bounding box for the player and the door
    double playerLeft = playerSpaceshipX - playerSpaceshipWidth / 2;
    double playerRight = playerSpaceshipX + playerSpaceshipWidth / 2;
    double playerTop = playerSpaceshipY + playerSpaceshipHeight / 2;
    double playerBottom = playerSpaceshipY - playerSpaceshipHeight / 2;

    double doorLeft = doorX;
    double doorRight = doorX + doorWidth;
    double doorTop = doorY + doorHeight;
    double doorBottom = doorY;

    // Check for collision between player and door
    if (playerRight >= doorLeft && playerLeft <= doorRight &&
        playerTop >= doorBottom && playerBottom <= doorTop) {
        playerWon = true;
    }
}




void drawBackground() {


    // Set the square color to white
    glColor3d(1.0, 1.0, 1.0);

    // Translate the origin to the center of the window

    glPushMatrix();
    rotationAngle += rotationSpeed;
    // Translate the origin to the center of the window
    glTranslated(windowWidth / 2, windowHeight / 2, 0);

    // Rotate the squares
    glRotatef(rotationAngle, 0, 0, 1);

    // Draw rotating squares around the origin
    for (int i = 0; i < numSquares; ++i) {
        double x = squareSize * i;
        double y = 0;

        // Draw a square at the (x, y) position
        glBegin(GL_POLYGON);
        glVertex2d(x - squareSize / 2, y - squareSize / 2);
        glVertex2d(x + squareSize / 2, y - squareSize / 2);
        glVertex2d(x + squareSize / 2, y + squareSize / 2);
        glVertex2d(x - squareSize / 2, y + squareSize / 2);
        glEnd();
    }

    glPopMatrix();
    //glFlush();
}













void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();


    // draw player's ship
    drawSpaceship(playerSpaceshipX, playerSpaceshipY, playerSpaceshipWidth, playerSpaceshipHeight, true);


        if (isGameOver) {
            glColor3f(1, 0, 0);
            char gameOverMsg[] = "Game Over!";
            print(450, 300, gameOverMsg);
        }
        else if (playerWon) {
            glColor3d(0, 1, 0); // Green text color

            char message[] = "Congratulations, you won!";
            print(450, 300, message);

            char scoreMessage[50];
            sprintf(scoreMessage, "Score: %d", playerScore);
            print(450, 250, scoreMessage);
        }
        else {
            glColor3f(1, 1, 1);
            char timeMsg[20];
            sprintf(timeMsg, "Time: %d", gameDuration);
            print(20, windowHeight - 30, timeMsg);


            // draw comp's ship
            drawSpaceship(computerSpaceshipX, computerSpaceshipY, computerSpaceshipWidth, computerSpaceshipHeight, false);

            // obstacles draw call hna 
            drawObstacles();
            for (int i = 0; i < maxObstacles; i++) {
                if (isObstacleActive[i] && isCollision(playerSpaceshipX, playerSpaceshipY, playerSpaceshipWidth, playerSpaceshipHeight, obstaclesX[i], obstaclesY[i], obstacleSize)) {

                    playerHealth -= 50; 
                    if (playerHealth <= 0) {

                        glColor3f(1, 0, 0);
                        char gameOverMsg[] = "Game Over!";
                        print(450, 300, gameOverMsg);
                        isGameOver = true;
                    }
                    // b3d el collision 5leha t5tfy
                    isObstacleActive[i] = false;
                }
            }




            drawCollectables();
            for (int i = 0; i < MAX_COLLECTABLES; i++) {
                if (collectableSizes[i] > 0 && isCollectable(playerSpaceshipX, playerSpaceshipY, playerSpaceshipWidth, playerSpaceshipHeight, collectablePositionsX[i], collectablePositionsY[i], collectableSizes[i])) {
 
                    playerScore += 10;
                    collectableSizes[i] = 0;
                }
            }

            // Call drawPowerUp 
            drawPowerUp();
            updatePowerUpRotation();
            handlePowerUp();

            int currentTime = glutGet(GLUT_ELAPSED_TIME);
            if (powerUpCollectedTime > 0 && (currentTime - powerUpCollectedTime) >= (speedUpDuration * 1000)) {
                // time up for the ability w dlw2ty deactivate it
                powerUpCollectedTime = 0; 
                playerMovementSpeed = 5.0; 
            }


            drawBorder();
            drawRedDotsBorder();

            drawDoor();
            checkPlayerWin();
           // gameWon();
 




            double healthBarX = windowWidth - 100;
            double healthBarY = windowHeight - 30;
            double healthBarWidth = 200;
            double healthBarHeight = 20;


            double playerHealthBarWidth = healthBarWidth * (playerHealth / maxPlayerHealth);


            // Draw the health bar background (gray)
            glColor3d(0.5, 0.5, 0.5);
            glBegin(GL_POLYGON);
            glVertex2d(healthBarX, healthBarY);
            glVertex2d(healthBarX + healthBarWidth, healthBarY);
            glVertex2d(healthBarX + healthBarWidth, healthBarY + healthBarHeight);
            glVertex2d(healthBarX, healthBarY + healthBarHeight);
            glEnd();

            // Draw the health part of the health bar (green)
            glColor3d(0, 1, 0);
            glBegin(GL_POLYGON);
            glVertex2d(healthBarX + healthBarWidth - playerHealthBarWidth, healthBarY);
            glVertex2d(healthBarX + healthBarWidth, healthBarY);
            glVertex2d(healthBarX + healthBarWidth, healthBarY + healthBarHeight);
            glVertex2d(healthBarX + healthBarWidth - playerHealthBarWidth, healthBarY + healthBarHeight);
            glEnd();

            // To display the score
            glColor3f(1, 0, 0);
            char p0s[20];
            sprintf(p0s, "Score: %d", playerScore);
            print(450, 570, p0s); // Adjusted position for upper center
        }

    glFlush();
}



void gameTimer(int value) {

    // Decrement the game duration timer
    gameDuration--;

    if (gameDuration <= 0) {
        // Game over because the time is up, implement game over logic here
        isGameOver = true;
    }

    // Redraw the scene
    glutPostRedisplay();

    // Set the timer callback to update again after a specified time
    glutTimerFunc(1000, gameTimer, 0); // 1 second
}

void computerTimer(int value) {


    // Redraw the scene
    glutPostRedisplay();

    // Set the timer callback to update again after a specified time
    glutTimerFunc(1000/60, computerTimer, 0); // 60 frames per second
}

void SpecialKeys(int key, int x, int y) {
    // Temporary variables to store the new player spaceship position
    double newPlayerSpaceshipX = playerSpaceshipX;
    double newPlayerSpaceshipY = playerSpaceshipY;

    if (isGameOver) {
        // Prevent player movement when the game is over
        return;
    }

    // Handle arrow key presses to control the player spaceship's movement
    switch (key) {
    case GLUT_KEY_RIGHT:
        newPlayerSpaceshipX += playerMovementSpeed;
        break;
    case GLUT_KEY_LEFT:
        newPlayerSpaceshipX -= playerMovementSpeed;
        break;
    case GLUT_KEY_UP:
        newPlayerSpaceshipY += playerMovementSpeed;
        break;
    case GLUT_KEY_DOWN:
        newPlayerSpaceshipY -= playerMovementSpeed;
        break;
    }

 // Check if the new player spaceship position is within the window boundaries
    if (newPlayerSpaceshipX - playerSpaceshipWidth / 2 >= 10 &&
        newPlayerSpaceshipX + playerSpaceshipWidth / 2 <= windowWidth -15 &&
        newPlayerSpaceshipY + playerSpaceshipHeight / 2 <= windowHeight -22 &&
        newPlayerSpaceshipY - playerSpaceshipHeight / 2 >= 0) {
        playerSpaceshipX = newPlayerSpaceshipX;
        playerSpaceshipY = newPlayerSpaceshipY;
    }


    // Redraw the scene
    glutPostRedisplay();
}

void ComputerMove(int value) {
    // Simulate computer spaceship movement
    computerSpaceshipX += computerMovementSpeed;

    // Check if the computer spaceship's position is within the window boundaries
    if (computerSpaceshipX - computerSpaceshipWidth / 2 <= 0 || computerSpaceshipX + computerSpaceshipWidth / 2 >= windowWidth) {
        // Reverse direction if the computer spaceship reaches the window boundaries
        computerMovementSpeed = -computerMovementSpeed;
    }

    // Ensure the computer spaceship stays within its y-coordinate boundaries
    if (computerSpaceshipY > computerMaxY) {
        computerSpaceshipY = computerMaxY;
    }
    else if (computerSpaceshipY < computerMinY) {
        computerSpaceshipY = computerMinY;
    }

    glutTimerFunc(100, ComputerMove, 0);
}

void playMusic() {
    if (isCollision) {
        sndPlaySound(NULL, SND_ASYNC);
        //PlaySound(NULL, NULL, 0);
        sndPlaySound(TEXT("Heavy object Hit and body thud sound effect.wav"), SND_ASYNC);
        sndPlaySound(TEXT("Better call saul - Main theme (8bit version).wav"), SND_ASYNC | SND_LOOP);
    }
    else {
        sndPlaySound(TEXT("Better call saul - Main theme (8bit version).wav"), SND_ASYNC | SND_LOOP);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitWindowSize(1000, 600);

    glutCreateWindow("Space War");
    glutDisplayFunc(Display);
    glutSpecialFunc(SpecialKeys);  // special keys callback
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gluOrtho2D(0.0, 1000, 0.0, 600);

    // Set the timer callback to update the game timer
    glutTimerFunc(1000, gameTimer, 0);
    // Set the initial timer callback to start the animation loop
    glutTimerFunc(0, computerTimer, 0);

    // Start computer spaceship movement
    glutTimerFunc(100, ComputerMove, 0);
    initializeCollectables();
    initializePowerUps();
    initializeObstacles();
    playMusic();





    glutMainLoop();
    return 0;
}