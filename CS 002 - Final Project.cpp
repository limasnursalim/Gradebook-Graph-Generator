//File Name: CS 002 - Final Project.cpp
//Author: Limas Nursalim
//Email: lnursalim@go.pasadena.edu
//Assignment: Final Project
//Description: Program to output gradebook as a graph
//Date Changed: June 09, 2019

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <cctype>
#include <string>

using namespace std;
const int MAX_COLUMN(15), MAX_ROW(60), OUTER_WIDTH(500), OUTER_HEIGHT(500), INNER_WIDTH(400), INNER_HEIGHT(400);

//FUNCTION DECLARATION
void open_file(ifstream& fin, ofstream& fout, ofstream& fout_graph_averageTest, ofstream& fout_graph_averageStudent, ofstream& fout_graph_sortedAverageTest);
//function to open input file and output file
//pre-condition: no file is opened
//post-condition: input and output file is opened
void read_and_store_arrays(ifstream& fin, string lastName, string firstName, double grade[][MAX_COLUMN], string name[][MAX_COLUMN], int& row_used, int& column_used);
//function store grade arrays & name arrays AND determine row used and column used
//pre-condition: none
//post-condition: filled grade & name arrays AND determined row & column used
void validate_input(int column_used, int row_used);
//function to validate input if the file is empty, grade and name is more than maximum input allowed
//pre-condition: none
//post-condition: validate if the input file is empty, grade and name is more than maximum input allowed
double convert_string_to_double(string& grades);
//function to convert grades read as string to double
//pre-condition: grades is string
//post-condition: grades is double
void average_calculation(double grade[][MAX_COLUMN], double averageTest[], double averageStudent[], int row_used, int column_used);
//function to fill average of each test and average of each student arrays
//pre-condition: averageTest and averageStudent arrays are not filled
//post-condition: averageTest and averageStudent arrays are filled
void generateTestNumber(int testNumber[MAX_COLUMN], int column_used);
//function to generate test number based on how many grades were read
//pre-condition: no test number for each test
//post-condition: there's test number for each test based on column_used
void output_table(ofstream& fout, double grade[][MAX_COLUMN], string name[][MAX_COLUMN], double averageStudent[MAX_ROW], int row_used, int column_used);
//function to output an organized table into another file
//pre-condition: table is not organized (from input file)
//post-condition: table is organized

/*** SCALING ***/
void scaling_graph_WIDTH(int columnOrRow, double& width_averageTestOrStudent);
//function to scale the graph width of bar and spaces, depending on how many column or row is used. If less column/row is used, then the bar will be wider.
//pre-condition: width of graph is undecided
//post-condition: width of graph is counted based on how many column or row are used, so that it may be more proportional
void scaling_graph_HEIGHT(double averageTestOrStudent[MAX_COLUMN], double averageTestOrStudent_Scaled_Bottom[MAX_COLUMN], double averageTestOrStudent_Scaled_Top[MAX_COLUMN], int columnOrRow);
//function to scale the graph height of bar and spaces, depending on how many column or row is used. We have to scale the top and bottom (height) of bar. Top means the y-coordinate from top of border (bc SVG count y-coordinate from top). Bottom means the height of the bar, the scaled height of bar depend on the biggest number to be the standard of scale.
//pre-condition: Top and bottom of bar is not scaled yet
//post-condition: Top and bottom of bar is scaled
void x_coordinate(double x_coordinate_graph_averageTestOrStudent[MAX_COLUMN], double width_averageTestOrStudent, int columnOrRow);
//function to determine the position of x-coordinate of each bar depending on the amount of column or row.
//pre-condition: x-coordinate position of each bar is undecided.
//post-condition: x-coordinate position of each bar is calculated.
void colorChanges_graph(double averageTestOrStudent_Scaled_Bottom[MAX_COLUMN], double RGB[][MAX_COLUMN], double sorted_averageTestOrStudent_Scaled_Bottom[MAX_COLUMN], int columnOrRow);
//function to change (and store) the color of RGB to decay from red to green or vice versa
//pre-condition: RGB 2D array is not initialized
//post-condition: RGB 2D array is initialized based on the rank of bar
int checkRank_graph(double current_averageTestOrStudent_Scaled_Bottom, double sorted_averageTestOrStudent_Scaled_Bottom[MAX_COLUMN], int columnOrRow);
//function to check current rank of each bar
//pre-condition: rank of each bar is unknown
//post-condition: rank of each bar is returned
void assign_RGB_value(double& red, double& green, int rank, double RGB[][MAX_COLUMN]);
//function to assign the RGB value to red and green variable depending on each bar's rank
//pre-condition: red and green variable is uninitialized
//post-condition: red and green variable is initialized based on the rank

