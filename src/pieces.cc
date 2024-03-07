#include "pieces.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

ClassPieces::ClassPieces(ClassPieces::PieceType piece_type_,
                         std::pair<int, int> position_,
                         ClassPieces::Color color_)
{
    piece_type = piece_type_;
    position = position_;
    color = color_;
}

std::pair<int, int> ClassPieces::get_Position()
{
    return position;
}

ClassPieces::Color ClassPieces::get_Color()
{
    return color;
}

ClassPieces::PieceType ClassPieces::get_PieceType()
{
    return piece_type;
}

void ClassPieces::UpdatePosition(int x, int y)
{
    position.first = x;
    position.second = y;
}