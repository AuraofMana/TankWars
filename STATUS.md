# TankWars — Status

**Started:** 2026-05-30
**Last updated:** 2026-05-30

## What This Is
Modernizing a ~2010 C++/SDL Battle City clone. Rebuilding in Godot 4 to get a
level editor and scrolling for free, nailing combat feel first, then content/meta.

## Current State
Full Godot 4 core loop is working — fully winnable and losable. Grid-snapped
movement (glide-onto-lane + forward-commit, feel LOCKED), brick/steel walls with
collision, shared `tank.gd` base for player + enemy, roaming enemy AI that picks
only valid directions and reverses out of dead ends, tank-vs-tank collision, and
win/lose with lives + enemy pool + HUD + R-restart. Committed at 63692ee.
Still placeholder rects, no art.

## What's Next
Core gameplay loop design discussion (deferred to next session, do NOT implement
until aligned). Options on the table: ~2-3 lives or ~3 health with fast deaths,
multi-health enemies, health replenishment, and temporary randomized power-ups
(not permanent items). Avoid both pure one-hit-death frustration and a
Diablo-style bullet-sponge feel.

## Context / Decisions Made
- **Engine: Godot 4** — chosen over an SDL 1.2→SDL2 port because the level editor
  is the first milestone and Godot's TileMap/Camera2D give editor + scrolling for
  free. GDScript is vibe-code friendly.
- **Movement feel LOCKED** — grid-snapped lanes, glide (not pop), buffered turns,
  forward-commit. Dials: SPEED 220, SNAP_TOLERANCE 20, COMMIT_THRESHOLD 8
  (== HALF - TANK_RADIUS so a tank flush to a wall can still turn), SNAP_ASSIST 5/10.
- **Build order:** tech → combat feel → objectives/scrolling → meta. Keep it 2D.
- Long-term vision (deferred): replace "defend static base," scrolling map, multiple
  objective types, off-map meta-progression base.

Full project memory: auto-memory `MEMORY.md` + `tankwars-design.md`.