/*** GRAPH - AVERAGE TEST ***/
void output_graph_averageTest(ofstream& fout_graph_averageTest, double averageTest_Scaled_Bottom[MAX_COLUMN], double averageTest_Scaled_Top[MAX_COLUMN], double x_coordinate_graph_averageTest[MAX_COLUMN], double sorted_averageTest_Scaled_Bottom[MAX_COLUMN], double averageTest[MAX_COLUMN], int testNumber[MAX_COLUMN], double& width_averageTest, int column_used);
//function to output the graph of average test
//pre-condition: none
//post-condition: graph of average test

/*** GRAPH - AVERAGE STUDENT ***/
void output_graph_averageStudent(ofstream& fout_graph_averageStudent, double averageStudent_Scaled_Bottom[MAX_ROW], double averageStudent_Scaled_Top[MAX_ROW], double x_coordinate_graph_averageStudent[MAX_ROW], double averageStudent[MAX_ROW], string name[MAX_ROW][MAX_COLUMN], double& width_averageStudent, int row_used);
//function to output the graph of average student
//pre-condition: none
//post-condition: graph of average student

/*** GRAPH - SORTED AVERAGE TEST ***/
void output_graph_sortedAverageTest(ofstream& fout_graph_sortedAverageTest, double averageTest_Scaled_Bottom[MAX_COLUMN], double averageTest_Scaled_Top[MAX_COLUMN], double x_coordinate_graph_averageTest[MAX_COLUMN], double sorted_averageTest_Scaled_Bottom[MAX_COLUMN], double averageTest[MAX_COLUMN], int testNumber[MAX_COLUMN], double& width_averageTest, int column_used);
//function to output the graph of sorted average test
//pre-condition: none
//post-condition: graph of sorted average test

void sort_rank(int column_used, double averageTest_Scaled_Top[MAX_COLUMN], double sorted_averageTest_Scaled_Top[MAX_COLUMN], double averageTest[MAX_COLUMN], double sorted_averageTest[MAX_COLUMN], int testNumber[MAX_COLUMN]);
//function to sort the rank of average test and test number
//pre-condition: average test and test number is unsorted
//post-condition: average test and test number is sorted

//MAIN
int main()
{
    ifstream fin;
    ofstream fout, fout_graph_averageTest, fout_graph_averageStudent, fout_graph_sortedAverageTest;
    
    string firstName = {"-"}, lastName = {"-"}, name[MAX_ROW][MAX_COLUMN] = {"-"};
    double grade[MAX_ROW][MAX_COLUMN] = {0}, averageTest[MAX_COLUMN], averageStudent[MAX_ROW];
    int testNumber[MAX_COLUMN];
    int row_used(0), column_used(0);
    
    /*** GRAPH - AVERAGE TEST ***/
    double averageTest_Scaled_Bottom[MAX_COLUMN], sorted_averageTest_Scaled_Bottom[MAX_COLUMN], averageTest_Scaled_Top[MAX_COLUMN], x_coordinate_graph_averageTest[MAX_COLUMN];
    double width_averageTest(0);
    
    /*** GRAPH - AVERAGE STUDENT ***/
    double averageStudent_Scaled_Bottom[MAX_ROW], averageStudent_Scaled_Top[MAX_ROW], x_coordinate_graph_averageStudent[MAX_COLUMN];
    double width_averageStudent(0);
    
    open_file(fin, fout, fout_graph_averageTest, fout_graph_averageStudent, fout_graph_sortedAverageTest);
    read_and_store_arrays(fin, lastName, firstName, grade, name, row_used, column_used);
    average_calculation(grade, averageTest, averageStudent, row_used, column_used);
    generateTestNumber(testNumber, column_used);
    output_table(fout, grade, name, averageStudent, row_used, column_used);
    
    /*** GRAPH - AVERAGE TEST ***/
    scaling_graph_WIDTH(column_used, width_averageTest);
    scaling_graph_HEIGHT(averageTest, averageTest_Scaled_Bottom, averageTest_Scaled_Top, column_used);
    x_coordinate(x_coordinate_graph_averageTest, width_averageTest, column_used);
    output_graph_averageTest(fout_graph_averageTest, averageTest_Scaled_Bottom, averageTest_Scaled_Top, x_coordinate_graph_averageTest, sorted_averageTest_Scaled_Bottom, averageTest, testNumber, width_averageTest, column_used);
    
    /*** GRAPH - AVERAGE STUDENT ***/
    scaling_graph_WIDTH(row_used, width_averageStudent);
    scaling_graph_HEIGHT(averageStudent, averageStudent_Scaled_Bottom, averageStudent_Scaled_Top, row_used);
    x_coordinate(x_coordinate_graph_averageStudent, width_averageStudent, row_used);
    output_graph_averageStudent(fout_graph_averageStudent, averageStudent_Scaled_Bottom, averageStudent_Scaled_Top, x_coordinate_graph_averageStudent, averageStudent, name, width_averageStudent, row_used);
    
    /*** GRAPH - SORTED AVERAGE STUDENT ***/
    output_graph_sortedAverageTest(fout_graph_sortedAverageTest, averageTest_Scaled_Bottom, averageTest_Scaled_Top, x_coordinate_graph_averageTest, sorted_averageTest_Scaled_Bottom, averageTest, testNumber, width_averageTest, column_used);
    
    return 0;
}

