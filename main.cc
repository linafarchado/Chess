#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "board.hh"
#include "perft.hh"
#include "pieces.hh"
#include "tools.hh"

int main(int argc, char *argv[])
{
    int res = options(argc, argv);
    if (res == 1)
    {
        std::string FEN = read_file(argv[2]);
        ClassBoard *board = new ClassBoard(FEN);

        // DEBUG
        // PrettyPrinter(board->get_board());

        auto nodes = perft(*board, board->get_depth());
        std::cout << nodes << std::endl;

        // Cleanup
        delete board;
    }
    else if (res == -1)
    {
        throw std::invalid_argument("Invalid args");
    }
    // TODO: res == 2

    return 0;
}