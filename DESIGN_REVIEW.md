## ChessEngine Design Review, Issues, and Roadmap

### Executive Summary

Current engine demonstrates a working bitboard model, a basic evaluator, a fixed-depth alpha-beta search, and two front-ends (UCI loop and SDL GUI). To reach a solid, “production-grade hobby engine” standard, we need to: fix correctness (full legal move gen + perft parity), modernize the architecture (core vs front-ends), strengthen search (TT, ordering, quiescence, ID), expand evaluation, and complete UCI compliance with time management.

## Key Design/Code Issues

- Correctness gaps in move legality

  - Generator produces pseudo-legal moves only; no self-check filtering.
  - Castling checks only for empty path and rights; does not verify check on path/king.
  - En passant relies on string target, not validated against check conditions.
  - Promotions auto-queen; underpromotion unsupported.

- UCI compliance not complete

  - Does not output `id name`/`id author`, `uciok` only, and no `bestmove ...` line.
  - Does not handle `position startpos moves ...` or full UCI position/moves sequences.
  - No `go wtime btime winc binc movestogo` time controls; search depth is fixed.

- Architecture and coupling

  - Core and GUI code intermingled in `main.cpp` (includes GUI headers in non-GUI path).
  - `Board` is very stateful and handles both representation, rules, and IO (debug printing inside model).
  - `Move` is a flexible struct with raw ints; no compact encoding or helpers for flags.
  - Opening module is a standalone prototype (reads `twic1560.pgn` from cwd) and not integrated.

- Performance/search limitations

  - No transposition table (TT) or Zobrist hashing.
  - No iterative deepening (ID), aspiration windows, or time management.
  - No quiescence search; prone to horizon effects.
  - No move ordering (hash move, MVV-LVA, killers, history).

- Evaluation is minimal

  - Material + knight PST only.
  - No PST for other pieces; no tapered eval (midgame/endgame), mobility, king safety, pawn structure, passed pawns, threats, space, etc.

- Game termination and check detection missing

  - `MoveGenerator::isKingInCheck/Checkmate/Stalemate` return `false`; there is no detection of terminal positions.
  - Moves are not filtered for self-check, so checkmate cannot be determined from current move lists.
  - GUI does not implement game-over logic; termination must be detected in the engine and surfaced to front-ends.

- Code quality issues
  - Console output in core logic (`Board::setBitBoard`, `makeMove`, `printBitBoards`, helper `getPieceletter` in `main.cpp`).
  - Hard-coded bit masks for castling and squares; better centralized constants/masks needed.
  - Castling-right updates via XOR may not correctly “clear” rights if repeated; use clear-mask instead.
  - Manual memory in GUI (`TextureManager` destructor called explicitly, then `free`) — risk of double free; prefer RAII/smart pointers.
  - Inconsistent naming (camelCase vs snake_case) and limited comments where rationale is non-obvious.
  - `UCIInterface::printMove` formats moves in a custom way and prints human text lines after — not UCI.
  - `Move.cpp` is empty; either remove or use for helper functions.
  - `ChessUtils` mapping of row↔rank is correct but easy to misuse; consider a typed `Square` abstraction.
  - Resource paths in GUI are relative; fragile if the working directory changes.

## Proposed Architecture Improvements

- Clear layering

  - `engine/` core library: `Board`, `Move`, `MoveGenerator`, `Evaluation`, `Search`, `TT`, `Zobrist`, `Perft`, `Attacks`.
  - `frontends/uci/` binary with `UCIInterface` using the engine lib.
  - `frontends/gui/` binary with `View/TextureManager/RenderData` using the engine lib.

- Stronger types and helpers

  - Introduce enums/types: `Color`, `PieceType`, `Piece`, `Square` (0..63), `CastlingRights` bitmask.
  - Pack `Move` into 32-bit bitfield (from, to, piece, captured, promo, flags: en-passant, castle, etc.) with helper accessors.

- Board and attacks

  - Add precomputed attack tables or magic bitboards for sliders (bishop/rook/queen) for speed and clarity.
  - Add pin and check detection; legal move generation should filter out moves that leave king in check.
  - Maintain incremental `AllPieces`, `WhitePieces`, `BlackPieces` and Zobrist key on make/undo.

- Search enhancements

  - Iterative deepening over `maxDepth` with principal variation (PV) tracking.
  - Transposition table (Zobrist hash, store bound/value/depth, move).
  - Move ordering: hash move first, captures by MVV-LVA, killer moves, history heuristic.
  - Quiescence search for captures/checks to stabilize leaf evaluation.
  - Aspiration windows; null-move pruning (later, carefully), late move reductions.
  - Time management for UCI `go` inputs and soft/hard node/time limits.

- Evaluation expansion

  - PSTs for all pieces; tapered evaluation (phase interpolation).
  - Mobility, king safety (pawn shield, attacks near king), pawn structure (isolated, doubled, passed), space, bishop pair, rook on open files, piece coordination.

- UCI compliance

  - Proper `uci` handshake (`id name`, `id author`, `uciok`).
  - `isready`, `ucinewgame`, `position startpos|fen ... moves ...`.
  - `go` with clocks, depth, nodes, movetime; output `bestmove` and optionally `ponder`.
  - Info lines during search (`info depth ... nodes ... nps ... pv ...`).

