extends Node2D

const TILE := 64
const HALF := TILE / 2.0
const COLS := 24
const ROWS := 18

const EMPTY := 0
const BRICK := 1   # Easy wall: one hit and it's gone (full passage).
const STEEL := 2   # Gated wall: blocks shots, takes no damage until a special weapon exists.
const HARD := 3    # Hard wall: chip it down over several hits, then it becomes RUBBLE.
const RUBBLE := 4  # What a HARD wall leaves: blocks tanks, but bullets fly over it.
const PIT := 5     # Terrain: blocks tanks, bullets cross. Permanent (can't be cleared).
const BUSH := 6    # Terrain: passable by tanks and bullets. Concealment is inert until enemy vision exists.

const HARD_HP := 3

# Map characters -> tile types.
const CHAR_TILE := {"B": BRICK, "S": STEEL, "H": HARD, "P": PIT, "T": BUSH}

const TEAM_PLAYER := 0
const HIT_RADIUS := 22.0

const START_LIVES := 3   # Lose all health -> lose a life and respawn. Out of lives = defeat.
const MAX_HEALTH := 3
const HEALTH_DROP_CHANCE := 0.25  # Chance a killed enemy drops a health pickup.
const TOTAL_ENEMIES := 16  # Whole pool to clear for a win.
const MAX_ALIVE := 8  # How many can be on the field at once.

# Enemy vision (fog of war): an enemy only knows where you are if it can SEE you.
# Tuned in TILES (map-relative) so these stay sensible as the map size changes;
# converted to px below since the sightline math works in world units.
const DETECT_TILES := 5.5          # Sight range along a clear line.
const FOLIAGE_PENALTY_TILES := 2.0 # Each foliage tile on the sightline cuts the range by this.
const FOLIAGE_MIN_TILES := 1.0     # Closest they can still spot you through heavy foliage.
const ALERT_SHARE_TILES := 4.0     # Spotting you also wakes enemies within this radius.
const DETECT_RADIUS := DETECT_TILES * TILE
const FOLIAGE_PENALTY := FOLIAGE_PENALTY_TILES * TILE
const FOLIAGE_MIN_RANGE := FOLIAGE_MIN_TILES * TILE
const ALERT_SHARE_RADIUS := ALERT_SHARE_TILES * TILE
const NOISE_TILES := 4.0           # A gunshot is heard this far — a "sound" sense atop sight.
const NOISE_RADIUS := NOISE_TILES * TILE

enum State { PLAYING, WON, LOST }

# Nostalgic sound cues lifted from the original 2010 game.
const SFX_LEVEL_START := "res://sound/LevelStart.wav"
const SFX_VICTORY := "res://sound/Victory.wav"
const SFX_DEFEAT := "res://sound/Defeat.wav"
const SFX_FIRE := "res://sound/TankFire1.wav"
const SFX_TANK_DEATH := "res://sound/TankDeath1.wav"
const SFX_PLAYER_DEATH := "res://sound/PTDeath.wav"
const SFX_WALL := "res://sound/WallExplosion.wav"
const SFX_ARTILLERY := "res://sound/MissileExplosion.wav"
const SFX_PICKUP := "res://sound/Score.wav"
const SFX_POOL_SIZE := 6  # Round-robin players so overlapping shots/deaths don't cut each other off.

# Temporary handcrafted layout. . empty, B brick (easy), H hard, S steel (gated),
# P pit, T bush. Real maps + editor later.
const MAP := [
	"........................",
	"..BBB......TT......BBB..",
	"..B.B......TT......B.B..",
	"..BBB..............BBB..",
	"........HHH..HHH........",
	"....PP..H......H..PP....",
	"....PP..H.SSSS.H..PP....",
	"........H.SSSS.H........",
	"TT......H.SSSS.H......TT",
	"TT......H.SSSS.H......TT",
	"........H......H........",
	"....PP..HHH..HHH..PP....",
	"....PP............PP....",
	"........................",
	"..BBB..............BBB..",
	"..B.B......TT......B.B..",
	"..BBB......TT......BBB..",
	"........................",
]

