/*
 * Author: João Leite <carvalhojldc@gmail.com>
 * Copyright (c) 2017 João Leite
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <ostream>
#include <cstdlib>

template <typename Type>
class Matrix {

private:
    Type **matrix;
    int lines, columns;

private:
    bool validateDimensions(const Matrix<Type> & other) const
    {
        if( lines == other.lines && columns == other.columns )
            return true;
        return false;
    }

    bool validatePosition(const int line, const int column) const
    {
        if( matrix == nullptr || line<0 || column<0 || \
            (*this).lines <= line || (*this).columns <= column )
        return false;

        return true;
    }

    void zeroColumns(Type* pointLine, int start, int end)
    {
        for(int column=start; column < end; column++)
            pointLine[column] = 0.0;
    }

    Type* resizeColumns(int column, int newSize)
    {
        return (Type*)realloc(matrix[column], newSize*sizeof(Type));
    }

    Type** resizeLines(int newSize)
    {
        return (Type**)realloc(matrix, newSize*sizeof(Type*));
    }

public:
    Matrix(int l=0, int c=0) : lines(l), columns(c)
    {
        matrix = (Type**) malloc( lines * sizeof(Type*) );
        for(int l=0; l < lines; l++) {
            matrix[l] = (Type*) malloc( columns * sizeof(Type) );
            zeroColumns(matrix[l], 0, columns);
        }
    }

    ~Matrix()
    {
        for(int l=0; l < lines; l++)
            free(matrix[l]);
        free(matrix);

        matrix = nullptr;
    }

    class MatrixProxy {
        friend class Matrix<Type>;

        private:
            Matrix<Type> & parent;
            int line;

            MatrixProxy(Matrix<Type> & parent, int line) :
            parent(parent), line(line) {}

        public:
            Type & operator[] (const int column)
            {
                if( parent.validatePosition(line, column) )
                    return parent.matrix[line][column];
            }
    };

    inline int getLines(void) const
    {
        return this->lines;
    }

    inline int getColumns(void) const
    {
        return this->columns;
    }

    void newSize(const int newLines, const int newColumns)
    {
        if(lines != newLines && columns != newColumns)
        {
            matrix = resizeLines(newLines);

            for(int l=0; l < newLines; l++) {
                matrix[l] = resizeColumns(l, newColumns);
                if( l < lines )
                    zeroColumns(matrix[l], columns, newColumns);
                else
                    zeroColumns(matrix[l], 0, newColumns);
            }
            lines   = newLines;
            columns = newColumns;
        }
        else if(lines != newLines)
        {
            matrix = resizeLines(newLines);

            if(lines < newLines) {
                for(int l=lines; l < newLines; l++) {
                    matrix[l] = resizeColumns(l, columns);
                    zeroColumns(matrix[l], 0, newColumns);
                }
            }
            lines = newLines;
        }
        else if(columns != newColumns)
        {
            for(int l=0; l < lines; l++) {
                matrix[l] = resizeColumns(l, newColumns);
                zeroColumns(matrix[l], columns, newColumns);
            }
            columns = newColumns;
        }
    }

    Matrix<Type> eye() const
    {
        Matrix<Type> eye(lines,columns);

        if(lines != columns)
            return eye;

        for(int l=0; l < lines; l++)
            eye.matrix[l][l] = 1.0;

        return eye;
    }

    Matrix<Type> inv() const
    {

    }

    Matrix<Type> trans() const
    {
        Matrix<Type> trans(columns, lines);

        for(int l=0; l< lines; l++)
            for(int c=0; c < columns; c++)
                trans.matrix[c][l] = matrix[l][c];

        return trans;
    }

    friend std::ostream & operator<< (std::ostream & os, const Matrix<Type> & mat)
    {
        for (int l=0; l < mat.lines; l++) {
            for (int c=0; c < mat.columns; c++) {
                os << mat.matrix[l][c] << " ";
            }
            os << std::endl;
        }

        return os;
    }

    bool operator== (const Matrix<Type> & other)
    {
        if( this == &other )
            return true;
        else if( ! validateDimensions(other) )
            return false;

        for(int l=0; l < lines; l++)
            for(int c=0; c < columns; c++) {
                if( (*this).matrix[l][c] != other.matrix[l][c] )
                    return false;
            }

        return true;
    }

    bool operator!= (const Matrix<Type> & other)
    {
        return !( (*this) == other );
    }

    Matrix<Type> & operator= (const Matrix<Type> & other)
    {
        if( this == &other )
            return (*this);
        else if( ! validateDimensions(other) )
            (*this).newSize(other.lines, other.columns);

        for(int l=0; l < other.lines; l++)
            for(int c=0; c < other.columns; c++)
                (*this).matrix[l][c] = other.matrix[l][c];

        return (*this);
    }

    Matrix<Type> & operator= (const Type value)
    {
        for(int l=0; l < lines; l++)
            for(int c=0; c < columns; c++)
                matrix[l][c] = value;

        return (*this);
    }

    Matrix<Type> & operator+= (const Matrix<Type> & other)
    {
        if( ! validateDimensions(other) )
            return (*this);

        for(int l=0; l < other.lines; l++)
            for(int c=0; c < other.columns; c++)
                (*this).matrix[l][c] += other.matrix[l][c];

        return (*this);
    }

    Matrix<Type> & operator-= (const Matrix<Type> & other)
    {
        if( ! validateDimensions(other) )
            return (*this);

        for(int l=0; l < other.lines; l++)
            for(int c=0; c < other.columns; c++)
                (*this).matrix[l][c] -= other.matrix[l][c];

        return (*this);
    }

    const Matrix<Type> operator+ (const Matrix<Type> & other)
    {
        Matrix<Type> result(lines, columns);

        if( ! validateDimensions(other) )
            return result;

        result = (*this);
        result += other;

        return result;
    }

    const Matrix<Type> operator- (const Matrix<Type> & other)
    {
        Matrix<Type> result(lines, columns);

        if( ! validateDimensions(other) )
            return result;

        result = *this;
        result -= other;

        return result;
    }

    const Matrix<Type> operator* (const Matrix<Type> & other)
    {
        Matrix<Type> result(lines, other.columns);
        int temp = 0;

        if(lines != other.columns || columns != other.lines )
            return result;

        for(int l=0; l < lines; l++)
            for(int c=0; c < other.getColumns(); c++) {
                for(int x=0; x < other.getLines(); x++) {
                    temp += (*this).matrix[l][x] * other.matrix[x][c];
                }
                result.matrix[l][c] = temp;
                temp = 0;
            }

        return result;
    }

    inline MatrixProxy operator[] (const int line)
    {
        return MatrixProxy(*this, line);
    }

    inline Type & operator() (const int line, const int column)
    {
        if( validatePosition(line, column))
            return (*this).matrix[line][column];
    }
};

#endif // MATRIX_H
