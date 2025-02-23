extern "C" void _fastcall MyProc1(double* resultMatrix, double* matrixA, double* matrixB,long long rows, long long cols, long long start, long long end);

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <malloc.h>
#include "JADll_C.h"


void readDimensionsFromFile(const std::string fileName, long long& rows, long long& cols) {
    std::ifstream file(fileName);

    //std::ifstream file(R"(C:\Users\agnel\Desktop\JA projekt\JASol\JAApp\matrixA.txt)");

    if (!file.is_open()) {
        std::cerr << "Cant open the file: " << fileName << std::endl;
        rows = 0;
        cols = 0;
        return;
    }

    rows = 0;
    cols = 0;
    std::string line;

    if (std::getline(file, line)) {
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            cols++;
        }
        rows++;
    }

    while (std::getline(file, line)) {
        if (line.empty())
            break;
        rows++;
    }

    file.close();
}

void loadMatrixesFromFile(double* matrixA, double* matrixB, long long rows, long long cols, std::string fileAName, std::string fileBname)
{
    long long tempRows = rows;
    long long tempCols = cols;

    //std::ifstream fileA(R"(C:\Users\agnel\Desktop\JA projekt\JASol\JAApp\matrixA.txt)");
    std::ifstream fileA(fileAName);
    if (fileA.is_open()) {
        // Wczytanie macierzy A
        if ((cols % 4) == 0)
        {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    fileA >> matrixA[i * cols + j];
                }
            }
        }
        else if ((cols % 4) == 1)
        {
            cols += 3;
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; ++j) {
                    if (j == (cols - 1) || j == (cols - 2) || j == (cols - 3))
                    {
                        matrixA[i * cols + j] = 0.0;
                    }
                    else
                    {
                        fileA >> matrixA[i * cols + j];
                    }
                }
            }
        }
        else if ((cols % 4) == 2)
		{
			cols += 2;
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < cols; ++j) {
					if (j == (cols - 1) || j == (cols - 2))
					{
						matrixA[i * cols + j] = 0.0;
					}
					else
					{
						fileA >> matrixA[i * cols + j];
					}
				}
			}
		}
		else
        {
            cols += 1;
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; ++j) {
                    if (j == (cols -1))
					{
						matrixA[i * cols + j] = 0.0;
					}
					else
					{
						fileA >> matrixA[i * cols + j];
					}
                    
                }

            }
        }

        // Uzupełnienie zerami, jeśli wymiary nie są podzielne przez 4
        
        fileA.close();
    }
    else {
        std::cerr << "Nie można otworzyć pliku matrixA.txt" << std::endl;
        exit(EXIT_FAILURE);
    }



    //std::ifstream fileB(R"(C:\Users\agnel\Desktop\JA projekt\JASol\JAApp\matrixB.txt)");
    std::ifstream fileB(fileBname);
    if (fileB.is_open()) {


        rows = tempRows;
        cols = tempCols;


        if ((rows % 4) == 0)
        {
            double** tempMatrix = new double* [rows];
            for (int i = 0; i < rows; ++i)
            {
                tempMatrix[i] = new double[cols];
            }

            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    fileB >> tempMatrix[i][j];
                    
                }
            }

            for (int i = 0; i < cols; i++) {
                for (int j = 0; j < rows; j++) {
                    matrixB[i * rows + j] = tempMatrix[j][i];
                }
            }


            for (int i = 0; i < rows; i++) {
				delete[] tempMatrix[i];
			}
            delete[] tempMatrix;
        }

        else if ((rows % 4) == 1)
        {

            rows += 3;
            double** tempMatrix = new double* [rows];
            for (int i = 0; i < rows; ++i)
            {
                tempMatrix[i] = new double[cols];
            }

            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (i == (rows - 1) || i == (rows - 2) || i == (rows - 3))
                    {
                        tempMatrix[i][j] = 0.0;
                    }
                    else
                    {
                        fileB >> tempMatrix[i][j];
                    }

                }
            }

            for (int i = 0; i < cols; i++) {
                for (int j = 0; j < rows; j++) {
                    matrixB[i * rows + j] = tempMatrix[j][i];
                }
            }

            for (int i = 0; i < rows; i++) {
                delete[] tempMatrix[i];
            }
            delete[] tempMatrix;
		}

        else if ((rows % 4) == 2)
        {
            rows += 2;
			double** tempMatrix = new double* [rows];
			for (int i = 0; i < rows; ++i)
			{
				tempMatrix[i] = new double[cols];
			}

            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (i == (rows - 1) || i == (rows - 2))
                    {
                        tempMatrix[i][j] = 0.0;
                    }
                    else
                    {
                        fileB >> tempMatrix[i][j];
                    }
                }
            }

            for (int i = 0; i < cols; i++) {
                for (int j = 0; j < rows; j++) {
                    matrixB[i * rows + j] = tempMatrix[j][i];
                }
            }

            for (int i = 0; i < rows; i++) {
                delete[] tempMatrix[i];
            }
            delete[] tempMatrix;
        }
        

        else
        {

            rows += 1;
            double** tempMatrix = new double* [rows];
            for (int i = 0; i < rows; ++i)
            {
                tempMatrix[i] = new double[cols];
            }

            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (i == (rows - 1))
                    {
                        tempMatrix[i][j] = 0.0;
                    }
                    else
                    {
                        fileB >> tempMatrix[i][j];
                    }

                }
            }

            for (int i = 0; i < cols; i++) {
                for (int j = 0; j < rows; j++) {
                    matrixB[i * rows + j] = tempMatrix[j][i];
                }
            }

            for (int i = 0; i < rows; i++) {
                delete[] tempMatrix[i];
            }
            delete[] tempMatrix;
            
        }
        fileB.close();
    }
    else {
        std::cerr << "Nie można otworzyć pliku matrixB.txt" << std::endl;
        exit(EXIT_FAILURE);
    }
        
}


