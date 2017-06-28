#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "Eigen/Dense"

using namespace std;
using namespace Eigen;

MatrixXd fromFile(const string &input_file_path) {
  // Reads in the file from "input_file_path"
  ifstream input_file(input_file_path, ifstream::in);
  MatrixXd m;
  string line;
  vector<float> temp_buffer;
  float coef;
  int num_cols = 0;
  int num_rows = 0;
  int cols_in_row;
  if (input_file) {
    // Iterates over every line in the input file
    while (!input_file.eof())
    {
      getline(input_file, line);
      if (line.find_first_not_of(' ') == std::string::npos)
        continue;
      replace(line.begin(), line.end(), ',', ' ');
      // Creates a stringstream out of every line in the file
      stringstream stream(line);
      cols_in_row = 0;
      // Reads every coefficient in the stringstream into the temporary buffer
      while (stream >> coef)
      {
         temp_buffer.push_back(coef);
         ++cols_in_row;
      }
      // If the number of columns in the matrix hasn't been set, make it the
      // current number of columns in the row
      if (num_cols == 0)
      {
         num_cols = cols_in_row;
         // If the matrix in the file is shaped incorrectly, throw an error
      }
      else if (num_cols != cols_in_row)
      {
          cerr << "Problem with Matrix in: " + input_file_path +
                           ", exiting..." << endl;
          exit(1);
      }
      ++num_rows;
    }
     // Instantiate the matrix's size and feed it the coefficients in the
     // temporary buffer
     m.resize(num_rows, num_cols);
     for (int i = 0; i < num_rows; ++i)
    	 for (int j = 0; j < num_cols; ++j)
    		 m(i, j) = temp_buffer[i * num_cols + j];
     return m;
     }
    else
    {
       // Error for when the file doesn't exist
       cerr << "Cannot open file " + input_file_path + ", exiting..."<< endl;
       exit(1);
     }
}

VectorXd fromFile(const string &input_file_path, int num_elements)
{
  ifstream input_file(input_file_path, ifstream::in);
  VectorXd m(num_elements);
  if (input_file) {
    for(int i = 0; i < num_elements; i++)
        input_file >> m(i);
    return m;
  } else {
    cerr << "Cannot open file " + input_file_path + ", exiting..." <<endl;
    exit(1);
  }
}

//gives the sum of squared error for two vectors
float SSE(VectorXd b, VectorXd bTest, int bRows) //do i need to put in the location?
{
	float sseNum = 0;
	for(int i = 0; i < 20; i++)
	{
		sseNum = sseNum + pow((b(i) - bTest(i)), 2.0);
	}
	return sseNum;
}

int main(){
	//defines the matrices and vectors
	VectorXd b;
	MatrixXd x (1000,100);
	x = fromFile("/home/mackenzie/practiceCode/X-12321.csv");
	VectorXd y (1000);
	y = fromFile("/home/mackenzie/practiceCode/y-26465.csv", 1000);
	VectorXd bTest(100);
	bTest = fromFile("/home/mackenzie/practiceCode/beta-12566.csv", 100);


	/*prints the matrix and vector thus far
	cout << "The matrix is " << x << endl;
	cout << "The vector is " << endl << y << endl;*/

	//the formula
	MatrixXd xTransposed = x.transpose();
	MatrixXd bPart1 = xTransposed*x;
	MatrixXd bPart1Inv = bPart1.inverse();
	VectorXd bPart2 = xTransposed*y;
	b = bPart1Inv*bPart2;
	int bRows = b.size();

	//cout << "The MLE estimator is " << endl <<b << endl; prints the b values we got

	float sum_of_squares = SSE(b, bTest, bRows);
	cout << "The sum of squared error is " << sum_of_squares << endl;
}
