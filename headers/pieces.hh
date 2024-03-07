#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

class ClassPieces
{
public:
    enum PieceType
    {
        Pawn = 1,
        Bishop = 2,
        Rook = 3,
        Queen = 4,
        King = 5,
        Knight = 6,
        Empty = 7
    };

    enum direction
    {
        front,
    };

    enum Color
    {
        white,
        black,
        empty
    };

    // Constructor
    ClassPieces(PieceType piece_type_, std::pair<int, int> position_,
                Color color_);

    // Getters
    std::pair<int, int> get_Position();
    Color get_Color();
    PieceType get_PieceType();

    void UpdatePosition(int x, int y);

    // Equal operator
    bool operator==(const ClassPieces &p) const
    {
        return this == &p;
    }

    friend std::ostream &operator<<(std::ostream &os, ClassPieces piece);

private:
    std::pair<int, int> position;
    Color color;
    PieceType piece_type;
};

inline std::ostream &operator<<(std::ostream &os, ClassPieces piece)
{
    switch (piece.get_PieceType())
    {
    case 1:
        os << "Pawn ";
        break;
    case 2:
        os << "Bishop ";
        break;
    case 3:
        os << "Rook ";
        break;
    case 4:
        os << "Queen ";
        break;
    case 5:
        os << "King ";
        break;
    case 6:
        os << "Knight ";
        break;
    case 7:
        os << "Empty ";
        break;
    }

    return os;
}