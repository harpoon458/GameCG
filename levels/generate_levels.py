import random
import sys
from collections import deque
from pathlib import Path

COLS = 32
ROWS = 16
STEPS = ((1, 0), (-1, 0), (0, 1), (0, -1))
ALLOWED = set("#.PZSHAKDE")
MIN_SPAWN_DISTANCE = 8

PLAN = {
    2: {"rooms": 5, "pillars": 0, "loops": 0, "max_w": 8, "max_h": 5},
    3: {"rooms": 6, "pillars": 2, "loops": 0, "max_w": 8, "max_h": 5},
    4: {"rooms": 6, "pillars": 4, "loops": 0, "max_w": 8, "max_h": 5},
    5: {"rooms": 7, "pillars": 6, "loops": 1, "max_w": 7, "max_h": 5},
    6: {"rooms": 7, "pillars": 8, "loops": 1, "max_w": 7, "max_h": 5},
    7: {"rooms": 8, "pillars": 10, "loops": 1, "max_w": 7, "max_h": 5},
    8: {"rooms": 8, "pillars": 12, "loops": 2, "max_w": 6, "max_h": 4},
    9: {"rooms": 9, "pillars": 14, "loops": 2, "max_w": 6, "max_h": 4},
    10: {"rooms": 9, "pillars": 16, "loops": 2, "max_w": 6, "max_h": 4},
}
CHASERS = {2: 5, 3: 6, 4: 7, 5: 8, 6: 9, 7: 10, 8: 11, 9: 12, 10: 13}
SHOOTERS = {2: 3, 3: 4, 4: 5, 5: 6, 6: 7, 7: 8, 8: 9, 9: 10, 10: 12}
HEALTH = {2: 3, 3: 3, 4: 3, 5: 3, 6: 3, 7: 4, 8: 4, 9: 4, 10: 4}


def min_route(level):
    return 40 + 2 * level


def ammo_packs(level):
    enemies = CHASERS[level] + SHOOTERS[level]
    ratio = 2.6 - (level - 1) * (1.5 / 9)
    return max(2, round((ratio * 3 * enemies - 30) / 12))


def rooms_overlap(a, b):
    ax, ay, aw, ah = a
    bx, by, bw, bh = b
    return ax - 1 < bx + bw and bx - 1 < ax + aw and ay - 1 < by + bh and by - 1 < ay + ah


def place_rooms(rng, target, max_w, max_h):
    rooms = []
    for _ in range(3000):
        if len(rooms) == target:
            break
        w = rng.randint(4, max_w)
        h = rng.randint(3, max_h)
        x = rng.randint(1, COLS - 1 - w)
        y = rng.randint(1, ROWS - 1 - h)
        room = (x, y, w, h)
        if not any(rooms_overlap(room, other) for other in rooms):
            rooms.append(room)
    return sorted(rooms)


def carve_room(grid, room):
    rx, ry, w, h = room
    for y in range(ry, ry + h):
        for x in range(rx, rx + w):
            grid[y][x] = "."


def carve_corridor(grid, a, b, horizontal_first):
    ax, ay = a
    bx, by = b
    if horizontal_first:
        for x in range(min(ax, bx), max(ax, bx) + 1):
            grid[ay][x] = "."
        for y in range(min(ay, by), max(ay, by) + 1):
            grid[y][bx] = "."
    else:
        for y in range(min(ay, by), max(ay, by) + 1):
            grid[y][ax] = "."
        for x in range(min(ax, bx), max(ax, bx) + 1):
            grid[by][x] = "."


def bfs(grid, start, blocked=frozenset()):
    dist = {start: 0}
    queue = deque([start])
    while queue:
        x, y = queue.popleft()
        for dx, dy in STEPS:
            nxt = (x + dx, y + dy)
            if nxt in dist or nxt in blocked or grid[nxt[1]][nxt[0]] == "#":
                continue
            dist[nxt] = dist[(x, y)] + 1
            queue.append(nxt)
    return dist


def shortest_path(grid, start, goal):
    parent = {start: None}
    queue = deque([start])
    while queue:
        cur = queue.popleft()
        if cur == goal:
            break
        x, y = cur
        for dx, dy in STEPS:
            nxt = (x + dx, y + dy)
            if nxt in parent or grid[nxt[1]][nxt[0]] == "#":
                continue
            parent[nxt] = cur
            queue.append(nxt)
    path = []
    cur = goal
    while cur is not None:
        path.append(cur)
        cur = parent[cur]
    return path[::-1]


def all_connected(grid):
    floors = [(x, y) for y in range(ROWS) for x in range(COLS) if grid[y][x] != "#"]
    return len(bfs(grid, floors[0])) == len(floors)


def is_corridor(grid, x, y):
    left = grid[y][x - 1] != "#"
    right = grid[y][x + 1] != "#"
    up = grid[y - 1][x] != "#"
    down = grid[y + 1][x] != "#"
    return (left and right and not up and not down) or (up and down and not left and not right)


def place_cells(rng, candidates, count, gap, occupied):
    pool = list(candidates)
    rng.shuffle(pool)
    chosen = []
    for cell in pool:
        if len(chosen) == count:
            break
        if all(max(abs(cell[0] - o[0]), abs(cell[1] - o[1])) >= gap for o in occupied + chosen):
            chosen.append(cell)
    if len(chosen) < count:
        return None
    occupied.extend(chosen)
    return chosen


