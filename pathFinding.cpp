#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <cmath>

using namespace std;

class Point
{
	private:
		string name;
		double x;
		double y;
	public:
		Point()
		{}

		Point(string id, double x_in, double y_in)
		{
			name = id;
			x = x_in;
			y = y_in;
		}

		Point(double x_in, double y_in)
		{
			x = x_in;
			y = y_in;
		}

		string getName()
		{ return name; }

		double getX()
		{ return x; }

		double getY()
		{ return y; }

		void setName(string id)
		{ name = id; }

		void setX(double x_in)
		{ x = x_in; }

		void setY(double y_in)
		{ y = y_in; }
};

class Line
{
	private:
		Point *point1;
		Point *point2;
		double length;
		double angle;
	public:
		Line(Point *p1, Point *p2)
		{
			point1 = p1;
			point2 = p2;
			length = sqrt(pow((point1->getX() - point2->getX()), 2) + 
				      pow((point1->getY() - point2->getY()), 2));
			angle = (point2->getY() - point1->getY()) / 
				(point2->getX() - point1->getX()); 
		}

		double getLength()
		{ return length; }

		double getAngle()
		{ return angle; }

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
	int numObs = 3;			// number of obstacles, in this case, 3 rocks
	double radiusMin = 1.25;	// minimum value of rock's radius
	double radiusMax = 1.5;		// maximum value of rock's radius
	Point obs[numObs + 2];     	// an array contains the positon of obstacle + 2 boundary lines
	double rad[numObs + 2];  	// an array contains radius of obstacle
	bool found = false;		// found == true if we can find the possible path
	vector<double> path_X; 		// a vector contains the x value of possible paths	

	// create the left and right boundary as obstacles
	obs[0].setX(0.0);		 	// left boundary, x = 0
	rad[0] = 0.0;
	obs[numObs + 1].setX(mapWidth);     	// right boundary, x = mapWidth
	rad[numObs + 1] = 0.0;

	srand(time(NULL));

	// create numObs obstacles and assign to it's division
	for (int i = 0; i < numObs; i++)
	{
		double divStart = (mapWidth / numObs) * i;	// division begin line
		double divEnd = (mapWidth / numObs) * (i + 1);  // division end line
		double r = fRand(radiusMin, radiusMax);		// find the radom btw max and min
		double px = fRand(divStart, divEnd);		// random value of x in its division
		double py = fRand(mapLength / 3, mapLength * 2 / 3); // random value of y
		
		// set the position of obstacles by x and y	
		obs[i + 1].setX(px);    
		obs[i + 1].setY(py);
		rad[i + 1] = r;		// set value of radius
	}

	// print out the obstacles and their radius and their x, y value
	for(int i = 0; i < numObs + 2; i++)
	{
		cout << "Rock " << i + 1 << ": " << "  radius: " << rad[i]
		     << "  x: " << obs[i].getX() << "  y: " << obs[i].getY() << endl;
	}

	// find the center of the distance between each obstacles 
	// and check can we put the robot between those obstacles
	for (int i = 0; i <= numObs; i++)
	{
		double distance;	// distance between two obstacles
		distance = obs[i + 1].getX() - obs[i].getX() - rad[i + 1] - rad[i];
		
		// if the robot can fit into the distance between two obstacles
		// put the x value of the center of two obstacles into path_X vector
		if (distance > robotWidth)
		{
			path_X.push_back(obs[i].getX() + distance / 2);	
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

	double target_Y = robot_Y;                // the y value of the point where the robot has to turn
	double target_X = closetPath;             // the x value of the point where the robot has to turn
	Point target(target_X, target_Y);         // create a point from the target point

	// move the turning point along the y-axis of the path we just find
	while (target_Y < mapLength)
	{
		for (int i = 1; i < numObs; i++)
		{
			if (target_Y >= obs[i].getY())
			{
				Line y(&target, &robot);		// draw a line from the turning point to the robot
				double temp = pointToLine(obs[i], y);	// calculate the distance from the rock to the line btw robot and turning point
				if (temp < robotWidth + radiusMax)
					goto finish;			// if we can find the target exit the loop
			}
		}
		target_Y += 0.2;
		target.setY(target_Y);        	// set new turning point with new  y value
	}

	finish:
	cout << "The Turning Point is: " 
	     << "x: " << target_X << "  y: " << target_Y << endl;



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
	double a1 = -1 / l.getAngle();            
	double b1 = p.getY() - a1 * p.getX();
	double a2 = l.getAngle();
	double b2 = l.getB();
	double x, y;
	double distance; 	// distance from ponint to line
	
	solveTwoEquations(a1, b1, a2, b2, x, y);

	distance = sqrt(pow((p.getX() - x), 2) + pow((p.getY() - y), 2));
	return distance;
}	

/**  find the distance between a point to a line
  *  @param p the point
  *  @param l the line
  */
void solveTwoEquations(double a1, double b1, double a2, double b2, double x, double y)
{
	double determinant = -a1 + a2;
	x = (b1 - b2) / determinant;
	y = (-a1 * b2 + a2 * b1) / determinant;
}