const ENEMY_SPAWNS := [
	Vector2i(1, 0), Vector2i(6, 0), Vector2i(12, 0), Vector2i(17, 0), Vector2i(22, 0),
	Vector2i(0, 4), Vector2i(23, 4),
]

# Turrets are STATIC defenses the MAP places, not part of the random spawn pool — they guard the
# four approaches to the central fortress. (Real per-map placement lands with the level editor.)
const TURRET_SPAWNS := [Vector2i(6, 6), Vector2i(17, 6), Vector2i(6, 11), Vector2i(17, 11)]

var grid := []
var hp := []  # Parallel to grid; only HARD tiles use it.
var state := State.PLAYING
var lives := START_LIVES
var health := MAX_HEALTH
var to_spawn := 0  # Enemies still waiting in the pool.
var alive := 0  # Enemies currently on the field.
var spawn_index := 0
var turrets_alive := 0  # Static map turrets left to destroy. Counted for the win, but kept
                        # separate from the mobile pool so they don't eat the on-field budget.
var astar: AStarGrid2D  # Grid pathfinding so hunting enemies route around walls (not ping-pong).

var hud_label: Label
var center_label: Label
var sfx_pool: Array = []  # AudioStreamPlayers cycled round-robin by _play_sfx.
var sfx_next := 0

func _ready() -> void:
	_setup_camera()
	_setup_audio()
	_build_ui()
	_load_map()
	_setup_astar()
	lives = START_LIVES
	health = MAX_HEALTH
	to_spawn = TOTAL_ENEMIES
	alive = 0
	turrets_alive = 0
	state = State.PLAYING
	_spawn_turrets()
	_refill_enemies()
	_update_hud()
	_play_sfx(SFX_LEVEL_START)

func _setup_audio() -> void:
	for i in range(SFX_POOL_SIZE):
		var p := AudioStreamPlayer.new()
		add_child(p)
		sfx_pool.append(p)

func _play_sfx(path: String) -> void:
	var stream := load(path)
	if not stream:
		return
	var p: AudioStreamPlayer = sfx_pool[sfx_next]
	sfx_next = (sfx_next + 1) % sfx_pool.size()
	p.stream = stream
	p.play()

# The map is now bigger than the window, but the player must SEE EVERYTHING (the fog of war
# is the enemies' handicap, not theirs). So instead of scrolling we zoom a camera out to fit
# the whole field on screen. Auto-derived from COLS/ROWS so any future map size just works.
func _setup_camera() -> void:
	var cam := Camera2D.new()
	cam.position = Vector2(COLS * TILE / 2.0, ROWS * TILE / 2.0)
	var vp := get_viewport_rect().size
	var fit: float = min(vp.x / (COLS * TILE), vp.y / (ROWS * TILE))
	cam.zoom = Vector2(fit, fit)  # zoom < 1 = zoomed out (shows more world).
	add_child(cam)
	cam.make_current()

func is_playing() -> bool:
	return state == State.PLAYING

func _unhandled_input(event: InputEvent) -> void:
	if state != State.PLAYING and event is InputEventKey and event.pressed and not event.echo:
		if event.physical_keycode == KEY_R:
			get_tree().reload_current_scene()

# --- UI ---

func _build_ui() -> void:
	var layer := CanvasLayer.new()
	add_child(layer)
	hud_label = Label.new()
	hud_label.position = Vector2(12, 8)
	hud_label.add_theme_font_size_override("font_size", 22)
	layer.add_child(hud_label)
	center_label = Label.new()
	center_label.set_anchors_preset(Control.PRESET_FULL_RECT)
	center_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	center_label.vertical_alignment = VERTICAL_ALIGNMENT_CENTER
	center_label.add_theme_font_size_override("font_size", 52)
	center_label.visible = false
	layer.add_child(center_label)

func _update_hud() -> void:
	hud_label.text = "Lives: %d     Health: %d     Enemies left: %d" % [lives, health, alive + to_spawn + turrets_alive]

# --- Enemy pool ---

func _refill_enemies() -> void:
	while alive < MAX_ALIVE and to_spawn > 0:
		_spawn_one()
		alive += 1
		to_spawn -= 1

