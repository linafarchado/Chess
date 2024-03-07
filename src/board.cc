#include "board.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "moves.hh"
#include "pieces.hh"
#include "tools.hh"

ClassBoard::ClassBoard(std::string FEN)
{
    // TODO: Parse fen_infos[0]
    // TODO: ASK: if there could be less arguments

    std::vector<std::string> fen_infos = tokenize(FEN, ' ');
    fill_board(fen_infos[0]);
    player = fen_infos[1];

    // Parse castling and update bool
    casle_K = fen_infos[2].find('K') != std::string::npos;
    casle_k = fen_infos[2].find('k') != std::string::npos;
    casle_q = fen_infos[2].find('q') != std::string::npos;
    casle_Q = fen_infos[2].find('Q') != std::string::npos;

    en_passant = fen_infos[3];
    halfmove = stoi(fen_infos[4]);
    fullmove = stoi(fen_infos[5]);
    if (fen_infos.size() > 6)
        depth = stoi(fen_infos[6]);
}
std::vector<std::vector<std::optional<ClassPieces>>> ClassBoard::get_board()
{
    return board;
}

int ClassBoard::get_depth()
{
    return depth;
}

int ClassBoard::get_halfmove()
{
    return halfmove;
}

int ClassBoard::get_fullmove()
{
    return fullmove;
}

std::string ClassBoard::get_player()
{
    return player;
}

std::string ClassBoard::get_en_passant()
{
    return en_passant;
}

void ClassBoard::set_en_passant(std::string pos)
{
    en_passant = pos;
}

std::vector<bool> ClassBoard::get_casle()
{
    return { casle_K, casle_Q, casle_k, casle_q };
}

void ClassBoard::set_casle(std::string name, bool value)
{
    if (name == "K")
    {
        if (player == "w")
            casle_K = value;
        else
            casle_k = value;
    }
    else
    {
        if (player == "w")
            casle_Q = value;
        else
            casle_q = value;
    }
}

void ClassBoard::set_board(
    std::vector<std::vector<std::optional<ClassPieces>>> &b)
{
    board = b;
}

int ClassBoard::to_sum_up(std::optional<ClassPieces> &piece,
                          std::vector<ClassMoves> &infos, int i, int j,
                          ClassPieces::Color player)
{
    if (!board[i][j])
    {
        infos.push_back(
            ClassMoves(i, j, piece, std::nullopt, ClassMoves::MoveType::basic));
        return 1;
    }
    else if (board[i][j]->get_Color() != player)
    {
        infos.push_back(
            ClassMoves(i, j, piece, std::nullopt, ClassMoves::MoveType::basic));
        return 0;
    }
    return -1;
}

// Get vector with informations about all the elements
// in the row position of the piece
std::vector<ClassMoves> ClassBoard::row_path(std::optional<ClassPieces> &piece)
{
    std::vector<ClassMoves> infos;

    // Actual informations
    int x = piece->get_Position().first;
    int y = piece->get_Position().second;
    ClassPieces::Color player = piece->get_Color();

    // Left
    for (int i = y - 1; i >= 0; i--)
    {
        if (to_sum_up(piece, infos, x, i, player) <= 0)
            break;
    }
    // Right
    for (int i = y + 1; i < 8; i++)
    {
        if (to_sum_up(piece, infos, x, i, player) <= 0)
            break;
    }
    return infos;
}
// Get vector with informations about all the elements
// in the col position of the piece
std::vector<ClassMoves> ClassBoard::col_path(std::optional<ClassPieces> &piece)
{
    std::vector<ClassMoves> infos;

    // Actual informations
    int x = piece->get_Position().first;
    int y = piece->get_Position().second;
    ClassPieces::Color player = piece->get_Color();

    // Up
    for (int i = x - 1; i >= 0; i--)
    {
        if (to_sum_up(piece, infos, i, y, player) <= 0)
            break;
    }
    // Down
    for (int i = x + 1; i < 8; i++)
    {
        if (to_sum_up(piece, infos, i, y, player) <= 0)
            break;
    }
    return infos;
}
// Get vector with informations about all the elements
// in the diagonals starting from the position of the piece
std::vector<ClassMoves> ClassBoard::diag_path(std::optional<ClassPieces> &piece)
{
    std::vector<ClassMoves> infos;

    // Actual informations
    int x = piece->get_Position().first;
    int y = piece->get_Position().second;
    ClassPieces::Color player = piece->get_Color();

    // Down right
    for (int i = x + 1, j = y + 1; i < 8 && j < 8; i++, j++)
    {
        if (to_sum_up(piece, infos, i, j, player) <= 0)
            break;
    }
    // Up left
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--)
    {
        if (to_sum_up(piece, infos, i, j, player) <= 0)
            break;
    }
    // Up right
    for (int i = x - 1, j = y + 1; i >= 0 && j < 8; i--, j++)
    {
        if (to_sum_up(piece, infos, i, j, player) <= 0)
            break;
    }
    // Down left
    for (int i = x + 1, j = y - 1; j >= 0 && i < 8; j--, i++)
    {
        if (to_sum_up(piece, infos, i, j, player) <= 0)
            break;
    }
    return infos;
}