//FUNCTION DEFINITION
void open_file(ifstream& fin, ofstream& fout, ofstream& fout_graph_averageTest, ofstream& fout_graph_averageStudent, ofstream& fout_graph_sortedAverageTest)
{
    fin.open("gradebook.txt"); //open input file
    fout.open("Gradebook Table Output.txt"); //open output file for table
    fout_graph_averageTest.open("Average Test Graph.svg"); //open output file for average test graph
    fout_graph_averageStudent.open("Average Student Graph.svg"); //open output file for average student graph
    fout_graph_sortedAverageTest.open("Sorted Average Test Graph.svg"); //openn output for sorted average test graph
    
    if (fin.fail() || fout.fail() || fout_graph_averageTest.fail() || fout_graph_averageStudent.fail() || fout_graph_sortedAverageTest.fail()) //validation
    {
        cout << "Error! File not found!" << endl;
        exit(1);
    }
}

void read_and_store_arrays(ifstream& fin, string lastName, string firstName, double grade[][MAX_COLUMN], string name[][MAX_COLUMN], int& row_used, int& column_used)
{
    string scores_string("0");
    double scores_double(0);
    int row_grade(0), column_grade(0), row_name(0), column_name(0), counter(0);
    
    while (!fin.eof())
    {
        column_grade = 0;
        column_name = 0;
        
        fin >> lastName >> firstName;
        fin.ignore();
        
        //STORE NAME INTO ARRAYS
        if (column_name == 0)
            name[row_name][column_name] = lastName;
        column_name++;
        if (column_name == 1)
            name[row_name][column_name] = firstName;
        
        //READ GRADES FROM FILE
        while (isdigit(fin.peek()))
        {
            fin >> scores_string; //read score as a string
            scores_double = convert_string_to_double(scores_string); //convert grade strings into double
            
            //STORE GRADE INTO ARRAYS
            grade[row_grade][column_grade] = scores_double;
            column_grade++;
            
            fin.ignore(); //ignore spaces before another digit
            
            column_used++;
            counter++;
        }
        row_grade++;
        row_name++;
        
        row_used++; //find the row used if the user did not use the max amount of rows
    }
    column_used /= row_used; //find the column used if the user did not use the max amount of column
    validate_input(column_used, row_used);
}

void validate_input(int column_used, int row_used)
{
    if (column_used > MAX_COLUMN)
    {
        cout << "Grade Exceeded! Input file grade can't be more than " <<  MAX_COLUMN << ".\n";
        exit(1);
    }
    
    if (row_used > MAX_ROW)
    {
        cout << "Name limit exceeded! Input file name can't be more than " << MAX_ROW << ".\n";
        exit(1);
    }
    
    if (column_used == 0 || row_used == 0)
    {
        cout << "File empty! Please input name and grade into file!\n";
        exit(1);
    }
}

double convert_string_to_double(string& grades)
{
    double score(0);
    score = stoi(grades);
    
    return (score);
}

