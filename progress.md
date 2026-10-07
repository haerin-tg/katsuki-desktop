# progress.md — Katsuki Desktop

> Living status file. What's done, what's broken, what's next — kept current every session.
> Design intent: [`design.md`](design.md) · tokens/measurements/implementation map: [`raw.md`](raw.md).

**Last updated:** 2026-10-07 (evening session)

## Where we are

Roadmap position: **step 1 done** (design tokens + waveform module landed and merged to `dev`), **CI hardening round complete**, step 2 (themes in `style/` + `lib_ui`) is next.

| | |
|---|---|
| Repo | [`haerin-tg/katsuki-desktop`](https://github.com/haerin-tg/katsuki-desktop) — standalone, full tdesktop history (26,611 commits), default branch `dev` |
| Branches | `dev` (stable), `katsuki-ui` (feature work → merges into `dev`) |
| Module | `Telegram/SourceFiles/katsuki/` — `katsuki_design.h` (v2 tokens), `katsuki_waveform.{h,cpp}` |
| Docs | `design.md` + `raw.md` + `progress.md` (this file) + `docs/design-mockups/` — canonical, pushed to the repo |

## CI matrix

Status after the 2026-10-07 hardening round (details & root causes: [`raw.md`](raw.md) §5/§8):

| Platform | Before | Fix | After |
|---|---|---|---|
| Linux | ✅ | — | ✅ expected |
| Windows | ❌ `QMouseEvent::position()` (Qt6-only) in waveform | Qt5/Qt6 version guard (`MouseLocalPosition()`) | ✅ expected |
| MacOS | ❌ ftp.gnu.org timeout building libiconv | network flake — rerun | ⏳ pending verification |
| MacOS-Packaged | ❌ `find_library(tlottie)` missing (upstream-inherited) | tlottie build step in `mac_packaged.yml` (mirrors `prepare.py`) | ⏳ pending verification |
| Snap | ❌ `git describe --tags` → "No names found" | pushed upstream tag `v7.2.10` (ancestor of `dev`) | ✅ expected |

Local verification convention (no local Qt/cmake): **dual-path stub harness** — compile `katsuki/` sources with `g++ -fsyntax-only` against mutually exclusive Qt5/Qt6 stub trees; the version guard is proven by a negative control. See `raw.md` §6.

## Roadmap

- [x] 0. Repo prep: standalone repo, gh auth, Actions enabled, CI green loop
- [x] 1. First module: design token system v2 + waveform widget (`46850f6a80` + Qt5 compat fix)
- [ ] 2. **Design tokens + themes** in `style/` + `lib_ui` — extend `katsuki_design.h` into real theme files
- [ ] 3. **Dock + titlebar** — custom titlebar, floating dock with magnification math
- [ ] 4. **Floating player** — `KatsukiPlayerBar` custom `RpWidget`, playback core untouched
- [ ] 5. **Profile hero** — `KatsukiProfileBox` (hero + compact variants per design.md §1)
- [ ] 6. **Record Shelf + Now Spinning** — vinyl painter, needs `Data::ProfileMusic` plumbing
- [ ] 7. **Polish** — motion, hover feel, reduced-motion, RTL, dark theme (token swap)

Each step: code → `katsuki-ui` → CI → verify → merge `dev`.

## Session log

### 2026-10-07 (evening) — resumed after compute-quota kill; CI repairs
- GitHub re-auth (device flow), full workspace restored from the exported snapshot.
- Applied & verified the Qt5 waveform fix that was mid-flight when the previous session died (dual-path stub check: qt5 ✅ qt6 ✅ negative control ✅).
- Root-caused all remaining CI failures (table above). tlottie/MacOS-Packaged is upstream-inherited — upstream tdesktop's own `mac_packaged` runs fail identically.
- Fixed `mac_packaged.yml` (tlottie from source, pinned shas) + snap versioning (tag `v7.2.10` pushed).
- Docs: `design.md`/`raw.md` updated, this file created.

### 2026-10-07 (earlier sessions)
- Repo migration to standalone `katsuki-desktop`; stale "Astra UI" About cleaned (9 topics added).
- v2 design direction locked (flat, no glassmorphism, MD3 player, Geist, `#3390EC`); final design pass landed in `design.md`/`raw.md`.
- v2 token system + waveform rework committed and merged to `dev` (`46850f6a80`).

## Open items / TODOs

| Item | Notes |
|---|---|
| Verify CI after hardening round | Windows/MacOS-Packaged green expected; rerun MacOS on flake |
| Real `api_id`/`api_hash` | user creates app at my.telegram.org → Actions secrets, release builds only |
| Client name | Katsuki Desktop (勝) — confirmed working name |
| Old fork `tdesktopmac` deletion | needs `delete_repo` scope or user via Danger Zone |
| Solaricons license | verify GPL-compatible redistribution before shipping; fallback Phosphor/Lucide |
| Trademark hygiene | remove tdesktop marks before public release |
| Open design picks (`raw.md` §7) | dark theme?, profile A vs B, song-card bg winner, accent confirm |