- Testing and tooling

  - `perft` driver with known positions to validate legal move generation.
  - Unit tests for FEN parse, make/undo, en-passant, castling, promotions.
  - Bench command for search speed baseline; simple profiling hooks.
  - CI to build and run tests on push.

- Code quality
  - Remove debug prints from core; use a logging macro controllable via compile-time flag.
  - RAII and smart pointers in GUI; avoid manual destructor calls and `free` for `new` allocations.
  - Centralize constants (square indices, masks) and remove magic numbers.
  - Separate `DOCUMENTATION.md` (user docs) and `DESIGN_REVIEW.md` (engineering roadmap) as introduced here.
- Check/terminal-state subsystem
  - Implement `isSquareAttacked(board, square, byColor)` and `isKingInCheck(board)` utilities.
  - Filter pseudo-legal moves into legal moves (make → test check → undo).
  - Define terminal-state evaluation and reporting (checkmate vs stalemate) for search and front-ends.

## Detailed Issues and Fix Ideas by Module

- Board

  - Issue: `makeMove` performs IO and updates en-passant as a string; promotion only to queen.
  - Fix: move debug IO out; store en-passant as `Square` or `-1`; support underpromotion in `Move` encoding; clear castling rights with masks when king/rook moves or is captured.

- MoveGenerator

  - Issue: pseudo-legal only; castling path not validated for check; hard-coded masks.
  - Fix: implement legal generation: compute checkers, pinned pieces, and evasion rules; use attack tables; validate castling squares (e, f, g for K-side) are not attacked; centralize masks. Add `isSquareAttacked` as the basis for check detection and legality filtering.

- Search

  - Issue: fixed depth only; no TT, no ordering, no quiescence; recursion may be slow without ordering.
  - Fix: add ID+TT; implement PV/ordering; add quiescence for captures; support abort by time; integrate per-node stats and info lines. When no legal moves exist, return mate scores (±MATE ± ply) if in check, or 0 for stalemate.

- Evaluation

  - Issue: material + knight PST only; Black PST handling subtracts using white table indices without flipping.
  - Fix: introduce mirrored PSTs or index flip helpers; add broader features and tapered eval.

- UCIInterface

  - Issue: custom move output and verbose print; lacks full command support.
  - Fix: output `bestmove e2e4`; implement `position startpos moves ...`; add go-time parsing and info reporting. On terminal states, print a valid `bestmove` and optionally an `info` string indicating checkmate/stalemate.

- GUI

  - Issue: manual memory, destructor misuse, relative paths.
  - Fix: use `std::unique_ptr` for `TextureManager`; avoid explicit destructor calls; use resource path relative to executable directory or embed via CMake config; add simple overlay for selected squares and legal moves. Surface terminal states from engine after each move and display a game-over dialog/overlay.

- Opening
  - Issue: not integrated; file path fixed.
  - Fix: parse a lightweight book (e.g., Polyglot) and probe by hash at root; make path configurable.

## Prioritized Roadmap (Next Steps)

Phase 1 — Correctness and Foundations

1. Implement legal move generation (check, pins, evasions). Replace pseudo-legal in `generateMoves` or filter after generation.
   - Add `isSquareAttacked` and `isKingInCheck`; filter pseudo-legal moves, validate castling squares; mark en-passant legality wrt check.
   - Detect terminal states: if no legal moves → checkmate (if in check) or stalemate (if not).
2. Add `perft` with a suite of standard positions; reach parity with known results up to depth 5/6.
3. Refactor `Board` to store en-passant as square index; fix castling-right updates with clear masks; remove core debug prints.
4. Introduce `Square`, `Color`, `PieceType`, `CastlingRights`; pack `Move` with flags and helpers.

Phase 2 — UCI and Search 5. Full UCI compliance: `id`, `bestmove`, `position` (startpos/fen + moves), `go` with time controls. 6. Iterative deepening + TT (Zobrist hashing) + PV tracking. 7. Move ordering (hash move, MVV-LVA, killers, history); quiescence search for captures/checks. 8. Basic time management (allocate time per move, soft stop, hard cutoff).

Phase 3 — Evaluation and Speed 9. Add PSTs for all pieces and tapered eval; basic mobility, king safety, pawn structure. 10. Optional: null-move pruning and late move reductions; aspiration windows. 11. Profiling and micro-optimizations (attack tables/magic bitboards, cache-friendly structures).

Phase 4 — GUI and UX 12. RAII in GUI; legal move highlighting; promotion UI; load/save FEN; simple game controls. 13. Configurable resource paths and application bundle packaging on macOS.

Phase 5 — Tooling and Quality 14. Unit tests (FEN, make/undo, special moves, UCI parsing) and CI pipeline. 15. Bench command and regression harness to prevent performance regressions.

## Acceptance Criteria (Milestones)

- M1: `perft` matches reference positions up to depth 5; 100% determinism for fixed depth.
- M2: Full UCI compliance; survives simple test suites; produces `bestmove` under time controls.
- M3: >150 knps on laptop in depth-limited test; stable PV; quiescence implemented.
- M4: Evaluation adds PSTs for all pieces and basic king safety; plays reasonable chess at low depths.
- M5: Check, checkmate, and stalemate detection pass standard test positions; GUI/CLI report game over correctly.

## Stretch Goals

- Polyglot opening book probing by Zobrist key.
- Endgame tablebase probing (Syzygy) hooks.
- Pondering support in UCI; multi-PV analysis mode.
