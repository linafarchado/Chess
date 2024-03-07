#pragma once

#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "pieces.hh"

// Parse FEN to Get pieces Type
std::tuple<ClassPieces::PieceType, ClassPieces::Color>
get_PieceTypeFromFENChar(char c);

// Parse command for options
int options(int argc, char *argv[]);

// Pretty printer for board
void PrettyPrinter(
    const std::vector<std::vector<std::optional<ClassPieces>>> &board);

// Read file
std::string read_file(std::string file);

// Get substrings from FEN
// TODO: Skip spaces
std::vector<std::string> tokenize(std::string const &str, char del);