void average_calculation(double grade[][MAX_COLUMN], double averageTest[], double averageStudent[], int row_used, int column_used)
{
    double totalAverageTest[MAX_COLUMN] = {0}, totalAverageStudent[MAX_ROW] = {0};
    
    for (int i=0; i<row_used; i++)
        for (int j=0; j<column_used; j++)
            totalAverageStudent[i] += grade[i][j]; //count total average student vertically

    for (int i=0; i<row_used; i++)
        for (int j=0; j<column_used; j++)
            totalAverageTest[j] += grade[i][j]; //count total average each test horizontally
    
    //STORE AVERAGE STUDENT GRADE ARRAYS
    for (int i=0; i<row_used; i++)
        averageStudent[i] = (totalAverageStudent[i]/static_cast<double>(column_used));
    
    //STORE AVERAGE EACH TEST ARRAYS
    for (int i=0; i<column_used; i++)
        averageTest[i] = (totalAverageTest[i]/static_cast<double>(row_used));
}

void generateTestNumber(int testNumber[MAX_COLUMN], int column_used)
{
    int counter(1);
    for (int i=0; i<column_used; i++)
    {
        testNumber[i] = counter;
        counter++;
    }
}

void output_table(ofstream& fout, double grade[][MAX_COLUMN], string name[][MAX_COLUMN], double averageStudent[MAX_ROW], int row_used, int column_used)
{
    string printDashedLine(115,'-');
    int counter(0);
    
    //PRINT HEADER FORMAT
    fout << printDashedLine << endl;
    fout << setw(9) << "Last Name" << setw(15) << "First Name" << setw (40) << "Score" << setw(50) <<  "Average" << endl;
    fout << printDashedLine << endl;
    
    //OUTPUTTING TABLE
    for (int i=0; i<row_used; i++)
    {
        counter = 0; //set counter back to 0 for every new row
        for (int j=0; j<3; j++) //j<3 because print 3x: name and grades and average
        {
            if (counter == 0)
            {
                fout.setf(ios:: left);
                for (int k=0; k<2; k++) //k<2 because print twice: last name and first name
                {
                    fout << setw(14) << name[i][k];
                }
                counter++;
                fout.unsetf(ios::left);
            }
            else if (counter == 1)
            {
                for(int l=0; l<column_used; l++) //l<column_used bc print "column_used" times: there's "column_used" scores
                {
                    fout.setf(ios::fixed); //set precision
                    fout.setf(ios::showpoint);
                    fout.precision(1);
                    fout << setw(7) << grade[i][l];
                }
                counter++;
            }
            else
                fout << setw(14) << averageStudent[i];
        }
        fout << endl;
    }
    fout << printDashedLine << endl;
}

/*** SCALING FOR GRAPH ***/
void scaling_graph_WIDTH(int columnOrRow, double& width_averageTestOrStudent)
{
    const double MULTIPLIER(1.5);
    width_averageTestOrStudent = INNER_WIDTH/(MULTIPLIER*columnOrRow);
}

void scaling_graph_HEIGHT(double averageTestOrStudent[MAX_COLUMN], double averageTestOrStudent_Scaled_Bottom[MAX_COLUMN], double averageTestOrStudent_Scaled_Top[MAX_COLUMN], int columnOrRow)
{
    const int BORDER(50);
    double biggestNumber(0);
    
    biggestNumber = averageTestOrStudent[0]; //assign biggest number to be the first element in the array
    
    for (int i=0; i<columnOrRow; i++) //find out what is the biggest number in the array to be the max scale height standard
        if (averageTestOrStudent[i] > biggestNumber)
            biggestNumber = averageTestOrStudent[i];
    
    for (int i=0; i<columnOrRow; i++) //fill averageTest_Scaled_Bottom arrays with averageTest arrays value
        averageTestOrStudent_Scaled_Bottom[i] = averageTestOrStudent[i];
    
    for (int i=0; i<columnOrRow; i++) //scale the graph
        averageTestOrStudent_Scaled_Bottom[i] = ((averageTestOrStudent_Scaled_Bottom[i]/biggestNumber)*INNER_HEIGHT);
    
    for (int i=0; i<columnOrRow; i++)
        averageTestOrStudent_Scaled_Top[i] = ((INNER_HEIGHT + BORDER) - averageTestOrStudent_Scaled_Bottom[i]); //bc SVG graph starts from the top, so we have to find the y-position (height) of the value
}

