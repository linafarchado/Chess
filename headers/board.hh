#pragma once

#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "moves.hh"
#include "pieces.hh"

class ClassMoves;

class ClassBoard
{
public:
    ClassBoard(std::string FEN);

    // Getters
    std::vector<std::vector<std::optional<ClassPieces>>> get_board();
    std::string get_en_passant();
    std::string get_player();
    int get_halfmove();
    int get_fullmove();
    int get_depth();
    void set_board(std::vector<std::vector<std::optional<ClassPieces>>> &b);
    void set_casle(std::string name, bool value);
    std::vector<bool> get_casle();
    void set_en_passant(std::string pos);

    // Get vector with informations about all the elements
    // in the row position of the piece
    std::vector<ClassMoves> row_path(std::optional<ClassPieces> &piece);

    // Get vector with informations about all the elements
    // in the col position of the piece
    std::vector<ClassMoves> col_path(std::optional<ClassPieces> &piece);

    // Get vector with informations about all the elements
    // in the diagonals starting from the position of the piece
    std::vector<ClassMoves> diag_path(std::optional<ClassPieces> &piece);

    // Get vector with informations about all the elements
    // Would be helpfull for king
    std::vector<ClassMoves> square_path(std::optional<ClassPieces> &piece);

    // Get vector with informations about all the elements
    // in specific positions
    std::vector<ClassMoves> knight_path(std::optional<ClassPieces> &piece);

    void Promotion(int new_x, int new_y, int old_x, int old_y,
                   std::vector<ClassMoves> &infos,
                   std::optional<ClassPieces> &piece);

    // Pawn_path_attack
    std::vector<ClassMoves> Pawn_path_attack(std::optional<ClassPieces> &piece, bool casle);

    // Move Pawn
    std::vector<ClassMoves> Pawn_path(std::optional<ClassPieces> &piece);

    // Check if castle is possible
    bool NoPossibleCasle(std::vector<int> &pos);

    // Castle
    std::vector<ClassMoves> castle(std::optional<ClassPieces> &piece);

    // Move piece
    std::optional<ClassPieces> MovePiece(std::optional<ClassPieces> &piece,
                                         int x, int y);

    void changeSide();

    // Fill Board from the FEN
    void fill_board(std::string FEN);

    // Is King in check
    bool IsInCheck(bool casle);

    // to sum up code
    int to_sum_up(std::optional<ClassPieces> &piece,
                  std::vector<ClassMoves> &infos, int i, int j,
                  ClassPieces::Color player);

private:
    std::vector<std::vector<std::optional<ClassPieces>>> board;
    bool casle_K;
    bool casle_Q;
    bool casle_q;
    bool casle_k;
    std::string en_passant;
    std::string player;
    int halfmove;
    int fullmove;
    int depth = 0;
};