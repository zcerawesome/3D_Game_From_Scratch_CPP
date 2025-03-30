#ifndef MATRICE_H
#define MATRICE_H
#include <vector>
#include <unordered_map>
#include <iostream>

template <typename T>
class matrice
{
private:
    int row, col;
public:
    std::unordered_map<std::string, bool> flags;
    std::vector<std::vector<T>> matrix;
    int size();
    matrice(int row, int col);
    matrice(std::vector<std::vector<T>> matrix);
    matrice(std::vector<T> matrix);
    matrice();
    void toString();
    void addRow(std::vector<T> row);
    void removeRow();
    std::vector<T>& operator[](int index);
    std::vector<T> iloc(int index);
    matrice<T> operator-(matrice<T>& inp);
    matrice<T> operator/(T inp);
    matrice<T> operator*(T inp);
    matrice<T> operator+(matrice<T>& inp);
    void operator=(matrice<T>& inp);
    void operator=(std::vector<std::vector<T>>& inp);
    void operator=(std::vector<T>& inp);
    matrice<T> operator*(matrice<T>& inp);
};

#include "matrice.cpp"
#endif