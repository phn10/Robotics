#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <cmath>

using namespace std;

/**
  * this class represent a point
  */
class Point
{
	private:
		double x;              // x value of this point
		double y;              // y value of this point
	public:
		/**
		  * constructor
		  */
		Point()
		{}

		/**
		  * constructor
		  * @param x_in x value of this point
		  * @param y_in y value of this point
		  */
		Point(double x_in, double y_in)
		{
			x = x_in;
			y = y_in;
		}

		/**
		  * get the value of x
		  */
		double getX()
		{ return x; }

		/**
		  * get the value of y
		  */
		double getY()
		{ return y; }

		/**
		  * set the value of x
		  * @param x_in x value of this point
		  */
		void setX(double x_in)
		{ x = x_in; }

		/**
		  * set the value of y
		  * @param y_in y value of this point
		  */
		void setY(double y_in)
		{ y = y_in; }
};

/**
  * this class represents a line by drawing a line between two points
  */
class Line
{
	private:
		Point *point1;       // point1 
		Point *point2;       // point2 
		double length;       // length of this secment
		double angle;        // the angle of this line
	public:
		
		/**
		  * constructor
		  * @param p1 point1
		  * @param p2 point2
		  */
		Line(Point *p1, Point *p2)
		{
			point1 = p1;          // point1
			point2 = p2;          // point2
			length = sqrt(pow((point1->getX() - point2->getX()), 2) + 
				      pow((point1->getY() - point2->getY()), 2));
			angle = (point2->getY() - point1->getY()) / 
				(point2->getX() - point1->getX()); 
		}

		/**
		  * get the length of the secment between point1 and point2 
		  */
		double getLength()
		{ return length; }

		/**
		  * get the angle of this line
		  */	
		double getAngle()
		{ return angle; }

		/**
		  * get the value of b in y = ax + b
		  */
		double getB()
		{ return point2->getY() - point2->getX() * angle; }
};

double fRand(double , double );		// generate a random double number between two double numbers
double pointToLine(Point, Line);	// find the distance from a point to a line
void solveTwoEquations(double , double , double , double , double , double);

