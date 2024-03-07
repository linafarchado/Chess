#include "tools.hh"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "pieces.hh"
int options(int argc, char *argv[])
{
    // 0 : -h --help, 1: --perft, 2: file
    std::vector<bool> boolean = { false, false, false };
    if (argc > 3)
    {
        throw std::runtime_error("too many input parameters!");
        return -1;
    }
    for (auto i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (argv[i][0] == '-')
        {
            if (arg == "-h" || arg == "--help")
            {
                if (boolean[1])
                {
                    throw std::runtime_error("cannot use two options");
                    return -1;
                }
                boolean[0] = true;
            }
            else if (arg == "--perft")
            {
                if (boolean[0])
                {
                    throw std::runtime_error("cannot use two options");
                    return -1;
                }
                boolean[1] = true;
            }

            else
            {
                throw std::invalid_argument("Invalid args");
                return -1;
            }
        }

        else if (boolean[1] && !boolean[0])
        {
            if (!std::filesystem::exists(arg))
            {
                throw std::runtime_error(
                    "cannot open file or the file does not exist");
                return -1;
            }
            boolean[2] = true;
        }
    }
    if (boolean[0])
    {
        std::string str = "Allowed options:\n\
                            -h [ --help ]   show usage\n\
                            --perft arg     path to a perft file, outputs the number of\n\
                                            legal move given a position and a depth";
        std::cout << str << std::endl;
        return 0;
    }
    else if (boolean[1] && boolean[2])
        return 1;
    return 2;
}

std::string read_file(std::string file)
{
    std::ifstream myfile;
    myfile.open(file);
    std::string myline;

    if (myfile.is_open())
    {
        if (myfile)
        {
            std::getline(myfile, myline);
            return myline;
        }
    }
    else

        throw std::runtime_error("Couldn't open file\n");
    return myline;
}

// Get substrings from FEN
// TODO: Skip spaces
std::vector<std::string> tokenize(std::string const &FEN, char del)
{
    std::vector<std::string> out;

    // construct a stream from the string
    std::stringstream ss(FEN);
    std::string s;
    while (std::getline(ss, s, del))
    {
        out.push_back(s);
    }
    return out;
}

std::tuple<ClassPieces::PieceType, ClassPieces::Color>
get_PieceTypeFromFENChar(char ch)
{
    using c = ClassPieces::Color;
    using t = ClassPieces::PieceType;
    switch (ch)
    {
    case 'P':
        return std::make_tuple(t::Pawn, c::white);
    case 'N':
        return std::make_tuple(t::Knight, c::white);
    case 'B':
        return std::make_tuple(t::Bishop, c::white);
    case 'R':
        return std::make_tuple(t::Rook, c::white);
    case 'Q':
        return std::make_tuple(t::Queen, c::white);
    case 'K':
        return std::make_tuple(t::King, c::white);
    case 'p':
        return std::make_tuple(t::Pawn, c::black);
    case 'n':
        return std::make_tuple(t::Knight, c::black);
    case 'b':
        return std::make_tuple(t::Bishop, c::black);
    case 'r':
        return std::make_tuple(t::Rook, c::black);
    case 'q':
        return std::make_tuple(t::Queen, c::black);
    case 'k':
        return std::make_tuple(t::King, c::black);
    default:
        return std::make_tuple(t::Empty, c::empty);
    }
}

void PrettyPrinter(
    const std::vector<std::vector<std::optional<ClassPieces>>> &board)
{
    const std::string horizontalLine =
        "  +----+----+----+----+----+----+----+----+";
    const std::string verticalLine = " | ";

    std::cout << "     A    B    C    D    E    F    G    H   " << std::endl;
    std::cout << horizontalLine << std::endl;

    for (int row = 0; row < 8; row++)
    {
        std::cout << row + 1;

        for (int col = 0; col < 8; ++col)
        {
            std::cout << verticalLine;

            auto piece = board[row][col];

            if (piece)
            {
                ClassPieces::PieceType pieceType = piece->get_PieceType();

                using c = ClassPieces::Color;

                switch (pieceType)
                {
                case ClassPieces::Pawn:
                    if (piece->get_Color() == c::white)
                        std::cout << "P";
                    else
                        std::cout << "p";
                    // std::cout << "♟";
                    break;
                case ClassPieces::Bishop:
                    if (piece->get_Color() == c::white)
                        std::cout << "B";
                    else
                        std::cout << "b";
                    // std::cout << "♝";
                    break;
                case ClassPieces::Rook:
                    if (piece->get_Color() == c::white)
                        std::cout << "R";
                    else
                        std::cout << "r";
                    // std::cout << "♜";
                    break;
                case ClassPieces::Queen:
                    if (piece->get_Color() == c::white)
                        std::cout << "Q";
                    else
                        std::cout << "q";
                    // std::cout << "♛";
                    break;
                case ClassPieces::King:
                    if (piece->get_Color() == c::white)
                        std::cout << "K";
                    else
                        std::cout << "k";
                    // std::cout << "♚";
                    break;
                case ClassPieces::Knight:
                    if (piece->get_Color() == c::white)
                        std::cout << "N";
                    else
                        std::cout << "n";
                    // std::cout << "♞";
                    break;
                case ClassPieces::Empty:
                    std::cout << "E";
                    break;
                }
            }
            else
                std::cout << " ";
            std::cout << " ";
        }

        std::cout << verticalLine << std::endl;
        std::cout << horizontalLine << std::endl;
    }

    std::cout << std::endl;
}