// Get vector with informations about all the elements
// would be helpfull for king
std::vector<ClassMoves>
ClassBoard::square_path(std::optional<ClassPieces> &piece)
{
    std::vector<ClassMoves> infos;

    // Actual informations
    int x = piece->get_Position().first;
    int y = piece->get_Position().second;
    ClassPieces::Color player = piece->get_Color();

    for (int i = x - 1; i < x + 2; i++)
    {
        for (int j = y - 1; j < y + 2; j++)
        {
            if ((i != x || j != y) && (j >= 0 && j < 8) && (i >= 0 && i < 8))
            {
                if (!board[i][j])
                {
                    infos.push_back(ClassMoves(i, j, piece, std::nullopt,
                                               ClassMoves::MoveType::basic));
                }
                else if (board[i][j]->get_Color() != player)
                    infos.push_back(ClassMoves(i, j, piece, std::nullopt,
                                               ClassMoves::MoveType::basic));
            }
        }
    }
    return infos;
}

// Get vector with informations about all the elements
// in specific positions
std::vector<ClassMoves>
ClassBoard::knight_path(std::optional<ClassPieces> &piece)
{
    std::vector<ClassMoves> infos;

    // Actual informations
    int x = piece->get_Position().first;
    int y = piece->get_Position().second;
    ClassPieces::Color player = piece->get_Color();

    // Possible positions
    std::vector<int> x_pos = { x + 2, x + 2, x + 1, x + 1,
                               x - 1, x - 1, x - 2, x - 2 };
    std::vector<int> y_pos = { y - 1, y + 1, y - 2, y + 2,
                               y - 2, y + 2, y - 1, y + 1 };

    for (size_t i = 0; i < x_pos.size(); i++)
    {
        if ((y_pos[i] >= 0 && y_pos[i] < 8) && (x_pos[i] >= 0 && x_pos[i] < 8))
        {
            if (!board[x_pos[i]][y_pos[i]])
                infos.push_back(ClassMoves(x_pos[i], y_pos[i], piece,
                                           std::nullopt,
                                           ClassMoves::MoveType::basic));
            else if (board[x_pos[i]][y_pos[i]]->get_Color() != player)
                infos.push_back(ClassMoves(x_pos[i], y_pos[i], piece,
                                           std::nullopt,
                                           ClassMoves::MoveType::basic));
        }
    }
    return infos;
}

void ClassBoard::Promotion(int new_x, int new_y, int old_x, int old_y,
                           std::vector<ClassMoves> &infos,
                           std::optional<ClassPieces> &piece)
{
    ClassPieces::Color player = piece->get_Color();

    if ((new_x == 7 && player == ClassPieces::Color::black)
        || (new_x == 0 && player == ClassPieces::Color::white))
    {
        std::vector<ClassPieces::PieceType> types = {
            ClassPieces::PieceType::Queen, ClassPieces::PieceType::Bishop,
            ClassPieces::PieceType::Rook, ClassPieces::PieceType::Knight
        };

        for (auto type : types)
        {
            auto newpiece = std::optional<ClassPieces>{ ClassPieces(
                type, std::make_pair(old_x, old_y), player) };
            infos.push_back(ClassMoves(new_x, new_y, newpiece, piece,
                                       ClassMoves::MoveType::promotion));
        }
    }
    else
        infos.push_back(ClassMoves(new_x, new_y, piece, std::nullopt,
                                   ClassMoves::MoveType::basic));
}