int main()
{
	double robot_X = 5.0;		// initial x position of robot
	double robot_Y = 1.0;		// initial y position of robot
	Point robot(robot_X, robot_Y);	// point of the initial position of robot
	double robotWidth = 0.5;	// the width of robot 
	double mapLength = 20.0;	// length of the map
	double mapWidth = 10.0;		// width of the map
	int numRocks = 3;		// number of rocks, in this case, 3 rocks
	double radiusMin = 1.25;	// minimum value of rock's radius
	double radiusMax = 1.5;		// maximum value of rock's radius
	Point rocks[numRocks + 2];     	// an array contains the positon of obstacle + 2 boundary lines
	double rad[numRocks + 2];  	// an array contains radius of obstacle
	bool found = false;		// found == true if we can find the possible path
	vector<double> path_X; 		// a vector contains the x value of possible paths	

	// create the left and right boundary as obstacles
	rocks[0].setX(0.0);		 	// left boundary, x = 0
	rad[0] = 0.0;
	rocks[numRocks + 1].setX(mapWidth);     	// right boundary, x = mapWidth
	rad[numRocks + 1] = 0.0;

	srand(time(NULL));

	// create numObs obstacles and assign to it's division
	for (int i = 0; i < numRocks; i++)
	{
		double divStart = (mapWidth / numRocks) * i;	// division begin line
		double divEnd = (mapWidth / numRocks) * (i + 1);  // division end line
		double r = fRand(radiusMin, radiusMax);		// find the radom btw max and min
		double px = fRand(divStart, divEnd);		// random value of x in its division
		double py = fRand(mapLength / 3, mapLength * 2 / 3); // random value of y
		
		// set the position of obstacles by x and y	
		rocks[i + 1].setX(px);    
		rocks[i + 1].setY(py);
		rad[i + 1] = r;		// set value of radius
	}

	// print out the obstacles, their radius and their x, y value
	for(int i = 0; i < numRocks + 2; i++)
	{
		cout << "Rock " << i + 1 << ": " << "  radius: " << rad[i]
		     << "  x: " << rocks[i].getX() << "  y: " << rocks[i].getY() << endl;
	}

	// find the center of the distance between each obstacles 
	// and check can we put the robot between those obstacles
	for (int i = 0; i < numRocks + 1; i++)
	{
		double distance;	// distance between two obstacles
		distance = rocks[i + 1].getX() - rocks[i].getX() - rad[i + 1] - rad[i];
		cout << "Rock" << i + 1 << " and Rock" << i + 2 << ": " << distance << endl;	
		// if the robot can fit into the distance between two obstacles
		// put the x value of the center of two obstacles into path_X vector
		if (distance > robotWidth)
		{
			path_X.push_back(rocks[i].getX() + rad[i] + distance / 2);	
			found = true;
		}
	}
		
	// if cannot find the any empty space, print out error and close the program
 	if (found == false)
	{
		cout << "ERROR: Canont find path!!!" << endl;
		return 1;
	}
	
	// list number of possible path, they are elements in path_X vector
	for (int i = 0; i < path_X.size(); i++)
	{
		cout << "Path x: " << path_X[i] << endl;
	}

	double distanceToPath = mapWidth;	// distance from robot to possible path
	double closetPath;                      // distance from robot to the closet path

	for (int i = 0; i < path_X.size(); i++)
	{
		double temp = abs(path_X[i] - robot_X);
		if (temp < distanceToPath)
		{
			distanceToPath = temp;
			closetPath = path_X[i];
		}
	}

	// print out the closet path to the robot
	cout << "Closest path: " << closetPath << " with: " 
	     << distanceToPath << " from robot" << endl;

	// we assume the safe distance from  robot to obstacles is radiusMax

	double turning_Y = robot_Y;                // the y value of the point where the robot has to turn
	double turning_X = closetPath;             // the x value of the point where the robot has to turn
	Point turning(turning_X, turning_Y);       // create a turning point

	// move the turning point along the y-axis of the path we just find
	while (turning_Y < mapLength)
	{
		cout << "y: " << turning_Y << endl;
		// find the distance between the line, turning point to the robot,
		// and each rocks
		for (int i = 1; i < numRocks + 1; i++)
		{
			if (turning_Y >= rocks[i].getY() - radiusMax)
			{
				cout << "Rock" << i << ": ";
				Line y(&turning, &robot);		// draw a line from the turning point to the robot
				double temp = pointToLine(rocks[i], y);	// calculate the distance from the rock to the line btw robot and turning point
				cout << temp << endl;
				if (temp < robotWidth + radiusMax)
					goto finish;			// if we can find the target exit the loop
			}
		}
		turning_Y += 0.2;                // increase the y value of turning point by 0.2 
		turning.setY(turning_Y);        	// set new turning point with new  y value
	}

	finish:
	cout << "The Turning Point is: " 
	     << "x: " << turning_X << "  y: " << turning_Y << endl;

	
	return 0;
}

/**  create random number between two double value
  *  @param fMin the smaller double number
  *  @param fMax the bigger double number
  */
double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

/**  find the distance between a point to a line
  *  @param p the point
  *  @param l the line
  */
double pointToLine(Point p, Line l)
{
	// create a perpendicular line to l
	double a1 = -1 / l.getAngle();         // angular of the perpendicular line   
	double b1 = p.getY() - a1 * p.getX();  // the b value of the perpendicular line     
	double a2 = l.getAngle();              // angular of line 2
	double b2 = l.getB();                  // the b value of line 2
	double x, y;            // x and y position of the intersect point        
	double distance; 	// distance from ponint to line 
	
	x = (b2 - b1) / (a1 - a2);
	y = a2 * x + b2;

	Point intersect(x, y);  // intersect point
	Line line1(&p, &intersect);	 // line between point p and intersect point
	distance = line1.getLength();    // find the distance of the secment
	return distance;
}	