def generate(level, seed):
    rng = random.Random(seed)
    plan = PLAN[level]
    rooms = place_rooms(rng, plan["rooms"], plan["max_w"], plan["max_h"])
    if len(rooms) < 4:
        return None
    grid = [["#"] * COLS for _ in range(ROWS)]
    for room in rooms:
        carve_room(grid, room)
    centers = [(x + w // 2, y + h // 2) for x, y, w, h in rooms]
    for i in range(len(rooms) - 1):
        carve_corridor(grid, centers[i], centers[i + 1], rng.random() < 0.5)
    for _ in range(plan["loops"]):
        i, j = rng.sample(range(len(rooms)), 2)
        carve_corridor(grid, centers[i], centers[j], rng.random() < 0.5)
    interior = [
        (x, y)
        for rx, ry, w, h in rooms
        for y in range(ry + 1, ry + h - 1)
        for x in range(rx + 1, rx + w - 1)
    ]
    rng.shuffle(interior)
    pillars = 0
    for x, y in interior:
        if pillars == plan["pillars"]:
            break
        if grid[y][x] != ".":
            continue
        grid[y][x] = "#"
        if all_connected(grid):
            pillars += 1
        else:
            grid[y][x] = "."
    room_cells = sorted(
        {
            (x, y)
            for rx, ry, w, h in rooms
            for y in range(ry, ry + h)
            for x in range(rx, rx + w)
            if grid[y][x] == "."
        }
    )
    start = (rooms[0][0], rooms[0][1])
    from_start = bfs(grid, start)
    exit_cell = max((c for c in room_cells if c != start), key=lambda c: from_start[c])
    total_floor = len(from_start)
    path = shortest_path(grid, start, exit_cell)
    best = None
    for cell in path[1:-1]:
        x, y = cell
        if not is_corridor(grid, x, y):
            continue
        reach = bfs(grid, start, {cell})
        if exit_cell in reach:
            continue
        exit_side = total_floor - len(reach) - 1
        if exit_side < 8:
            continue
        if best is None or len(reach) > best[0]:
            best = (len(reach), cell)
    if best is None:
        return None
    door = best[1]
    near = bfs(grid, start, {door})
    key_cell = max((c for c in room_cells if c in near and c != start), key=lambda c: near[c])
    after = bfs(grid, start)
    occupied = [start, exit_cell, key_cell, door]
    free = [c for c in room_cells if c not in set(occupied)]
    enemy_cells = [c for c in free if after[c] >= MIN_SPAWN_DISTANCE]
    enemies = None
    for gap in (3, 2):
        trial = list(occupied)
        enemies = place_cells(rng, enemy_cells, CHASERS[level] + SHOOTERS[level], gap, trial)
        if enemies is not None:
            occupied = trial
            break
    if enemies is None:
        return None
    enemy_set = set(enemies)
    pool = [c for c in free if c not in enemy_set]
    side = [c for c in pool if c in near and near[c] >= 4]
    first_health = place_cells(rng, side, 1, 2, occupied)
    first_ammo = place_cells(rng, side, 1, 2, occupied)
    if first_health is None or first_ammo is None:
        return None
    rest_health = place_cells(rng, pool, HEALTH[level] - 1, 2, occupied)
    rest_ammo = place_cells(rng, pool, ammo_packs(level) - 1, 2, occupied)
    if rest_health is None or rest_ammo is None:
        return None
    chasers = CHASERS[level]
    marks = [
        ([start], "P"),
        ([exit_cell], "E"),
        ([key_cell], "K"),
        ([door], "D"),
        (enemies[:chasers], "Z"),
        (enemies[chasers:], "S"),
        (first_health + rest_health, "H"),
        (first_ammo + rest_ammo, "A"),
    ]
    for cells, ch in marks:
        for x, y in cells:
            grid[y][x] = ch
    return ["".join(row) for row in grid]


def validate(rows, level):
    if len(rows) != ROWS or any(len(r) != COLS for r in rows):
        return False
    if any(c not in ALLOWED for r in rows for c in r):
        return False
    if set(rows[0]) != {"#"} or set(rows[-1]) != {"#"}:
        return False
    if any(r[0] != "#" or r[-1] != "#" for r in rows):
        return False
    cells = {}
    for y, r in enumerate(rows):
        for x, c in enumerate(r):
            cells.setdefault(c, []).append((x, y))
    if any(len(cells.get(k, [])) != 1 for k in "PEKD"):
        return False
    if len(cells.get("Z", [])) != CHASERS[level] or len(cells.get("S", [])) != SHOOTERS[level]:
        return False
    if len(cells.get("H", [])) != HEALTH[level] or len(cells.get("A", [])) != ammo_packs(level):
        return False
    grid = [list(r) for r in rows]
    start, door = cells["P"][0], cells["D"][0]
    key_cell, exit_cell = cells["K"][0], cells["E"][0]
    before = bfs(grid, start, {door})
    after = bfs(grid, start)
    floors = sum(1 for r in rows for c in r if c != "#")
    if key_cell not in before or exit_cell in before or exit_cell not in after:
        return False
    if len(after) != floors:
        return False
    if before[key_cell] + bfs(grid, key_cell)[exit_cell] < min_route(level):
        return False
    return all(after[c] >= MIN_SPAWN_DISTANCE for c in cells["Z"] + cells["S"])


def main():
    out_dir = Path(sys.argv[1])
    out_dir.mkdir(parents=True, exist_ok=True)
    for level in range(2, 11):
        seed = level * 1000
        rows = None
        for attempt in range(20000):
            rows = generate(level, seed + attempt)
            if rows is not None and validate(rows, level):
                break
            rows = None
        if rows is None:
            raise SystemExit(f"level {level}: no valid layout found")
        (out_dir / f"level{level}.txt").write_text("\n".join(rows) + "\n")
        print(f"level{level}.txt seed {seed + attempt}")


main()
