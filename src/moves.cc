#include "moves.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "board.hh"
#include "pieces.hh"
#include "tools.hh"

// Constructor
ClassMoves::ClassMoves(int x, int y, std::optional<ClassPieces> &PieceToMove_,
                       std::optional<ClassPieces> Maybe_, MoveType type_)
{
    end_position.first = x;
    end_position.second = y;
    start_position = PieceToMove_->get_Position();
    type = type_;
    PieceToMove = PieceToMove_;
    Maybe = Maybe_;
    PieceUnderAttack = std::nullopt;
}

std::optional<ClassPieces> ClassMoves::get_PieceUnderAttack()
{
    return PieceUnderAttack;
}

std::pair<int, int> ClassMoves::get_end_position()
{
    return end_position;
}

std::pair<int, int> ClassMoves::get_start_position()
{
    return start_position;
}

std::optional<ClassPieces> ClassMoves::get_PieceToMove()
{
    return PieceToMove;
}

ClassMoves::MoveType ClassMoves::get_type()
{
    return type;
}

// Make a move
void ClassMoves::move(ClassBoard &board_)
{
    auto b = board_.get_board();

    PieceToMove->UpdatePosition(end_position.first, end_position.second);
    PieceUnderAttack = b[end_position.first][end_position.second];
    b[end_position.first][end_position.second].reset();
    b[end_position.first][end_position.second] = PieceToMove;
    b[start_position.first][start_position.second].reset();

    // Move rook in casle
    if (type == MoveType::casle)
    {
        int y = end_position.second == 2 ? 3 : 5;
        int old_y = end_position.second == 2 ? 0 : 7;

        Maybe->UpdatePosition(end_position.first, y);
        b[end_position.first][y] = Maybe;
        b[end_position.first][old_y].reset();
    }

    // Save en passant and castling
    en_passant = board_.get_en_passant();
    auto casles = board_.get_casle();
    casle_K = casles[0];
    casle_Q = casles[1];
    casle_k = casles[2];
    casle_q = casles[3];

    // en passant is reset to empty string
    board_.set_en_passant("-");

    // If piece to move is king, no more castle allowed
    if (PieceToMove->get_PieceType() == ClassPieces::PieceType::King)
    {
        board_.set_casle("K", false);
        board_.set_casle("Q", false);
    }

    // If Rook, set castle
    else if (PieceToMove->get_PieceType() == ClassPieces::PieceType::Rook)
    {
        if (((PieceToMove->get_Color() == ClassPieces::Color::white)
             && board_.get_casle()[0])
            && (start_position.first == 7 && start_position.second == 7))
            board_.set_casle("K", false);
        else if (((PieceToMove->get_Color() == ClassPieces::Color::white)
                  && board_.get_casle()[1])
                 && (start_position.first == 7 && start_position.second == 0))
            board_.set_casle("Q", false);
        else if (((PieceToMove->get_Color() == ClassPieces::Color::black)
                  && board_.get_casle()[2])
                 && (start_position.first == 0 && start_position.second == 7))
            board_.set_casle("K", false);
        else if (((PieceToMove->get_Color() == ClassPieces::Color::black)
                  && board_.get_casle()[3])
                 && (start_position.first == 0 && start_position.second == 0))
            board_.set_casle("K", false);
    }

    // If move is en passant, set en passant to one cell behind pawn moved.
    else if (type == MoveType::en_passant_)
    {
        int x = board_.get_player() == "w" ? end_position.first + 1
                                           : end_position.first - 1;
        int y = end_position.second;

        std::string s;
        s.push_back(('a' + y));
        s.push_back(('0' + (8 - x)));
        board_.set_en_passant(s);
    }

    else if (type == MoveType::passant)
    {
        int x = board_.get_player() == "w" ? end_position.first + 1
                                           : end_position.first - 1;
        int y = end_position.second;
        Maybe = b[x][y];
        b[x][y].reset();
    }

    board_.set_board(b);
}