void x_coordinate(double x_coordinate_graph_averageTestOrStudent[MAX_COLUMN], double width_averageTestOrStudent, int columnOrRow)
{
    const int BORDER(50);
    int SECOND_BAR_X_COORDINATE(2);
    double DIFFERENCE_BETWEEN_BAR(0);
    
    x_coordinate_graph_averageTestOrStudent[0] = (width_averageTestOrStudent/SECOND_BAR_X_COORDINATE) + BORDER; //if column_used == 1
    
    for (int i=1; i<columnOrRow; i++) //for column_used > 1
    {
        x_coordinate_graph_averageTestOrStudent[i] = (width_averageTestOrStudent * (SECOND_BAR_X_COORDINATE + DIFFERENCE_BETWEEN_BAR)) + BORDER;
        DIFFERENCE_BETWEEN_BAR += 1.5; //difference between bar increases as seen from the second bar perspective
    }
}

void colorChanges_graph(double averageTestOrStudent_Scaled_Bottom[MAX_COLUMN], double RGB[][MAX_COLUMN], double sorted_averageTestOrStudent_Scaled_Bottom[MAX_COLUMN], int columnOrRow)
{
    double temp(0), red(255), green(0);
    
    for (int i=0; i<columnOrRow; i++) //fill array first
        sorted_averageTestOrStudent_Scaled_Bottom[i] = averageTestOrStudent_Scaled_Bottom[i];
    
    //SORT AVERAGE TEST ARRAY FROM HIGHEST TO LOWEST (exchange sort formula)
    for (int i=0; i<columnOrRow-1; i++)
        for (int j=(i+1); j<columnOrRow; j++)
            if (sorted_averageTestOrStudent_Scaled_Bottom[i] > sorted_averageTestOrStudent_Scaled_Bottom[j])
            {
                temp = sorted_averageTestOrStudent_Scaled_Bottom[i];
                sorted_averageTestOrStudent_Scaled_Bottom[i] = sorted_averageTestOrStudent_Scaled_Bottom[j];
                sorted_averageTestOrStudent_Scaled_Bottom[j] = temp;
            }
    
    for (int i=0; i<columnOrRow+1; i++) //decaying RGB based on rank (green for highest) (red for lowest)
        for (int j=0; j<2; j++)
        {
            if (j == 0)
            {
                RGB[i][j] = red;
                red -= (255/columnOrRow);
            }
            else if (j == 1)
            {
                RGB[i][j] = green;
                green += (255/columnOrRow);
            }
        }
}

int checkRank_graph(double current_averageTestOrStudent_Scaled_Bottom, double sorted_averageTestOrStudent_Scaled_Bottom[MAX_COLUMN], int columnOrRow)
{
    for (int i=0; i<columnOrRow; i++)
    {
        if (current_averageTestOrStudent_Scaled_Bottom == sorted_averageTestOrStudent_Scaled_Bottom[i])
            return i;
    }
    return -1;
}

void assign_RGB_value(double& red, double& green, int rank, double RGB[][MAX_COLUMN])
{
    switch (rank)
    {
        case 0:
            red = RGB[0][0];
            green = RGB[0][1];
            break;
        case 1:
            red = RGB[1][0];
            green = RGB[1][1];
            break;
        case 2:
            red = RGB[2][0];
            green = RGB[2][1];
            break;
        case 3:
            red = RGB[3][0];
            green = RGB[3][1];
            break;
        case 4:
            red = RGB[4][0];
            green = RGB[4][1];
            break;
        case 5:
            red = RGB[5][0];
            green = RGB[5][1];
            break;
        case 6:
            red = RGB[6][0];
            green = RGB[6][1];
            break;
        case 7:
            red = RGB[7][0];
            green = RGB[7][1];
            break;
        case 8:
            red = RGB[8][0];
            green = RGB[8][1];
            break;
        case 9:
            red = RGB[9][0];
            green = RGB[9][1];
            break;
        case 10:
            red = RGB[10][0];
            green = RGB[10][1];
            break;
    }
}

