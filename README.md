# C++ Chess Engine

A C++17 chess-engine project built around bitboards. It includes FEN parsing, move application and undo, pseudo-legal move generation, a fixed-depth alpha-beta search, a lightweight command interface and an optional SDL2 board UI.

## What is implemented

- One `uint64_t` bitboard per piece set, with aggregate occupancy boards
- FEN parsing with active colour, castling rights and en-passant target state
- Pseudo-legal move generation for all piece types
- Captures, castling, en passant and automatic queen promotion
- Fixed-depth alpha-beta minimax search
- Material evaluation with a knight piece-square table
- A subset of common UCI commands for driving the engine from standard input
- An SDL2/SDL2_image board UI for interactive testing

## Architecture

```text
Board and Move
      │
      ├── MoveGenerator
      ├── Evaluation
      └── Search (alpha-beta minimax)
               │
               ├── command interface
               └── SDL2 board UI
```

The core implementation is split across `include/` and `src/`. See [DOCUMENTATION.md](DOCUMENTATION.md) for module-level details and [DESIGN_REVIEW.md](DESIGN_REVIEW.md) for the engineering roadmap and known limitations.

## Build

The current CMake configuration targets macOS and expects `SDL2.framework` and `SDL2_image.framework` under `third_party/`. Those frameworks are not committed to the repository.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Run

### Command mode

```bash
./build/ChessEngine
```

Supported commands include `uci`, `isready`, `ucinewgame`, `position <FEN>` and `go`.

Example:

```text
uci
isready
position rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPPPP1PP/RNBQKBNR w KQkq e6
go
```

### SDL2 UI

```bash
./build/ChessEngine --gui
```

Select a source square and then a destination square to test board interaction and move generation.

## Current limitations

This is an educational engine under development, not yet a fully compliant chess engine:

- Move generation is pseudo-legal and does not filter moves that leave the king in check.
- Check, checkmate and stalemate detection are not implemented.
- The command interface implements only a UCI-like subset and does not emit standard `bestmove` output.
- Search is fixed-depth; iterative deepening, move ordering, transposition tables and time management are roadmap items.
- Evaluation currently uses material and a knight piece-square table.

The next milestone is correctness: legal move generation backed by perft tests, followed by complete UCI output and stronger search.
