extends Node2D

const TILE := 64
const HALF := TILE / 2.0
const COLS := 16
const ROWS := 12

const EMPTY := 0
const BRICK := 1
const STEEL := 2

const TEAM_PLAYER := 0
const HIT_RADIUS := 22.0

const START_LIVES := 3
const TOTAL_ENEMIES := 8  # Whole pool to clear for a win.
const MAX_ALIVE := 4  # How many can be on the field at once.

enum State { PLAYING, WON, LOST }

# Temporary handcrafted layout (. empty, B brick, S steel). Real maps + editor later.
const MAP := [
	"................",
	"................",
	"..BBB.....BBB...",
	"..B.B.....B.B...",
	"..BBB.....BBB...",
	".......SS.......",
	".......SS.......",
	"....BB......BB..",
	"....BB......BB..",
	"................",
	"................",
	"................",
]

const ENEMY_SPAWNS := [Vector2i(2, 0), Vector2i(8, 0), Vector2i(13, 0)]

var grid := []
var state := State.PLAYING
var lives := START_LIVES
var to_spawn := 0  # Enemies still waiting in the pool.
var alive := 0  # Enemies currently on the field.
var spawn_index := 0

var hud_label: Label
var center_label: Label

func _ready() -> void:
	_build_ui()
	_load_map()
	lives = START_LIVES
	to_spawn = TOTAL_ENEMIES
	alive = 0
	state = State.PLAYING
	_refill_enemies()
	_update_hud()

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
	hud_label.text = "Lives: %d     Enemies left: %d" % [lives, alive + to_spawn]

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
	add_child(e)

func _on_enemy_killed() -> void:
	alive -= 1
	_refill_enemies()
	_update_hud()
	if state == State.PLAYING and alive <= 0 and to_spawn <= 0:
		_win()

func _on_player_hit() -> void:
	lives -= 1
	_update_hud()
	if lives <= 0:
		_lose()
	else:
		var p := get_tree().get_first_node_in_group("player")
		if p:
			p.hit()

func _win() -> void:
	state = State.WON
	center_label.text = "VICTORY\nPress R to restart"
	center_label.visible = true

func _lose() -> void:
	state = State.LOST
	center_label.text = "DEFEAT\nPress R to restart"
	center_label.visible = true

# --- Map / collision queries ---

func _load_map() -> void:
	grid.clear()
	for y in range(ROWS):
		var line: String = MAP[y] if y < MAP.size() else ""
		var row := []
		for x in range(COLS):
			var ch := line.substr(x, 1) if x < line.length() else "."
			match ch:
				"B":
					row.append(BRICK)
				"S":
					row.append(STEEL)
				_:
					row.append(EMPTY)
		grid.append(row)
	queue_redraw()

func _cell(world: Vector2) -> Vector2i:
	return Vector2i(floori(world.x / TILE), floori(world.y / TILE))

func _tile_at(cell: Vector2i) -> int:
	if cell.x < 0 or cell.y < 0 or cell.x >= COLS or cell.y >= ROWS:
		return STEEL  # Out of bounds behaves like an unbreakable wall.
	return grid[cell.y][cell.x]

func is_solid(world: Vector2) -> bool:
	return _tile_at(_cell(world)) != EMPTY

func bullet_hits(b) -> bool:
	var cell := _cell(b.position)
	var t := _tile_at(cell)
	if t == BRICK:
		grid[cell.y][cell.x] = EMPTY
		queue_redraw()
		return true
	if t == STEEL:
		return true
	if b.team == TEAM_PLAYER:
		for e in get_tree().get_nodes_in_group("enemies"):
			if e.position.distance_to(b.position) < HIT_RADIUS:
				e.kill()
				_on_enemy_killed()
				return true
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
			if t == BRICK:
				draw_rect(r, Color(0.6, 0.32, 0.2))
			else:
				draw_rect(r, Color(0.55, 0.55, 0.62))
	var grid_col := Color(1, 1, 1, 0.06)
	for c in range(COLS + 1):
		draw_line(Vector2(c * TILE, 0), Vector2(c * TILE, h), grid_col)
	for rr in range(ROWS + 1):
		draw_line(Vector2(0, rr * TILE), Vector2(w, rr * TILE), grid_col)
