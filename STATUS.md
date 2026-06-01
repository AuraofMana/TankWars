# TankWars — Status

**Started:** 2026-05-30
**Last updated:** 2026-05-31

## What This Is
Modernizing a ~2010 C++/SDL Battle City clone. Rebuilding in Godot 4 to get a
level editor and scrolling for free, nailing combat feel first, then content/meta.

## Current State
Core loop is winnable/losable and now has real depth. Three systems landed and
tested this session:

- **Wall system v2.** BRICK (1 hit → gone), HARD (chip over 3 hits → permanent
  RUBBLE that blocks tanks but lets bullets fly over), STEEL (gated, blocks shots,
  no damage yet), PIT (blocks tanks, bullets cross), BUSH (passable concealment).
  No sub-tile destruction — keeps the locked grid movement intact.
- **Two-tier health + lives.** 3 lives × 3 health. Hits cost health (brief i-frames,
  no teleport); health 0 = lose a life + respawn at full health with longer grace;
  lives 0 = defeat. No auto-regen — health only from kills (25% enemy drop).
- **Enemy fog-of-war detection spine (P0).** Player sees all; enemies have line-of-sight
  vision (solid walls block, foliage shortens range). Per-enemy state: wander → detect →
  hunt last-seen → give up. Proximity alert-sharing (not global). Pawn + Skirmisher
  (flanks for your back) built on it. All tested and loved.

Still placeholder rects, no art.

## What's Next
P1: **Artillery** — back-line indirect fire that lobs over walls. Drops a telegraphed
3×3 shadow on the player's position, detonates ~2s later for 1 dmg to player + walls in
the box. Dodge by moving out. 1 HP. Then P2 (Controller + player slow + Defender +
Turret), P3 (Brute + Support), P4 (Scout / alert propagation), P5 (Liquid tank).
Full roster + build order in auto-memory `enemy-design.md`.

## Context / Decisions Made
- **Engine: Godot 4** — chosen over an SDL 1.2→SDL2 port because the level editor
  is the first milestone and Godot's TileMap/Camera2D give editor + scrolling for free.
- **Movement feel LOCKED** — grid-snapped lanes, glide (not pop), buffered turns,
  forward-commit. No sub-tile destruction so walls never fight the grid.
- **Keystone pillar = enemy fog-of-war asymmetry.** Game identity is "outwit their
  senses," not "clear the screen." Every enemy type plugs into the detection spine.
- **HP philosophy:** almost everything is 1 HP — danger comes from AI behavior + fog,
  not bullet-sponge health. A few exceptions (Brute directional armor, Liquid tank,
  Support shields), balanced later.
- **Rubble is terminal** — resolves the "bullets fly over rubble so you can't clear it"
  paradox; chipping a HARD wall opens a firing lane but never a driving lane.
- Map-wide coordinated sweep AI is exciting but DEFERRED (don't know how to build/test yet).
- Tech debt: pull tunable constants out of code into an editable config file (later).

Full project memory: auto-memory `MEMORY.md` + `enemy-design.md` + `tankwars-design.md`.