/*** GRAPH - AVERAGE TEST ***/
void output_graph_averageTest(ofstream& fout_graph_averageTest, double averageTest_Scaled_Bottom[MAX_COLUMN], double averageTest_Scaled_Top[MAX_COLUMN], double x_coordinate_graph_averageTest[MAX_COLUMN], double sorted_averageTest_Scaled_Bottom[MAX_COLUMN], double averageTest[MAX_COLUMN], int testNumber[MAX_COLUMN], double& width_averageTest, int column_used)
{
    int rank(0), yAxis(467), adjustment[3] = {4,2,10}; //adjustment so that the bar looks good and proportional
    double red(0), green(0), blue(0), RGB[MAX_ROW][MAX_COLUMN];
    
    fout_graph_averageTest << "<?xml version=\"1.0\" standalone=\"no\"?>"
    << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">"
    << "<svg width=\"" << OUTER_WIDTH << "\" height=\"" << OUTER_HEIGHT << "\" xmlns=\"http://www.w3.org/2000/svg\">" << endl;
    
    fout_graph_averageTest << "<text x=\"180\" y=\"38\" style=\"font-size: 15px; font-family: Verdana;\"> Average Test Grade </text>"; //OUTPUT THE TITLE
    fout_graph_averageTest << "<line x1=\"50\" y1=\"50\" x2=\"50\" y2=\"450\" style=\"stroke:black;stroke-width:2\"/>" //MAKING Y-AXIS LINE
                           << "<line x1=\"50\" y1=\"450\" x2=\"450\" y2=\"450\" style=\"stroke:black;stroke-width:2\"/>" << endl; //MAKING X-AXIS LINE
    
    colorChanges_graph(averageTest_Scaled_Bottom, RGB, sorted_averageTest_Scaled_Bottom, column_used); //fill the RGB gradient color based on rank
    
    for (int i=column_used-1; i>=0; i--) //OUTPUT THE BAR
    {
        rank = checkRank_graph(averageTest_Scaled_Bottom[i], sorted_averageTest_Scaled_Bottom, column_used); //check the rank of each bar
        assign_RGB_value(red, green, rank, RGB); //assign the RGB value for each rank
        
        fout_graph_averageTest.setf(ios::fixed); //set precision
        fout_graph_averageTest.setf(ios::showpoint);
        fout_graph_averageTest.precision(1);
    
        //OUTPUT BAR
        fout_graph_averageTest << "<rect x=\"" << x_coordinate_graph_averageTest[i] << "\" y=\"" << averageTest_Scaled_Top[i] << "\" width=\"" << width_averageTest << "\" height=\"" << averageTest_Scaled_Bottom[i] << "\" style=\"fill:rgb(" << red << "," << green << "," << blue << ");\"/>" << endl;
        //OUTPUT THE VALUE OF BAR
        fout_graph_averageTest << "<text x=\"" << x_coordinate_graph_averageTest[i] + adjustment[0] << "\" y=\"" << averageTest_Scaled_Top[i] - adjustment[1] << "\" style=\"font-size: 12px;\">" << averageTest[i] << "</text>" << endl;
        //OUTPUT THE NUMBER OF TEST BELOW BAR
        fout_graph_averageTest << "<text x=\"" << x_coordinate_graph_averageTest[i] + adjustment[2] << "\" y=\"" << yAxis << "\" style=\"font-size: 12px;\">" << testNumber[i] << "</text>" << endl;
    }
    
    fout_graph_averageTest << "</svg>" << endl; //close the .svg
    
    cout << "Check build file for output of the average of each test!" << endl;
}