func _spawn_one() -> void:
	var c: Vector2i = ENEMY_SPAWNS[spawn_index % ENEMY_SPAWNS.size()]
	spawn_index += 1
	var e := Node2D.new()
	e.set_script(load("res://enemy.gd"))
	e.position = Vector2(c.x * TILE + HALF, c.y * TILE + HALF)
	var r := randf()  # Mobile kind mix: 0 PAWN, 1 SKIRMISHER, 2 BRUTE, 3 ARTILLERY (turrets are static).
	var k := 0
	if r < 0.15:
		k = 3
	elif r < 0.35:
		k = 2
	elif r < 0.6:
		k = 1
	e.set("kind", k)
	add_child(e)

func _spawn_turrets() -> void:
	for c in TURRET_SPAWNS:
		var e := Node2D.new()
		e.set_script(load("res://enemy.gd"))
		e.position = Vector2(c.x * TILE + HALF, c.y * TILE + HALF)
		e.set("kind", 4)  # Kind.TURRET
		add_child(e)
		turrets_alive += 1

func _on_enemy_killed(at: Vector2) -> void:
	alive -= 1
	_play_sfx(SFX_TANK_DEATH)
	if randf() < HEALTH_DROP_CHANCE:
		_spawn_health(at)
	_refill_enemies()
	_update_hud()
	_check_win()

func _on_turret_killed(at: Vector2) -> void:
	turrets_alive -= 1
	_play_sfx(SFX_TANK_DEATH)
	if randf() < HEALTH_DROP_CHANCE:
		_spawn_health(at)
	_update_hud()
	_check_win()

func _check_win() -> void:
	if state == State.PLAYING and alive <= 0 and to_spawn <= 0 and turrets_alive <= 0:
		_win()

func _on_player_hit() -> void:
	var p := get_tree().get_first_node_in_group("player")
	if p and p.is_invulnerable():
		return  # Still in i-frames from the last hit.
	health -= 1
	if health <= 0:
		lives -= 1
		_play_sfx(SFX_PLAYER_DEATH)  # Lost a life (the tank blew up).
		if lives <= 0:
			_update_hud()
			_lose()
			return
		health = MAX_HEALTH  # New life, full health.
		if p:
			p.respawn()
	elif p:
		p.hit()
	_update_hud()

func heal() -> void:
	health = min(health + 1, MAX_HEALTH)
	_play_sfx(SFX_PICKUP)
	_update_hud()

func _spawn_health(at: Vector2) -> void:
	var h := Node2D.new()
	h.set_script(load("res://health_pickup.gd"))
	h.position = at
	add_child(h)

# --- Artillery (indirect fire) ---

# An artillery enemy lobs a telegraphed shell at a world position. The shell node
# draws the 3x3 warning and counts down on its own.
func spawn_artillery_strike(at: Vector2) -> void:
	var s := Node2D.new()
	s.set_script(load("res://artillery_strike.gd"))
	s.set("center_cell", _cell(at))
	add_child(s)

# Called by the shell when its fuse runs out: damages walls in the 3x3 and hits
# the player if they're still standing in the box.
func artillery_detonate(center: Vector2i) -> void:
	for dy in range(-1, 2):
		for dx in range(-1, 2):
			_damage_tile(Vector2i(center.x + dx, center.y + dy))
	_play_sfx(SFX_ARTILLERY)
	var p := get_tree().get_first_node_in_group("player")
	if p:
		var pc := _cell(p.position)
		if absi(pc.x - center.x) <= 1 and absi(pc.y - center.y) <= 1:
			_on_player_hit()

func _damage_tile(c: Vector2i) -> void:
	if c.x < 0 or c.y < 0 or c.x >= COLS or c.y >= ROWS:
		return
	var t: int = grid[c.y][c.x]
	if t == BRICK:
		grid[c.y][c.x] = EMPTY
		_refresh_astar_cell(c)
		queue_redraw()
	elif t == HARD:
		hp[c.y][c.x] -= 1
		if hp[c.y][c.x] <= 0:
			grid[c.y][c.x] = RUBBLE
		_refresh_astar_cell(c)
		queue_redraw()