void saveMatrixToFile(const double* resultMatrix, long long rows, long long cols, long long rowsPadded) {
    // Otwarcie pliku do zapisu

    std::ofstream file(R"(C:\Users\agnel\Desktop\JA projekt\JASol\JAApp\resultMatrix.txt)");

    // Sprawdzenie, czy plik został poprawnie otwarty
    if (!file) {
        std::cerr << "Nie udało się otworzyć pliku do zapisu!" << std::endl;
        return;
    }

    // Zapisanie danych do pliku
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Zapisanie elementu i dodanie spacji po każdym elemencie
            file << resultMatrix[i * rowsPadded + j] << " ";

            // Dodanie przecinka (opcjonalnie, do CSV)
            
        }
        file << std::endl;
    }

    // Zamknięcie pliku
    file.close();
}

double* createMatrix(long long rows, long long cols) {
    if (rows <= 0 || cols <= 0) {
        std::cerr << "Rozmiar tablicy musi być większy od zera!" << std::endl;
        return nullptr;
    }
    // Tworzenie dynamicznej tablicy
    double* matrix = new double[rows * cols];
    // Inicjalizacja zerami (opcjonalnie)
    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = 0.0;
    }
    return matrix;
}


void generateMatrixes(double* matrixA, double* matrixB, long long rows, long long cols) {
	for (int i = 0; i < rows * cols; ++i) {
		matrixA[i] = static_cast<double>(rand()) / RAND_MAX;
	}

    if ((cols % 4) == 0)
    {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                matrixA[i * cols + j] = static_cast<double>(rand()) / RAND_MAX;
                matrixB[i * cols + j] = static_cast<double>(rand()) / RAND_MAX;
            }
        }
    }
    else if ((cols % 4) == 1)
    {
        cols += 3;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; ++j) {
                if (j == (cols - 1) || j == (cols - 2) || j == (cols - 3))
                {
                    matrixA[i * cols + j] = 0.0;
                    matrixB[i * cols + j] = 0.0;
                }
                else
                {
                    matrixA[i * cols + j] = static_cast<double>(rand()) / RAND_MAX;
                    matrixB[i * cols + j] = static_cast<double>(rand()) / RAND_MAX;
                }
            }
        }
    }
    else if ((cols % 4) == 2)
    {
        cols += 2;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; ++j) {
                if (j == (cols - 1) || j == (cols - 2))
                {
                    matrixA[i * cols + j] = 0.0;
                    matrixB[i * cols + j] = 0.0;
                }
                else
                {
                    matrixA[i * cols + j] = static_cast<double>(rand()) / RAND_MAX;
                    matrixB[i * cols + j] = static_cast<double>(rand()) / RAND_MAX;
                }
            }
        }
    }
    else
    {
        cols += 1;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; ++j) {
                if (j == (cols - 1))
                {
                    matrixA[i * cols + j] = 0.0;
                    matrixB[i * cols + j] = 0.0;
                }
                else
                {
                    matrixA[i * cols + j] = static_cast<double>(rand()) / RAND_MAX;
                    matrixB[i * cols + j] = static_cast<double>(rand()) / RAND_MAX;
                }

            }

        }
    }
}



