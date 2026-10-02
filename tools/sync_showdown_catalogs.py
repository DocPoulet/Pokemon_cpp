#!/usr/bin/env python3
"""Synchronise les catalogues Pokemon_cpp depuis les données officielles Pokémon Showdown.

Aucune dépendance Python externe n'est requise. Le script télécharge le catalogue
client Showdown pour les attaques et les sources officielles Smogon/Showdown pour
les talents/objets, puis produit les JSON consommés par GameData.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
import urllib.request
from pathlib import Path
from typing import Any

SHOWDOWN = "https://play.pokemonshowdown.com/data"
RAW = "https://raw.githubusercontent.com/smogon/pokemon-showdown/master/data"

SUPPORTED_ABILITIES = {
    "Blaze": "Blaze", "Torrent": "Torrent", "Overgrow": "Overgrow",
    "Levitate": "Levitate", "Guts": "Guts", "Primordial Sea": "PrimordialSea",
    "Desolate Land": "DesolateLand", "Delta Stream": "DeltaStream",
    "Static": "Static", "Adaptability": "Adaptability",
}
SUPPORTED_ITEMS = {
    "Leftovers": "Leftovers", "Life Orb": "LifeOrb", "Red Orb": "RedOrb",
    "Blue Orb": "BlueOrb", "Air Balloon": "AirBalloon",
}
STATUS_MAP = {"brn": "Burn", "psn": "Poison", "tox": "Poison", "par": "Paralysis", "slp": "Sleep", "frz": "Freeze"}
STAT_MAP = {"atk": "Attack", "def": "Defense", "spa": "SpecialAttack", "spd": "SpecialDefense", "spe": "Speed"}
ACC_MAP = {"accuracy": "Accuracy", "evasion": "Evasion"}
WEATHER_MAP = {"sunnyday": "Sun", "raindance": "Rain", "sandstorm": "Sandstorm", "snow": "Snow", "hail": "Snow"}
TERRAIN_MAP = {"electricterrain": "Electric", "mistyterrain": "Misty", "grassyterrain": "Grassy", "psychicterrain": "Psychic"}


def fetch(url: str) -> str:
    req = urllib.request.Request(url, headers={"User-Agent": "Pokemon_cpp catalog sync"})
    with urllib.request.urlopen(req, timeout=30) as response:
        return response.read().decode("utf-8")


def find_close(text: str, open_pos: int) -> int:
    depth = 0
    quote: str | None = None
    escaped = False
    line_comment = False
    block_comment = False
    i = open_pos
    while i < len(text):
        c = text[i]
        n = text[i + 1] if i + 1 < len(text) else ""
        if line_comment:
            if c == "\n": line_comment = False
            i += 1; continue
        if block_comment:
            if c == "*" and n == "/": block_comment = False; i += 2; continue
            i += 1; continue
        if quote:
            if escaped: escaped = False
            elif c == "\\": escaped = True
            elif c == quote: quote = None
            i += 1; continue
        if c == "/" and n == "/": line_comment = True; i += 2; continue
        if c == "/" and n == "*": block_comment = True; i += 2; continue
        if c in "\"'`": quote = c; i += 1; continue
        if c == "{": depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0: return i
        i += 1
    raise ValueError("Objet TypeScript non terminé")


def top_entries(source: str, marker: str) -> dict[str, str]:
    marker_pos = source.index(marker)
    root = source.index("{", marker_pos)
    end = find_close(source, root)
    out: dict[str, str] = {}
    i = root + 1
    while i < end:
        while i < end and (source[i].isspace() or source[i] == ","): i += 1
        if source.startswith("//", i):
            i = source.find("\n", i)
            if i < 0: break
            continue
        if source.startswith("/*", i):
            j = source.find("*/", i + 2)
            i = end if j < 0 else j + 2
            continue
        if i >= end: break
        if source[i] in "\"'":
            q = source[i]; i += 1; k = i
            while i < end and source[i] != q: i += 2 if source[i] == "\\" else 1
            key = source[k:i]; i += 1
        else:
            m = re.match(r"[A-Za-z0-9_]+", source[i:])
            if not m: i += 1; continue
            key = m.group(0); i += len(key)
        while i < end and source[i].isspace(): i += 1
        if i >= end or source[i] != ":": continue
        i += 1
        while i < end and source[i].isspace(): i += 1
        if i >= end or source[i] != "{":
            while i < end and source[i] != ",": i += 1
            continue
        close = find_close(source, i)
        out[key] = source[i:close + 1]
        i = close + 1
    return out


def simple_field(body: str, name: str) -> Any:
    m = re.search(rf"^\s*{re.escape(name)}:\s*\"([^\"]*)\"", body, re.M)
    if m: return m.group(1)
    m = re.search(rf"^\s*{re.escape(name)}:\s*'([^']*)'", body, re.M)
    if m: return m.group(1)
    m = re.search(rf"^\s*{re.escape(name)}:\s*(-?\d+(?:\.\d+)?)", body, re.M)
    if m: return float(m.group(1)) if "." in m.group(1) else int(m.group(1))
    m = re.search(rf"^\s*{re.escape(name)}:\s*(true|false|null)", body, re.M)
    if m: return {"true": True, "false": False, "null": None}[m.group(1)]
    return None


def official(num: Any, nonstandard: Any) -> bool:
    return isinstance(num, int) and num > 0 and nonstandard not in {"CAP", "Custom"}


def text_map(source: str, marker: str) -> dict[str, str]:
    result: dict[str, str] = {}
    for key, body in top_entries(source, marker).items():
        desc = simple_field(body, "shortDesc") or simple_field(body, "desc") or ""
        result[key] = str(desc)
    return result


def ratio_percent(value: Any) -> int:
    if isinstance(value, list) and len(value) == 2 and value[1]:
        return round(100 * value[0] / value[1])
    return 0


def add_boosts(effects: list[dict[str, Any]], boosts: dict[str, Any], target: str, chance: int = 100) -> None:
    for stat, stages in boosts.items():
        if stat in STAT_MAP:
            effects.append({"kind":"StatChange","target":target,"stat":STAT_MAP[stat],"stages":int(stages),"chance":chance})
        elif stat in ACC_MAP:
            effects.append({"kind":"AccuracyChange","target":target,"accuracy_stat":ACC_MAP[stat],"stages":int(stages),"chance":chance})


def add_status(effects: list[dict[str, Any]], status: Any, target: str, chance: int = 100) -> None:
    if status in STATUS_MAP:
        effects.append({"kind":"ApplyStatus","target":target,"status":STATUS_MAP[status],"chance":chance})


def move_effects(move: dict[str, Any]) -> list[dict[str, Any]]:
    effects: list[dict[str, Any]] = []
    target = "Self" if move.get("target") in {"self", "allySide", "allyTeam"} else "Opponent"
    if isinstance(move.get("boosts"), dict): add_boosts(effects, move["boosts"], target)
    add_status(effects, move.get("status"), target)
    self_data = move.get("self")
    if isinstance(self_data, dict) and isinstance(self_data.get("boosts"), dict): add_boosts(effects, self_data["boosts"], "Self")
    secondaries = []
    if isinstance(move.get("secondary"), dict): secondaries.append(move["secondary"])
    if isinstance(move.get("secondaries"), list): secondaries += [x for x in move["secondaries"] if isinstance(x, dict)]
    for sec in secondaries:
        chance = int(sec.get("chance", 100))
        if isinstance(sec.get("boosts"), dict): add_boosts(effects, sec["boosts"], "Opponent", chance)
        add_status(effects, sec.get("status"), "Opponent", chance)
        if isinstance(sec.get("self"), dict) and isinstance(sec["self"].get("boosts"), dict):
            add_boosts(effects, sec["self"]["boosts"], "Self", chance)
    weather = move.get("weather")
    if weather in WEATHER_MAP: effects.append({"kind":"SetWeather","target":"Self","weather":WEATHER_MAP[weather]})
    terrain = move.get("terrain")
    if terrain in TERRAIN_MAP: effects.append({"kind":"SetTerrain","target":"Self","terrain":TERRAIN_MAP[terrain]})
    return effects


def sync_moves(out_dir: Path) -> int:
    raw = json.loads(fetch(f"{SHOWDOWN}/moves.json"))
    entries = []
    for key, m in raw.items():
        num = m.get("num", 0); ns = m.get("isNonstandard", "") or ""
        if not official(num, ns): continue
        accuracy = -1 if m.get("accuracy") is True else int(m.get("accuracy", 100))
        multihit = m.get("multihit", 1)
        if isinstance(multihit, list): min_hits, max_hits = int(multihit[0]), int(multihit[1])
        elif isinstance(multihit, int): min_hits = max_hits = multihit
        else: min_hits = max_hits = 1
        crit = 3 if m.get("willCrit") else max(0, int(m.get("critRatio", 1)) - 1)
        entry = {
            "id": m.get("name", key), "type": m.get("type", "Normal"), "power": int(m.get("basePower", 0)),
            "category": m.get("category", "Status"), "accuracy": accuracy, "critical_bonus": crit,
            "pp": int(m.get("pp", 1)), "priority": int(m.get("priority", 0)),
            "recoil_percent": ratio_percent(m.get("recoil")), "drain_percent": ratio_percent(m.get("drain")),
            "min_hits": min_hits, "max_hits": max_hits, "generation": int(m.get("gen", 0) or 0),
            "number": int(num), "non_standard": str(ns), "short_description": str(m.get("shortDesc", "")),
            "mechanics_complete": False, "effects": move_effects(m),
        }
        entries.append(entry)
    entries.sort(key=lambda x: x["id"].casefold())
    (out_dir / "moves.json").write_text(json.dumps({"version":2,"source":"Pokemon Showdown","moves":entries}, indent=2, ensure_ascii=False)+"\n", encoding="utf-8")
    return len(entries)


def sync_catalog_ts(out_dir: Path, kind: str) -> int:
    plural = "abilities" if kind == "ability" else "items"
    marker = "export const Abilities" if kind == "ability" else "export const Items"
    text_marker = "export const AbilitiesText" if kind == "ability" else "export const ItemsText"
    main = top_entries(fetch(f"{RAW}/{plural}.ts"), marker)
    descriptions = text_map(fetch(f"{RAW}/text/{plural}.ts"), text_marker)
    entries = []
    for key, body in main.items():
        num = simple_field(body, "num") or 0
        ns = simple_field(body, "isNonstandard") or ""
        if not official(num, ns): continue
        name = simple_field(body, "name") or key
        gen = simple_field(body, "gen") or 0
        mechanic = (SUPPORTED_ABILITIES if kind == "ability" else SUPPORTED_ITEMS).get(str(name), "None")
        entries.append({"id":str(name),"mechanic":mechanic,"generation":int(gen),"number":int(num),
                        "non_standard":str(ns),"short_description":descriptions.get(key, "")})
    entries.sort(key=lambda x:x["id"].casefold())
    root = {"version":2,"source":"Pokemon Showdown", plural:entries}
    (out_dir / f"{plural}.json").write_text(json.dumps(root, indent=2, ensure_ascii=False)+"\n", encoding="utf-8")
    return len(entries)



def sync_kanto_species(out_dir: Path) -> int:
    """Synchronise les 151 espèces de Kanto avec leurs learnsets Showdown complets."""
    pokedex = json.loads(fetch(f"{SHOWDOWN}/pokedex.json"))
    learnsets = json.loads(fetch(f"{SHOWDOWN}/learnsets.json"))
    moves = json.loads(fetch(f"{SHOWDOWN}/moves.json"))
    move_names = {key: data.get("name", key) for key, data in moves.items()}

    entries: list[dict[str, Any]] = []
    for species_id, data in pokedex.items():
        number = int(data.get("num", 0) or 0)
        if not (1 <= number <= 151) or data.get("baseSpecies"):
            continue
        stats = data.get("baseStats", {})
        ability_map = data.get("abilities", {})
        ability_ids = []
        for slot in ("0", "1", "H", "S"):
            ability = ability_map.get(slot)
            if ability and ability not in ability_ids:
                ability_ids.append(ability)

        learnset = learnsets.get(species_id, {}).get("learnset", {})
        move_pool = sorted({move_names[mid] for mid in learnset if mid in move_names}, key=str.casefold)
        entries.append({
            "number": number,
            "name": data.get("name", species_id),
            "base_stats": {
                "hp": int(stats.get("hp", 0)), "attack": int(stats.get("atk", 0)),
                "defense": int(stats.get("def", 0)), "special_attack": int(stats.get("spa", 0)),
                "special_defense": int(stats.get("spd", 0)), "speed": int(stats.get("spe", 0)),
            },
            "types": list(data.get("types", [])),
            "move_pool": move_pool,
            "abilities": ability_ids,
        })

    entries.sort(key=lambda x: x["number"])
    if len(entries) != 151:
        raise ValueError(f"Catalogue Kanto incomplet: {len(entries)} espèces au lieu de 151")
    for entry in entries:
        entry.pop("number", None)
    root = {
        "version": 2,
        "source": "Pokemon Showdown pokedex + learnsets, espèces #001-151",
        "species": entries,
    }
    (out_dir / "pokemon_species.json").write_text(
        json.dumps(root, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    return len(entries)

def main() -> int:
    parser=argparse.ArgumentParser()
    parser.add_argument("--output", default="data", help="Dossier de sortie (défaut: data)")
    args=parser.parse_args()
    out=Path(args.output); out.mkdir(parents=True, exist_ok=True)
    try:
        m=sync_moves(out); a=sync_catalog_ts(out,"ability"); i=sync_catalog_ts(out,"item"); p=sync_kanto_species(out)
    except Exception as exc:
        print(f"Erreur de synchronisation: {exc}", file=sys.stderr); return 1
    print(f"Catalogues synchronisés: {m} attaques, {a} talents, {i} objets, {p} espèces Kanto.")
    return 0

if __name__ == "__main__": raise SystemExit(main())