func _win() -> void:
	state = State.WON
	center_label.text = "VICTORY\nPress R to restart"
	center_label.visible = true
	_play_sfx(SFX_VICTORY)

func _lose() -> void:
	state = State.LOST
	center_label.text = "DEFEAT\nPress R to restart"
	center_label.visible = true
	_play_sfx(SFX_DEFEAT)

# --- Map / collision queries ---

func _load_map() -> void:
	grid.clear()
	hp.clear()
	for y in range(ROWS):
		var line: String = MAP[y] if y < MAP.size() else ""
		var row := []
		var hp_row := []
		for x in range(COLS):
			var ch := line.substr(x, 1) if x < line.length() else "."
			var t: int = CHAR_TILE.get(ch, EMPTY)
			row.append(t)
			hp_row.append(HARD_HP if t == HARD else 0)
		grid.append(row)
		hp.append(hp_row)
	queue_redraw()

func _cell(world: Vector2) -> Vector2i:
	return Vector2i(floori(world.x / TILE), floori(world.y / TILE))

# --- Pathfinding (AStarGrid2D, cardinal-only to match grid movement) ---
# A* is the standard for tile games and ships with Godot; it replaces the old greedy steering
# that ping-ponged at a wall sitting between an enemy and the player.

func _setup_astar() -> void:
	astar = AStarGrid2D.new()
	astar.region = Rect2i(0, 0, COLS, ROWS)
	astar.cell_size = Vector2(TILE, TILE)
	astar.diagonal_mode = AStarGrid2D.DIAGONAL_MODE_NEVER
	astar.update()
	for y in range(ROWS):
		for x in range(COLS):
			_refresh_astar_cell(Vector2i(x, y))

func _in_grid(c: Vector2i) -> bool:
	return c.x >= 0 and c.y >= 0 and c.x < COLS and c.y < ROWS

func _refresh_astar_cell(c: Vector2i) -> void:
	if astar == null or not _in_grid(c):
		return
	var t: int = grid[c.y][c.x]
	astar.set_point_solid(c, t != EMPTY and t != BUSH)  # Passable = empty floor + bush.

# Next cardinal step for `who` heading toward world `target`, routing around walls AND other
# tanks (so hunters don't grind into a turret or each other). The player is left passable so
# enemies still converge on you. Returns ZERO if already there / unreachable -> greedy fallback.
func path_step(who, target: Vector2) -> Vector2:
	if astar == null:
		return Vector2.ZERO
	var from := _cell(who.position)
	var to := _cell(target)
	if not _in_grid(from) or not _in_grid(to) or from == to:
		return Vector2.ZERO
	var temp := []  # Mark the OTHER tanks solid for this query, then restore.
	for o in get_tree().get_nodes_in_group("enemies"):
		if o != who:
			var c := _cell(o.position)
			if _in_grid(c) and not astar.is_point_solid(c):
				astar.set_point_solid(c, true)
				temp.append(c)
	var path: Array = [] if astar.is_point_solid(to) else astar.get_id_path(from, to)
	for c in temp:
		astar.set_point_solid(c, false)
	if path.size() < 2:
		return Vector2.ZERO
	var nxt: Vector2i = path[1]
	return Vector2(nxt.x - from.x, nxt.y - from.y)

# Ring of approach slots (in tiles) around the hunted spot: cardinals first, then diagonals.
# Hunters spread across these so they don't all pile onto the player's exact cell.
const SURROUND_OFFSETS := [
	Vector2i(0, -2), Vector2i(2, 0), Vector2i(0, 2), Vector2i(-2, 0),
	Vector2i(2, -2), Vector2i(2, 2), Vector2i(-2, 2), Vector2i(-2, -2),
]