int main(int argc, char* argv[])
{
    bool isFinished = false;
    bool generateMatrixesFlag = true;
    int generateMatrixesChoice;
    bool areEven = true;
    bool isAssembler = true;
    int choice;
    long long rows = 3000;
    long long cols = rows;
    long long numberOfThreads = 32;
    std::string fileA;
    std::string fileB;



    while (!isFinished) {
        
            std::cout << "Generate matrixes? [1/0]" << std::endl;
            std::cin >> generateMatrixesChoice;
            if (generateMatrixesChoice == 1) {
                generateMatrixesFlag = true;
                std::cout << "Enter number of rows and columns: ";
                std::cin >> rows;
                cols = rows;
            }
            else if (generateMatrixesChoice == 0) {
                generateMatrixesFlag = false;
                std::cout << "enter file paths\nfile with matrix A path\n";
                std::cin >> fileA;
                std::cout << "file with matrix B path\n";
                std::cin >> fileB;

            }
            else {
                std::cerr << "Wrong choice" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::cout << "Enter number of threads: ";
            std::cin >> numberOfThreads;
            std::cout << "Using assembler? [1/0]" << std::endl;
            std::cin >> choice;
            if (choice == 1) {
                isAssembler = true;
            }
            else {
                isAssembler = false;
            }

            if (!generateMatrixesFlag)
            {
                readDimensionsFromFile(fileA, rows, cols);
            }


            



        double* matrixA = (double*)_aligned_malloc(sizeof(double) * rows * cols, 32);
        double* matrixB = (double*)_aligned_malloc(sizeof(double) * rows * cols, 32);
        double* resultMatrix = (double*)_aligned_malloc(sizeof(double) * rows * cols, 32);




        if (rows == 0 || cols == 0) {
            std::cerr << "Error loading matrixes" << std::endl;
            exit(EXIT_FAILURE);
        }
        else if (rows != cols)
        {
            std::cerr << "Matrixes must have the same number of rows and columns" << std::endl;
            exit(EXIT_FAILURE);
        }
        else if (rows % 4 == 0)
        {

            matrixA = createMatrix(rows, cols);
            matrixB = createMatrix(rows, cols);
            resultMatrix = createMatrix(rows, cols);
        }
        else if (rows % 4 == 1)
        {

            matrixA = createMatrix(rows + 3, cols);
            matrixB = createMatrix(rows + 3, cols);
            resultMatrix = createMatrix(rows + 3, cols);

            for (int i = 0; i < ((rows + 3) * cols); i++)
            {
                resultMatrix[i] = 0.0;
            }
        }
        else if (rows % 4 == 2)
        {

            matrixA = createMatrix(rows + 2, cols);
            matrixB = createMatrix(rows + 2, cols);
            resultMatrix = createMatrix(rows + 2, cols);

            for (int i = 0; i < ((rows + 2) * cols); i++)
            {
                resultMatrix[i] = 0.0;
            }
        }
        else
        {

            matrixA = createMatrix(rows + 1, cols);
            matrixB = createMatrix(rows + 1, cols);
            resultMatrix = createMatrix(rows + 1, cols);

            for (int i = 0; i < ((rows + 1) * cols); i++)
            {
                resultMatrix[i] = 0.0;
            }
        }

        std::cout << "Matrixes created\n";


        if (generateMatrixesFlag)
            generateMatrixes(matrixA, matrixB, rows, cols);
        else
            loadMatrixesFromFile(matrixA, matrixB, rows, cols, fileA, fileB);

        long long rowsPadded = rows;
        long long colsPadded = cols;


        if (rows % 4 == 1)
        {
            if (rows < 50)
            {
                for (int i = 0; i < (rows * (cols + 3)); i++)
                {
                    std::cout << matrixA[i] << " ";
                }
                std::cout << std::endl;
                for (int i = 0; i < ((rows + 3) * cols); i++)
                {
                    std::cout << matrixB[i] << " ";
                }
                std::cout << std::endl;
            }


            rowsPadded += 3;
            colsPadded += 3;

        }
        else if (rows % 4 == 2)
        {
            if (rows < 50)
            {
                for (int i = 0; i < (rows * (cols + 2)); i++)
                {
                    std::cout << matrixA[i] << " ";
                }
                std::cout << std::endl;
                for (int i = 0; i < ((rows + 2) * cols); i++)
                {
                    std::cout << matrixB[i] << " ";
                }
                std::cout << std::endl;
            }


            rowsPadded += 2;
            colsPadded += 2;
        }
        else if (rows % 4 == 3)
        {
            if (rows < 50)
            {
                for (int i = 0; i < (rows * (cols + 1)); i++)
                {
                    std::cout << matrixA[i] << " ";
                }
                std::cout << std::endl;
                for (int i = 0; i < ((rows + 1) * cols); i++)
                {
                    std::cout << matrixB[i] << " ";
                }
                std::cout << std::endl;
            }


            rowsPadded += 1;
            colsPadded += 1;
        }





        std::vector<std::thread> threads;
        std::vector<std::thread> threadsForC;


        long long rowsPerThread = rows / numberOfThreads;
        long long extraRows = rows % numberOfThreads;


        if (!isAssembler)
        {
            std::cout << "creating threads\n";
            long long startRow = 0;
            auto start_time = std::chrono::high_resolution_clock::now();
            for (long long i = 0; i < numberOfThreads; ++i) {
                // Determine the end row for each thread
                long long endRow = startRow + rowsPerThread + (i < extraRows ? 1 : 0);

                // Create the thread
                threadsForC.emplace_back(matrixMultiplicationThread, resultMatrix, matrixA, matrixB, rows, cols, rowsPadded, colsPadded, startRow, endRow);
                //std::cout << "thread " << threadsForC.back().get_id() << " created\n";

                // Update startRow for the next thread
                /*
                std::cout << startRow << std::endl;
                std::cout << endRow << std::endl;*/
                startRow = endRow;

            }

            //std::cout << "threads created\n";
            //std::cout << "Joining threads\n";

            for (auto& t : threadsForC) {
                std::thread::id id = t.get_id();
                t.join();
                //std::cout << "Thread " << id << " joined" << std::endl;
            }



            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end_time - start_time;

            std::cout << "Time to compute: " << elapsed.count() << " seconds" << std::endl;

        }
        else
        {
            std::cout << "creating threads\n";
            long long startRow = 0;
            auto start_time = std::chrono::high_resolution_clock::now();
            for (long long i = 0; i < numberOfThreads; ++i) {
                // Determine the end row for each thread
                long long endRow = startRow + rowsPerThread + (i < extraRows ? 1 : 0);

                if (startRow == endRow)
                {
                    break;
                }

                // Create the thread
                threads.emplace_back(MyProc1, resultMatrix, matrixA, matrixB, rowsPadded, colsPadded, startRow, endRow);
                //std::cout << "thread " << threads.back().get_id() << " created\n";
                // Update startRow for the next thread
                /*std::cout << startRow << std::endl;
                std::cout << endRow << std::endl;*/
                startRow = endRow;

            }

            //std::cout << "threads created\n";
            //std::cout << "Joining threads\n";

            for (auto& t : threads) {

                std::thread::id id = t.get_id();
                t.join();
                //std::cout << "Thread " << id << " joined" << std::endl;
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end_time - start_time;

            std::cout << "Time to compute: " << elapsed.count() << " seconds" << std::endl;
        }


        saveMatrixToFile(resultMatrix, rows, cols, rowsPadded);



        free(matrixA);
        free(matrixB);
        free(resultMatrix);
        
        int continueChoice;
        std::cout<< "continue using the program? [1/0]" << std::endl;
        std::cin>> continueChoice;
        if (continueChoice == 0)
        {
            isFinished = true;
            std::cout << "Exiting the program" << std::endl;
            std::cout << "Goodbye!" << std::endl;
        }
        
    }



    return 0;
}






