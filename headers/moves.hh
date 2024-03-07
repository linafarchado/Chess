#pragma once

#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "board.hh"
#include "pieces.hh"
#include "tools.hh"

class ClassBoard;

class ClassMoves
{
public:
    // Pseudo Legal Moves
    static std::vector<ClassMoves>
    get_PossibleMoves(ClassBoard &board_, std::optional<ClassPieces> &piece);

    // Move Pieces
    static std::vector<ClassMoves>
    move_Bishop(ClassBoard &board_, std::optional<ClassPieces> &piece);
    static std::vector<ClassMoves> move_Rook(ClassBoard &board_,
                                             std::optional<ClassPieces> &piece);
    static std::vector<ClassMoves>
    move_Queen(ClassBoard &board_, std::optional<ClassPieces> &piece);
    static std::vector<ClassMoves> move_Pawn(ClassBoard &board_,
                                             std::optional<ClassPieces> &piece);
    static std::vector<ClassMoves>
    Pawn_attack(ClassBoard &board_, std::optional<ClassPieces> &piece);
    static std::vector<ClassMoves> move_King(ClassBoard &board_,
                                             std::optional<ClassPieces> &piece);
    static std::vector<ClassMoves>
    move_Knight(ClassBoard &board_, std::optional<ClassPieces> &piece);

    enum MoveType
    {
        basic,
        en_passant_,
        passant,
        casle,
        promotion
    };

    // Constructor
    ClassMoves(int x, int y, std::optional<ClassPieces> &PieceToMove_,
               std::optional<ClassPieces> Maybe_, MoveType type_);

    // Make a move
    virtual void move(ClassBoard &board_);

    // Undo a Move
    virtual void UndoMove(ClassBoard &board_);

    std::optional<ClassPieces> get_PieceUnderAttack();
    std::pair<int, int> get_start_position();
    std::pair<int, int> get_end_position();
    std::optional<ClassPieces> get_PieceToMove();
    MoveType get_type();

private:
    std::pair<int, int> start_position;
    std::pair<int, int> end_position;
    std::optional<ClassPieces> PieceToMove;
    std::optional<ClassPieces> PieceUnderAttack;
    std::optional<ClassPieces> Maybe;
    MoveType type;
    std::string en_passant;
    bool casle_K;
    bool casle_Q;
    bool casle_q;
    bool casle_k;
};