# Goal-cell distribution: instead of every hunter pathing to the SAME spot (which makes them
# converge and clump), assign each one a distinct ring cell around `center` and let it approach
# from that side. Greedy nearest-slot (stable enemy order) so an enemy already on the east takes
# an east slot rather than crossing the field. Returns the asking enemy's assigned world target.
func hunt_target(who, center: Vector2) -> Vector2:
	var hunters := []
	for e in get_tree().get_nodes_in_group("enemies"):
		if e.is_surrounder():
			hunters.append(e)
	if hunters.size() <= 1:
		return center
	hunters.sort_custom(func(a, b): return a.get_instance_id() < b.get_instance_id())
	var taken := {}
	var result := center
	for e in hunters:
		var best := -1
		var best_d := INF
		for i in range(SURROUND_OFFSETS.size()):
			if taken.has(i):
				continue
			var off: Vector2i = SURROUND_OFFSETS[i]
			var cell := center + Vector2(off.x * TILE, off.y * TILE)
			var d: float = e.position.distance_squared_to(cell)
			if d < best_d:
				best_d = d
				best = i
		if best < 0:
			continue  # More hunters than slots: this one keeps the center.
		taken[best] = true
		if e == who:
			var o: Vector2i = SURROUND_OFFSETS[best]
			result = center + Vector2(o.x * TILE, o.y * TILE)
	return result

func _tile_at(cell: Vector2i) -> int:
	if cell.x < 0 or cell.y < 0 or cell.x >= COLS or cell.y >= ROWS:
		return STEEL  # Out of bounds behaves like an unbreakable wall.
	return grid[cell.y][cell.x]

# Blocks tank movement. Everything but empty floor and bush stops a tank.
func is_solid(world: Vector2) -> bool:
	var t := _tile_at(_cell(world))
	return t != EMPTY and t != BUSH

# Can an enemy at `from` see the player at `to`? Solid walls hard-block the line;
# foliage doesn't block but shortens the range, so they must close in to spot you.
# range_px lets a unit override its sight range (per-kind vision, e.g. a future Scout).
# Pass 0 (or omit) to use the global default.
func enemy_can_see(from: Vector2, to: Vector2, range_px := 0.0) -> bool:
	var detect: float = range_px if range_px > 0.0 else DETECT_RADIUS
	var dist := from.distance_to(to)
	if dist > detect:
		return false
	var foliage := 0
	var seen := {}
	var steps := int(dist / 16.0) + 1
	for i in range(1, steps + 1):
		var pt: Vector2 = from.lerp(to, float(i) / float(steps))
		var c := _cell(pt)
		if seen.has(c):
			continue
		seen[c] = true
		var t := _tile_at(c)
		if t == BRICK or t == STEEL or t == HARD:
			return false  # A solid wall breaks the line of sight.
		elif t == BUSH:
			foliage += 1
	var effective: float = max(FOLIAGE_MIN_RANGE, detect - foliage * FOLIAGE_PENALTY)
	return dist <= effective

# An enemy that just spotted you wakes its neighbors and hands them your position.
func alert_near(at: Vector2, seen: Vector2) -> void:
	for e in get_tree().get_nodes_in_group("enemies"):
		if e.position.distance_to(at) <= ALERT_SHARE_RADIUS:
			e.receive_alert(seen)

# A gunshot is heard, not seen: enemies within earshot wake and turn toward where it
# came from even with no line of sight (Commandos-style). Sound sense atop the fog of war.
func on_player_fired(at: Vector2) -> void:
	for e in get_tree().get_nodes_in_group("enemies"):
		if e.position.distance_to(at) <= NOISE_RADIUS:
			e.receive_alert(at)

# A player bullet impact (wall or tank) is loud: nearby enemies investigate the FIRING LINE
# it came down — a point back toward the shooter, not the impact itself. So plinking a wall
# from "safety" still gives away your direction and pulls them toward you.
func _impact_noise(b) -> void:
	if b.team != TEAM_PLAYER:
		return
	var origin: Vector2 = b.position - b.dir * (TILE * 2.0)
	for e in get_tree().get_nodes_in_group("enemies"):
		if e.position.distance_to(b.position) <= NOISE_RADIUS:
			e.receive_alert(origin)

