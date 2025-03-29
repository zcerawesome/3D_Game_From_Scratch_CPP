#include "matrice.h"

template <typename T>
matrice<T>::matrice(std::vector<T> matrix): row(matrix.size()), col(1)
{
    this->matrix = std::vector<std::vector<T>>(row);
    for (auto& typestuff: this->matrix)
        typestuff.resize(col);
    *this = matrix;
}

template <typename T>
matrice<T>::matrice(std::vector<std::vector<T>> matrix): matrix(matrix), row(matrix.size()), col(matrix[0].size())
{}

template <typename T>
matrice<T>::matrice()
{}

template <typename T>
void matrice<T>::toString()
{
    std::string s = "";
    for(int i =0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
            std::cout << matrix[i][j] << " ";
        std::cout << std::endl;
    }
}

template <typename T>
matrice<T> matrice<T>::operator+(matrice<T>& inp)
{
    matrice<T> temp(row, col);
    if(row != inp.row || col != inp.col)
    {
        std::cerr << "ERROR DIFFERENT DIMENSTIONS FOR SUBTRACTION" << std::endl;
        return temp;
    }
    for(int i = 0; i < row; i++)
        for(int j  = 0; j < col; j++)
            temp[i][j]= matrix[i][j] + inp[i][j];
    return temp;
}

template <typename T>
matrice<T> matrice<T>::operator/(T inp)
{
    matrice<T> temp(row, col);
    temp.matrix = matrix;
    for(std::vector<T>& rows: temp.matrix)
        for(T& value: rows)
            value /= inp;
    return temp;
}

template <typename T>
matrice<T> matrice<T>::operator-(matrice<T>& inp)
{
    matrice<T> temp(row, col);
    if(row != inp.row || col != inp.col)
    {
        std::cerr << "ERROR DIFFERENT DIMENSTIONS FOR SUBTRACTION" << std::endl;
        return temp;
    }
    for(int i = 0; i < row; i++)
        for(int j  = 0; j < col; j++)
            temp[i][j]= matrix[i][j] - inp[i][j];
    return temp;
}

template <typename T>
std::vector<T>& matrice<T>::operator[](int index)
{
    return matrix[index];
}

template <typename T>
matrice<T>::matrice(int row, int col): row(row), col(col)
{
    matrix = std::vector<std::vector<T>>(row);
    for (auto& typestuff: matrix)
    {
        typestuff.resize(col);
    }
}


template <typename T>
void matrice<T>::addRow(std::vector<T> inprow)
{
    row++;
    matrix.push_back(inprow);
}

template <typename T>
std::vector<T> matrice<T>::iloc(int index)
{
    std::vector<T> temp(row);
    for(int i = 0; i < row; i++)
        temp[i] = matrix[i][index];
    return temp;
}

template <typename T>
int matrice<T>::size()
{
    return matrix.size();
}

template <typename T>
void matrice<T>::operator=(std::vector<T>& inp)
{
    for(int i = 0; i < inp.size(); i++)
        matrix[i][0] = inp[i];
}

template <typename T>
void matrice<T>::operator=(std::vector<std::vector<T>>& inp)
{
    matrix = inp;
}

template <typename T>
void matrice<T>::operator=(matrice<T>& inp)
{
    matrix = inp.matrix;
}

template <typename T>
matrice<T> matrice<T>::operator*(matrice<T>& inp)
{
    if(col != inp.row)
    {
        std::cerr << "Error invalid dimensions for matrice multiplication" << std::endl;
    }
    matrice<T> temp(row, inp.col);
    for(int i = 0; i < row; i++)
    {
        std::vector<T>& row_val = temp[i];
        for(int j = 0; j < inp[0].size(); j++)
        {
            T& value = row_val[j];
            value = 0;
            for(int k = 0; k < col; k++)
                value += matrix[i][k] * inp[k][j];
        }
    }
    return temp;
}