// Pawn Move
std::vector<ClassMoves> ClassBoard::Pawn_path(std::optional<ClassPieces> &piece)
{
    int x = piece->get_Position().first;
    int y = piece->get_Position().second;
    std::vector<ClassMoves> moves;

    if (piece->get_Color() == ClassPieces::Color::black && !board[x + 1][y])
    {
        if (x == 1 && !board[x + 2][y])
            moves.push_back(ClassMoves(x + 2, y, piece, std::nullopt,
                                       ClassMoves::MoveType::en_passant_));
        if (x + 1 == 7)
        {
            Promotion(x + 1, y, x, y, moves, piece);
        }
        else
            moves.push_back(ClassMoves(x + 1, y, piece, std::nullopt,
                                       ClassMoves::MoveType::basic));
    }
    else if (piece->get_Color() == ClassPieces::Color::white
             && !board[x - 1][y])
    {
        if (x == 6 && !board[x - 2][y])
            moves.push_back(ClassMoves(x - 2, y, piece, std::nullopt,
                                       ClassMoves::MoveType::en_passant_));
        if (x - 1 == 0)
        {
            Promotion(x - 1, y, x, y, moves, piece);
        }
        else
            moves.push_back(ClassMoves(x - 1, y, piece, std::nullopt,
                                       ClassMoves::MoveType::basic));
    }
    return moves;
}

// Pawn_path_attack
std::vector<ClassMoves>
ClassBoard::Pawn_path_attack(std::optional<ClassPieces> &piece, bool casle)
{
    std::vector<ClassMoves> infos;

    // Actual informations
    int x = piece->get_Position().first;
    int y = piece->get_Position().second;
    ClassPieces::Color player = piece->get_Color();

    // Possible positions
    std::vector<int> x_pos = { x };

    if (player == ClassPieces::Color::black)
        x_pos = { x + 1 };
    else
        x_pos = { x - 1 };
    std::vector<int> y_pos = { y - 1, y + 1 };

    for (size_t i = 0; i < y_pos.size(); i++)
    {
        if ((x_pos[0] >= 0 && x_pos[0] < 8) && (y_pos[i] >= 0 && y_pos[i] < 8))
        {
            if (board[x_pos[0]][y_pos[i]]
                && board[x_pos[0]][y_pos[i]]->get_Color() != player)
            {
                if (board[x_pos[0]][y_pos[i]]->get_PieceType()
                    == ClassPieces::King)
                    infos.push_back(ClassMoves(x_pos[0], y_pos[i], piece,
                                               std::nullopt,
                                               ClassMoves::MoveType::basic));
                else
                    Promotion(x_pos[0], y_pos[i], x, y, infos, piece);
            }
            // En passant
            else if (x_pos[0] == 8 - (en_passant[1] - '0')
                     && y_pos[i] == en_passant[0] - 'a')
            {
                infos.push_back(ClassMoves(x_pos[0], y_pos[i], piece,
                                           std::nullopt,
                                           ClassMoves::MoveType::passant));
            }
            else if (casle && (x_pos[0] == 0 || x_pos[0] == 7) && y_pos[i] == 4)
                infos.push_back(ClassMoves(x_pos[0], y_pos[i], piece,
                                           std::nullopt,
                                           ClassMoves::MoveType::basic));
        }
    }
    return infos;
}

std::vector<ClassMoves> ClassBoard::castle(std::optional<ClassPieces> &piece)
{
    std::vector<ClassMoves> infos;

    // Actual informations
    ClassPieces::Color color = piece->get_Color();

    using c = ClassPieces::Color;

    auto Q = ((casle_Q) && (color == c::white));
    auto q = ((casle_q) && (color == c::black));
    auto K = ((casle_K) && (color == c::white));
    auto k = ((casle_k) && (color == c::black));

    if (Q || q)
    {
        int x = Q ? 7 : 0;

        if ((!board[x][1] && !board[x][2]) && !board[x][3])
        {
            if ((board[x][0]->get_Color() == piece->get_Color())
                && (board[x][0]->get_PieceType()
                    == ClassPieces::PieceType::Rook))
                infos.push_back(ClassMoves(x, 2, piece, board[x][0],
                                           ClassMoves::MoveType::casle));
        }
    }
    if (K || k)
    {
        int x = K ? 7 : 0;
        if (!board[x][5] && !board[x][6])
        {
            if ((board[x][7]->get_Color() == piece->get_Color())
                && (board[x][7]->get_PieceType()
                    == ClassPieces::PieceType::Rook))
                infos.push_back(ClassMoves(x, 6, piece, board[x][7],
                                           ClassMoves::MoveType::casle));
        }
    }
    return infos;
}

