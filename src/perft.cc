#include "perft.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "moves.hh"
#include "tools.hh"

size_t perft(ClassBoard &board_, int depth)
{
    // Actual informations
    std::string player = board_.get_player();

    if (depth == 0)
        return 1;

    size_t nodes = 0;

    std::vector<ClassMoves> AllMoves;

    auto b = board_.get_board();

    for (size_t i = 0; i < b.size(); i++)
    {
        for (auto piece : b[i])
        {
            // Call getPossible in all pieces
            if (piece)
            {
                std::string color =
                    piece->get_Color() == ClassPieces::Color::white ? "w" : "b";

                if (player == color)
                {
                    auto moves = ClassMoves::get_PossibleMoves(board_, piece);
                    AllMoves.insert(AllMoves.end(), moves.begin(), moves.end());
                }
            }
        }
    }

    for (size_t i = 0; i < AllMoves.size(); i++)
    {
        // DEBUG
        // std::cout << *AllMoves[i].get_PieceToMove() << std::endl;

        // DEBUG
        // std::cout << "Before move" << std::endl;
        // PrettyPrinter(board_.get_board());

        AllMoves[i].move(board_);
        board_.changeSide();

        // DEBUG
        // std::cout << "After move" << std::endl;
        // PrettyPrinter(board_.get_board());

        if (!board_.IsInCheck(AllMoves[i].get_type()
                              == ClassMoves::MoveType::casle))
        {
            // DEBUG
            /*if (AllMoves[i].get_PieceToMove()->get_PieceType() ==
               ClassPieces::PieceType::King)
                PrettyPrinter(board_.get_board());*/
            /*if (depth == 3)
            {
                std::cout << AllMoves[i].get_PieceToMove()->get_Color() << " --
            " << *AllMoves[i].get_PieceToMove() <<
            AllMoves[i].get_start_position().first << ", " <<
            AllMoves[i].get_start_position().second << " ---> "
                << AllMoves[i].get_end_position().first << ", "<<
            AllMoves[i].get_end_position().second << std::endl;
            }*/

            nodes += perft(board_, depth - 1);
        }

        board_.changeSide();

        AllMoves[i].UndoMove(board_);

        // DEBUG
        // std::cout << "After undomove" << std::endl;
        // PrettyPrinter(board_.get_board());
    }

    // DEBUG
    // std::cout << nodes << std::endl;
    // if (depth == 2zz)
    // {
    // for (size_t i = 0; i < AllMoves.size(); i++)
    // std::cout << AllMoves[i].get_PieceToMove()->get_Color() << " -- " <<
    // *AllMoves[i].get_PieceToMove() << AllMoves[i].get_start_position().first
    // << ", " << AllMoves[i].get_start_position().second << " ---> "
    // << AllMoves[i].get_end_position().first << ", "<<
    // AllMoves[i].get_end_position().second << std::endl;
    // }

    return nodes;
}