func bullet_hits(b) -> bool:
	var cell := _cell(b.position)
	var t := _tile_at(cell)
	if t == BRICK:
		grid[cell.y][cell.x] = EMPTY
		_refresh_astar_cell(cell)  # Now passable: open it up for pathing.
		queue_redraw()
		_play_sfx(SFX_WALL)
		_impact_noise(b)
		return true
	if t == HARD:
		hp[cell.y][cell.x] -= 1
		if hp[cell.y][cell.x] <= 0:
			grid[cell.y][cell.x] = RUBBLE  # Chipped down: bullets now fly over, tanks still can't pass.
		_refresh_astar_cell(cell)
		queue_redraw()
		_play_sfx(SFX_WALL)
		_impact_noise(b)
		return true
	if t == STEEL:
		_impact_noise(b)
		return true  # Gated wall: stops the shot, takes no damage.
	# RUBBLE, PIT, BUSH, EMPTY: the bullet flies over. Check for tank hits.
	if b.team == TEAM_PLAYER:
		for e in get_tree().get_nodes_in_group("enemies"):
			if e.position.distance_to(b.position) < HIT_RADIUS:
				var ep: Vector2 = e.position
				if e.take_hit(b.dir):  # Armored enemies (brute) may survive a hit.
					if e.is_turret():
						_on_turret_killed(ep)
					else:
						_on_enemy_killed(ep)
				else:
					var pl := get_tree().get_first_node_in_group("player")
					if pl:
						e.receive_alert(pl.position)  # Being shot reveals you to the survivor.
				_impact_noise(b)  # The bang also alerts that enemy's neighbors.
				return true  # Bullet stops on contact either way.
	else:
		var p := get_tree().get_first_node_in_group("player")
		if p and p.position.distance_to(b.position) < HIT_RADIUS:
			_on_player_hit()
			return true
	return false

func _draw() -> void:
	var w := COLS * TILE
	var h := ROWS * TILE
	draw_rect(Rect2(0, 0, w, h), Color(0.12, 0.12, 0.14))
	for y in range(ROWS):
		for x in range(COLS):
			var t: int = grid[y][x]
			if t == EMPTY:
				continue
			var r := Rect2(x * TILE, y * TILE, TILE, TILE).grow(-2)
			match t:
				BRICK:
					draw_rect(r, Color(0.6, 0.32, 0.2))
				STEEL:
					draw_rect(r, Color(0.55, 0.55, 0.62))
				HARD:
					_draw_hard(r, hp[y][x])
				RUBBLE:
					_draw_rubble(x, y)
				PIT:
					_draw_pit(r)
				BUSH:
					draw_rect(r, Color(0.2, 0.5, 0.25, 0.55))
	var grid_col := Color(1, 1, 1, 0.06)
	for c in range(COLS + 1):
		draw_line(Vector2(c * TILE, 0), Vector2(c * TILE, h), grid_col)
	for rr in range(ROWS + 1):
		draw_line(Vector2(0, rr * TILE), Vector2(w, rr * TILE), grid_col)

func _draw_hard(r: Rect2, h: int) -> void:
	draw_rect(r, Color(0.42, 0.45, 0.55))
	# Cracks deepen as it's chipped down toward rubble.
	var dmg := HARD_HP - h
	if dmg >= 1:
		draw_line(r.position, r.end, Color(0, 0, 0, 0.35), 2.0)
	if dmg >= 2:
		draw_line(Vector2(r.end.x, r.position.y), Vector2(r.position.x, r.end.y), Color(0, 0, 0, 0.35), 2.0)

func _draw_rubble(x: int, y: int) -> void:
	# Low debris: scattered chunks so it clearly reads as "shoot over, can't drive over."
	var base := Vector2(x * TILE, y * TILE)
	var col := Color(0.4, 0.3, 0.22)
	for c in [Vector2(8, 12), Vector2(36, 8), Vector2(20, 34), Vector2(42, 40), Vector2(6, 44)]:
		draw_rect(Rect2(base + c, Vector2(15, 12)), col)

func _draw_pit(r: Rect2) -> void:
	draw_rect(r, Color(0.06, 0.06, 0.09))
	draw_rect(r.grow(-6), Color(0.02, 0.02, 0.04))
