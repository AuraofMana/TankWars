# TankWars — Status

**Started:** 2026-05-30
**Last updated:** 2026-06-01

## What This Is
Modernizing a ~2010 C++/SDL Battle City clone. Rebuilding in Godot 4 to get a
level editor and scrolling for free, nailing combat feel first, then content/meta.

## Current State
Core loop is winnable/losable with real depth. Earlier systems (wall system v2,
two-tier health + lives, fog-of-war detection spine with Pawn + Skirmisher) are in
and tested. This session (2026-06-01) added the enemy roster through Turret plus the
crowd-control and feel work to make a pack of them playable:

- **Enemies through P3.** P1 **Brute** (4 HP directional armor, slow 0.75s turn so
  you can flank), P2 **Artillery** (siege-tank: deploy/bombard/flee, telegraphed 3×3
  strike), P3 **Turret** (bolted-down lane guardian, static map placement, not in the
  random pool). Three-state senses: CALM / INVESTIGATING (heard a shot, moves blind,
  amber "?") / SPOTTED (line of sight, hunts + fires, red "!"). Bullet impacts and
  shots flying off the map edge "bang" and alert nearby enemies (no visible border).
- **Pathfinding + crowd control.** Enemies now use AStarGrid2D (was greedy steering
  that ping-ponged at walls). Three layered fixes so a pack doesn't pin/blob you:
  (1) collision blocks only moves that *worsen* overlap, (2) hunters stand off ~1.2
  tiles and fire instead of ramming in, (3) goal-cell distribution spreads pawns/brutes
  onto a ring around your last-seen spot, (4) boids separation actively un-stacks
  overlapping enemies. Player movement feel untouched (separation is enemy-only).
- **Bigger map + camera.** 24×18 field (bigger than the window); zoom-to-fit Camera2D
  keeps the "see everything" pillar without scrolling. Up to 8 mobile enemies at once.
- **Audio.** First nostalgic SFX from the 2010 game wired through a 6-player pool:
  level start, victory, defeat, fire, tank death, player death, wall break, artillery,
  pickup.

Still placeholder rects, no art. All 2026-06-01 work is parse-checked; awaiting playtest.

## What's Next
**Roadmap to a satisfying core loop (Ben, 2026-06-01):** enemies → tune enemies → playtest feel →
power-ups → tune power-ups → likely retune enemies to compensate → satisfying core loop.
Playtest every step.

**Concrete near-term milestone:**
1. Build enemies up through **Controller** → P1 Brute ✅, P2 Artillery ✅, P3 Turret ✅,
   **P4 Controller (AoE damage + player slow) — NEXT.**
   (Remaining roster — Defender, Support, Scout, Lieutenant, Liquid tank — deferred past reeval.)
2. Build **tiles** + **1 interesting map** (simple "kill all enemies" objective) so it feels
   amazing to play.
3. Build **some basic power-ups**.
4. **Reevaluate:** more enemies? more maps? more power-ups? different objectives?

**Immediate:** playtest the 06-01 build (turret feel, crowd-control fixes, audio). Two open
questions: turrets are currently *mandatory* kills for victory (make optional?), and enemy
fire SFX plays for every shot (may get noisy with a swarm — easy to gate to player-only).

Renames: "Leader" → **Lieutenant** (mini-boss; final bosses are custom + come with level design).
Objective types parked for the reeval: defend a base, capture bases, reach a location before EOD,
escort (Ben dislikes), kill a VIP, destroy an objective. Full roster in auto-memory `enemy-design.md`.

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
- **Crowd control is a stack of general rules, not situational patches** (2026-06-01): the
  player-pin / enemy-blob problems were multi-agent crowding, NOT pathfinding bugs (A* works).
  Fixed with collision-that-only-blocks-worsening-overlap + hunter standoff + goal-cell
  distribution + boids separation. Deliberately did NOT build reservation-table cooperative
  pathfinding (heavier, unneeded for ≤8 agents on a grid).
- **Turrets are static map placements, not autospawned** — a static defense the map authors
  place, kept out of the random enemy pool (per-map placement is ultimately an editor job).
- Tech debt: pull tunable constants out of code into an editable config file (later).
- GDScript gotcha (recurring): `:=` can't infer type off an untyped Variant (e.g. `get_parent()`
  method returns, or `o.position` from an untyped Array) — always annotate explicitly. The
  headless parse-check misses these; the editor's full type pass catches them.

Full project memory: auto-memory `MEMORY.md` + `enemy-design.md` + `tankwars-design.md`.