/*** GRAPH - AVERAGE STUDENT ***/
void output_graph_averageStudent(ofstream& fout_graph_averageStudent, double averageStudent_Scaled_Bottom[MAX_ROW], double averageStudent_Scaled_Top[MAX_ROW], double x_coordinate_graph_averageStudent[MAX_ROW], double averageStudent[MAX_ROW], string name[MAX_ROW][MAX_COLUMN], double& width_averageStudent, int row_used)
{ 
    int rank(0), yAxis(467), adjustment[2] = {4,2}; //adjustment so that the bar looks good and proportional
    double red(0), green(0), blue(0);
    double RGB[MAX_ROW][MAX_COLUMN] = {0}, sorted_averageStudent_Scaled_Bottom[MAX_ROW] = {0};
    
    fout_graph_averageStudent << "<?xml version=\"1.0\" standalone=\"no\"?>"
    << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">"
    << "<svg width=\"" << OUTER_WIDTH << "\" height=\"" << OUTER_HEIGHT << "\" xmlns=\"http://www.w3.org/2000/svg\">" << endl;
    
    fout_graph_averageStudent << "<text x=\"180\" y=\"38\" style=\"font-size: 15px; font-family: Verdana;\"> Average Student Grade </text>"; //OUTPUT THE TITLE
    fout_graph_averageStudent << "<line x1=\"50\" y1=\"50\" x2=\"50\" y2=\"450\" style=\"stroke:black;stroke-width:2\"/>" //MAKING Y-AXIS LINE
                              << "<line x1=\"50\" y1=\"450\" x2=\"450\" y2=\"450\" style=\"stroke:black;stroke-width:2\"/>" << endl; //MAKING X-AXIS LINE
    colorChanges_graph(averageStudent_Scaled_Bottom, RGB, sorted_averageStudent_Scaled_Bottom, row_used); //fill the RGB gradient color based on rank
    
    for (int i=row_used-1; i>=0; i--)
    {
        rank = checkRank_graph(averageStudent_Scaled_Bottom[i], sorted_averageStudent_Scaled_Bottom, row_used);
        assign_RGB_value(red, green, rank, RGB);
        
        fout_graph_averageStudent.setf(ios::fixed); //set precision
        fout_graph_averageStudent.setf(ios::showpoint);
        fout_graph_averageStudent.precision(1);
        
        //OUTPUT BAR
        fout_graph_averageStudent << "<rect x=\"" << x_coordinate_graph_averageStudent[i] << "\" y=\"" << averageStudent_Scaled_Top[i] << "\" width=\"" << width_averageStudent << "\" height=\"" << averageStudent_Scaled_Bottom[i] << "\" style=\"fill:rgb(" << red << "," << green << "," << blue << ");\"/>" << endl;
        //OUTPUT THE VALUE OF BAR
        fout_graph_averageStudent << "<text x=\"" << x_coordinate_graph_averageStudent[i] + adjustment[0] << "\" y=\"" << averageStudent_Scaled_Top[i] - adjustment[1] << "\" style=\"font-size: 12px;\">" << averageStudent[i] << "</text>" << endl;
        //OUTPUT THE NUMBER OF TEST BELOW BAR
        fout_graph_averageStudent << "<text x=\"" << x_coordinate_graph_averageStudent[i] << "\" y=\"" << yAxis << "\" style=\"font-size: 10px;\">" << name[i][0] << "</text>" << endl;
    }
    
    fout_graph_averageStudent << "</svg>" << endl; //close the .svg
    
    cout << "Check build file for output of the average of each student!" << endl;
}