// Undo a Move
void ClassMoves::UndoMove(ClassBoard &board_)
{
    auto b = board_.get_board();

    // put pieces back to their place
    PieceToMove->UpdatePosition(start_position.first, start_position.second);
    b[end_position.first][end_position.second].reset();
    b[start_position.first][start_position.second].reset();
    b[end_position.first][end_position.second] = PieceUnderAttack;

    if (type == MoveType::promotion)
    {
        b[start_position.first][start_position.second] = { ClassPieces(
            ClassPieces::PieceType::Pawn,
            std::make_pair(start_position.first, start_position.second),
            PieceToMove->get_Color()) };
    }
    else
        b[start_position.first][start_position.second] = PieceToMove;

    PieceUnderAttack.reset();

    if (type == MoveType::casle)
    {
        int y = end_position.second == 2 ? 0 : 7;
        int old_y = end_position.second == 2 ? 3 : 5;

        Maybe->UpdatePosition(end_position.first, y);
        b[end_position.first][y] = Maybe;
        b[end_position.first][old_y].reset();
    }

    // reset en passant and castling to their original value
    if (board_.get_player() == "w")
    {
        board_.set_casle("K", casle_K);
        board_.set_casle("Q", casle_Q);
    }
    else if (board_.get_player() == "b")
    {
        board_.set_casle("K", casle_k);
        board_.set_casle("Q", casle_q);
    }

    if (type == MoveType::passant)
    {
        int x = Maybe->get_Position().first;
        int y = Maybe->get_Position().second;
        b[x][y] = Maybe;
    }

    board_.set_en_passant(en_passant);

    board_.set_board(b);
}

std::vector<ClassMoves>
ClassMoves::get_PossibleMoves(ClassBoard &board_,
                              std::optional<ClassPieces> &piece)
{
    using t = ClassPieces::PieceType;
    switch (piece->get_PieceType())
    {
    case t::Bishop:
        return move_Bishop(board_, piece);
    case t::King:
        return move_King(board_, piece);
    case t::Knight:
        return move_Knight(board_, piece);
    case t::Pawn:
        return move_Pawn(board_, piece);
    case t::Queen:
        return move_Queen(board_, piece);
    case t::Rook:
        return move_Rook(board_, piece);
    case t::Empty:
        throw std::runtime_error("empty piece");
    }
    std::vector<ClassMoves> v;
    return v;
}
std::vector<ClassMoves>
ClassMoves::move_Bishop(ClassBoard &board_, std::optional<ClassPieces> &piece)
{
    auto row = board_.diag_path(piece);

    return row;
}

std::vector<ClassMoves> ClassMoves::move_Rook(ClassBoard &board_,
                                              std::optional<ClassPieces> &piece)
{
    std::vector<ClassMoves> row = board_.row_path(piece);
    std::vector<ClassMoves> col = board_.col_path(piece);

    row.insert(row.end(), col.begin(), col.end());

    return row;
}

std::vector<ClassMoves>
ClassMoves::move_Queen(ClassBoard &board_, std::optional<ClassPieces> &piece)
{
    std::vector<ClassMoves> row = board_.row_path(piece);
    std::vector<ClassMoves> col = board_.col_path(piece);
    std::vector<ClassMoves> diag = board_.diag_path(piece);
    row.insert(row.end(), col.begin(), col.end());
    row.insert(row.end(), diag.begin(), diag.end());

    return row;
}

std::vector<ClassMoves> ClassMoves::move_Pawn(ClassBoard &board_,
                                              std::optional<ClassPieces> &piece)
{
    std::vector<ClassMoves> attack = board_.Pawn_path_attack(piece, false);
    std::vector<ClassMoves> path = board_.Pawn_path(piece);
    path.insert(path.end(), attack.begin(), attack.end());

    return path;
}
std::vector<ClassMoves>
ClassMoves::Pawn_attack(ClassBoard &board_, std::optional<ClassPieces> &piece)
{
    return board_.Pawn_path_attack(piece, false);
}

std::vector<ClassMoves> ClassMoves::move_King(ClassBoard &board_,
                                              std::optional<ClassPieces> &piece)
{
    std::vector<ClassMoves> castle = board_.castle(piece);
    std::vector<ClassMoves> path = board_.square_path(piece);
    path.insert(path.end(), castle.begin(), castle.end());

    return path;
}

std::vector<ClassMoves>
ClassMoves::move_Knight(ClassBoard &board_, std::optional<ClassPieces> &piece)
{
    auto moves = board_.knight_path(piece);

    return moves;
}