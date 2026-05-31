extends "res://tank.gd"

# WASD by physical position, so it works on any keyboard layout.
const KEYS := {
	KEY_W: Vector2.UP,
	KEY_S: Vector2.DOWN,
	KEY_A: Vector2.LEFT,
	KEY_D: Vector2.RIGHT,
}

var _prev := {}
var _spawn := Vector2.ZERO

func _ready() -> void:
	team = TEAM_PLAYER
	hull_color = Color(0.45, 0.8, 0.45)
	barrel_color = Color(0.2, 0.5, 0.2)
	speed = 220.0
	fire_cooldown = 0.22
	_spawn = _cell_center(8, 9)
	position = _spawn
	add_to_group("player")
	for k in KEYS:
		_prev[k] = false

func _decide(_delta: float) -> void:
	_read_input()
	if Input.is_mouse_button_pressed(MOUSE_BUTTON_LEFT):
		fire()

func _read_input() -> void:
	# Last key pressed wins; if it's released, fall back to anything still held.
	for k in KEYS:
		var down: bool = Input.is_physical_key_pressed(k)
		if down and not _prev[k]:
			want = KEYS[k]
		_prev[k] = down
	if want != Vector2.ZERO and not _is_held(want):
		want = _any_held()

func _is_held(dir: Vector2) -> bool:
	for k in KEYS:
		if KEYS[k] == dir:
			return Input.is_physical_key_pressed(k)
	return false

func _any_held() -> Vector2:
	for k in KEYS:
		if Input.is_physical_key_pressed(k):
			return KEYS[k]
	return Vector2.ZERO

func hit() -> void:
	# No lives yet: just reset to spawn.
	position = _spawn
	facing = Vector2.DOWN
	want = Vector2.ZERO