/*** GRAPH - SORTED AVERAGE TEST ***/
void output_graph_sortedAverageTest(ofstream& fout_graph_sortedAverageTest, double averageTest_Scaled_Bottom[MAX_COLUMN], double averageTest_Scaled_Top[MAX_COLUMN], double x_coordinate_graph_averageTest[MAX_COLUMN], double sorted_averageTest_Scaled_Bottom[MAX_COLUMN], double averageTest[MAX_COLUMN], int testNumber[MAX_COLUMN], double& width_averageTest, int column_used)
{
    int rank(0), counter(column_used-1), yAxis(467), adjustment[3] = {4,2,10}; //adjustment so that the bar looks good and proportional
    double red(0), green(0), blue(0), RGB[MAX_ROW][MAX_COLUMN];
    double sorted_averageTest_Scaled_Top[MAX_COLUMN] = {0}, sorted_averageTest[MAX_COLUMN] = {0};
    
    fout_graph_sortedAverageTest << "<?xml version=\"1.0\" standalone=\"no\"?>"
                                 << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">"
                                 << "<svg width=\"" << OUTER_WIDTH << "\" height=\"" << OUTER_HEIGHT << "\" xmlns=\"http://www.w3.org/2000/svg\">" << endl;
    
    fout_graph_sortedAverageTest << "<text x=\"180\" y=\"38\" style=\"font-size: 15px; font-family: Verdana;\"> Sorted Average Test Grade </text>"; //OUTPUT THE TITLE
    fout_graph_sortedAverageTest << "<line x1=\"50\" y1=\"50\" x2=\"50\" y2=\"450\" style=\"stroke:black;stroke-width:2\"/>" //MAKING Y-AXIS LINE
                                 << "<line x1=\"50\" y1=\"450\" x2=\"450\" y2=\"450\" style=\"stroke:black;stroke-width:2\"/>" << endl; //MAKING X-AXIS LINE
    
    colorChanges_graph(averageTest_Scaled_Bottom, RGB, sorted_averageTest_Scaled_Bottom, column_used); //fill the RGB gradient color based on rank
    sort_rank(column_used, averageTest_Scaled_Top, sorted_averageTest_Scaled_Top, averageTest, sorted_averageTest, testNumber); //sort averageTestScaledTop and averageTest
    
    for (int i=column_used-1; i>=0; i--)
    {
        rank = checkRank_graph(averageTest_Scaled_Bottom[i], sorted_averageTest_Scaled_Bottom, column_used); //check the rank
        assign_RGB_value(red, green, i, RGB); //assign the RGB value for each rank (in the parameter I put "i" instead of "rank" because we already know that the graph is sorted, and the color will just decay from green to red without needing to validate for the rank for each bar
        
        fout_graph_sortedAverageTest.setf(ios::fixed); //set precision
        fout_graph_sortedAverageTest.setf(ios::showpoint);
        fout_graph_sortedAverageTest.precision(1);
        
        //OUTPUT BAR
        fout_graph_sortedAverageTest << "<rect x=\"" << x_coordinate_graph_averageTest[i] << "\" y=\"" << sorted_averageTest_Scaled_Top[counter] << "\" width=\"" << width_averageTest << "\" height=\"" << sorted_averageTest_Scaled_Bottom[counter] << "\" style=\"fill:rgb(" << red << "," << green << "," << blue << ");\"/>" << endl;
        //OUTPUT VALUE OF BAR
        fout_graph_sortedAverageTest << "<text x=\"" << x_coordinate_graph_averageTest[i] + adjustment[0] << "\" y=\"" << sorted_averageTest_Scaled_Top[counter] - adjustment[1] << "\" style=\"font-size: 12px;\">" << sorted_averageTest[counter] << "</text>" << endl;
        //OUTPUT THE NUMBER OF TEST BELOW BAR
        fout_graph_sortedAverageTest << "<text x=\"" << x_coordinate_graph_averageTest[i] + adjustment[2] << "\" y=\"" << yAxis << "\" style=\"font-size: 12px;\">" << testNumber[counter] << "</text>" << endl; //not rank bc rank indicate gradient
        counter--;
    }
    
    fout_graph_sortedAverageTest << "</svg>" << endl; //close the .svg
    
    cout << "Check build file for output of the sorted average of each test!" << endl;
}

void sort_rank(int column_used, double averageTest_Scaled_Top[MAX_COLUMN], double sorted_averageTest_Scaled_Top[MAX_COLUMN], double averageTest[MAX_COLUMN], double sorted_averageTest[MAX_COLUMN], int testNumber[MAX_COLUMN])
{
    double temp(0);
    int temp2(0);
    
    for (int i=0; i<column_used; i++)
        sorted_averageTest_Scaled_Top[i] = averageTest_Scaled_Top[i];
    
    //SORT AVERAGE TEST SCALED TOP ARRAY FROM HIGHEST TO LOWEST (exchange sort formula)
    for (int i=0; i<column_used-1; i++)
        for (int j=(i+1); j<column_used; j++)
            if (sorted_averageTest_Scaled_Top[i] < sorted_averageTest_Scaled_Top[j])
            {
                temp = sorted_averageTest_Scaled_Top[i];
                sorted_averageTest_Scaled_Top[i] = sorted_averageTest_Scaled_Top[j];
                sorted_averageTest_Scaled_Top[j] = temp;
            }
    
    for (int i=0; i<column_used; i++)
        sorted_averageTest[i] = averageTest[i];
    
    //SORT AVERAGE TEST ARRAY FROM HIGHEST TO LOWEST (exchange sort formula)
    for (int i=0; i<column_used-1; i++)
        for (int j=(i+1); j<column_used; j++)
            if (sorted_averageTest[i] > sorted_averageTest[j])
            {
                temp = sorted_averageTest[i];
                sorted_averageTest[i] = sorted_averageTest[j];
                sorted_averageTest[j] = temp;
                
                temp2 = testNumber[i]; //sort the test number as well
                testNumber[i] = testNumber[j];
                testNumber[j] = temp2;
            }
}