// Move Piece
std::optional<ClassPieces>
ClassBoard::MovePiece(std::optional<ClassPieces> &piece, int x, int y)
{
    int old_x = piece->get_Position().first;
    int old_y = piece->get_Position().second;

    board[old_x][old_y].reset();

    // Save old piece
    auto old_piece = board[x][y];

    // Update cooordinates
    piece->UpdatePosition(x, y);

    board[x][y] = piece;

    return old_piece;
}

void ClassBoard::changeSide()
{
    player = player == "b" ? "w" : "b";
}

void ClassBoard::fill_board(std::string FEN)
{
    std::vector<std::string> rows = tokenize(FEN, '/');

    for (size_t i = 0; i < 8; i++)
    {
        std::vector<std::optional<ClassPieces>> pieces;
        std::string str = rows[i];
        size_t y = 0;
        for (size_t j = 0; j < str.size(); j++)
        {
            std::tuple<ClassPieces::PieceType, ClassPieces::Color> type_color =
                get_PieceTypeFromFENChar(str[j]);
            if (std::get<0>(type_color) != ClassPieces::PieceType::Empty)
            {
                std::pair<int, int> pos = std::make_pair(i, y);
                std::optional<ClassPieces> piece = std::optional<ClassPieces>{
                    ClassPieces(std::get<0>(type_color), pos,
                                std::get<1>(type_color))
                };
                pieces.push_back(piece);

                // DEBUG
                // std::cout << *piece
                //   << "start position: " << piece->get_Position().first
                //   << ", " << piece->get_Position().second << std::endl;

                y++;
            }
            else
            {
                for (int k = 0; k < str[j] - '0'; k++)
                {
                    pieces.push_back(std::nullopt);
                    y++;
                }
            }
        }
        board.push_back(pieces);
    }
}

// Is King in check
bool ClassBoard::IsInCheck(bool casle)
{
    // Check if is in check
    std::vector<ClassMoves> moves;

    std::optional<ClassPieces> king = std::nullopt;

    for (size_t i = 0; i < board.size(); i++)
    {
        for (auto piece_ : board[i])
        {
            // Call getPossible in all pieces
            if (piece_)
            {
                std::string color =
                    piece_->get_Color() == ClassPieces::Color::white ? "w"
                                                                     : "b";

                // After we have changed side
                if (player == color)
                {
                    if (piece_->get_PieceType() == ClassPieces::PieceType::Pawn)
                    {
                        auto move = Pawn_path_attack(piece_, true);
                        moves.insert(moves.end(), move.begin(), move.end());
                    }
                    else
                    {
                        auto move =
                            ClassMoves::get_PossibleMoves(*this, piece_);
                        moves.insert(moves.end(), move.begin(), move.end());
                    }
                }
                else
                {
                    if (piece_->get_PieceType() == ClassPieces::PieceType::King)
                        king = piece_;
                }
            }
        }
    }

    // Actual informations
    std::pair<int, int> position = king->get_Position();

    for (ClassMoves move : moves)
    {
        auto check_pos = std::make_pair(position.first, 4);
        auto check_pos1 = std::make_pair(position.first, 5);
        auto check_pos2 = std::make_pair(position.first, 6);
        auto check_pos1n = std::make_pair(position.first, 3);
        auto check_pos2n = std::make_pair(position.first, 2);

        auto pos = move.get_end_position();

        if (pos == position)
        {
            return true;
        }

        // check if king is in check cannot do casle
        else if (casle
                 && ((pos == check_pos1n && (position == check_pos2n))
                     || (pos == check_pos1 && (position == check_pos2))
                     || (pos == check_pos)))
        {
            // DEBUG
            // PrettyPrinter(board);
            return true;
        }
    }
    return false;
}