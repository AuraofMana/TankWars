extends "res://tank.gd"

# WASD by physical position, so it works on any keyboard layout.
const KEYS := {
	KEY_W: Vector2.UP,
	KEY_S: Vector2.DOWN,
	KEY_A: Vector2.LEFT,
	KEY_D: Vector2.RIGHT,
}

const PLAYER_INVULN := 0.8  # I-frames after a hit, so a crossfire can't instagib you.
const RESPAWN_INVULN := 2.0  # Longer grace on spawn/respawn to get clear of danger.

var _prev := {}
var invuln := 0.0
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
	invuln = RESPAWN_INVULN  # Spawn grace so you never appear straight into a bullet.
	for k in KEYS:
		_prev[k] = false

func _decide(delta: float) -> void:
	if invuln > 0.0:
		invuln -= delta
		modulate.a = 0.35 if int(invuln * 12.0) % 2 == 0 else 1.0
	else:
		modulate.a = 1.0
	_read_input()
	if Input.is_mouse_button_pressed(MOUSE_BUTTON_LEFT):
		fire()

func is_invulnerable() -> bool:
	return invuln > 0.0

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
	# Stay put; health is the buffer. Brief i-frames so the next bullet doesn't chain.
	invuln = PLAYER_INVULN

func respawn() -> void:
	# Lost a life: back to spawn at full health, with grace.
	position = _spawn
	facing = Vector2.DOWN
	want = Vector2.ZERO
	invuln = RESPAWN_INVULN
