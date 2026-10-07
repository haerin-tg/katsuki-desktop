# raw.md — implementation notes (Katsuki Desktop)

> Companion to [`design.md`](design.md). Design intent lives there; numbers, mappings and operational trivia live here.

## 1 · Exact tokens (from the mockups)

```css
/* flat / v2 */
--bg:#FFFFFF; --canvas:#E9EBEF; --subtle:#F3F4F6; --border:#E6E8EB;
--text:#111418; --text-2:#656D76;
--accent:#3390EC; --accent-soft:#E7F1FD; --accent-tint:#D3E5F9;

/* MD3 roles (player) */
--primary:#2E7CD6; --on-primary:#FFFFFF;
--primary-container:#D6E8FB; --on-primary-container:#0B2A47;
--secondary-container:#E3E7ED; --on-secondary-container:#1B1F26;
--surface:#FCFCFD;
--surface-cl:#F7F8FA; --surface-c:#F1F3F7; --surface-ch:#EAEDF2; --surface-chh:#E3E7ED;
--on-surface:#191C21; --on-surface-var:#565D67;
--outline:#BFC5CD; --outline-var:#DEE2E8;

/* shape */
--m3-xs:8px; --m3-s:12px; --m3-m:16px; --m3-l:20px; --m3-xl:28px; --m3-full:999px;
```

Fonts: `Geist` 400/500/600/700 + `Geist Mono` 400/500 via Google Fonts. Base letter-spacing `-0.011em`; titles `-0.02em`; mono micro-labels `0.12–0.14em` uppercase.

## 2 · Component measurements

**Profile panel:** 412px wide, outer r24, 1px border. Topbar: 36px icon buttons r999.
- A: avatar 124² **circle** (initials 42/600 `#2A72C4` on `--accent-soft`), name 22/600, no handle, "last seen recently" pill h≈26 (5×11 padding, 12px, `--subtle` bg / `--text-2` — neutral, no live dot).
- B: avatar 82² **circle** (initials 28/600), name 19/600, sub 13 "last seen recently" (no status pill), bio tile: `--subtle` r14, 13.5/1.5.
- Action row: h44 pills, gap 9, padding 0 20; Message flex 2.2 (accent), Call flex 1.1 (tonal), **bell/mute icon-only** fixed 50×44 tonal r999 (18px stroke-2 bell, no text label).
- Info rows: container r18 + 1px border; row padding 12×15; icon tile 33² r10; k 12.5 / v 14/500.

**Song card (A/C/D/E/F):** h84–88, r22–24, padding 0 14–16, gap 13–15.
- cover 54–58² r14–15; vinyl mark = circle + inset-10 center dot.
- title 15–15.5/600; artist 12.5; timecode mono 11 with 3px dot separator.
- play circle 42–44 `--accent`, white triangle 15–16px.
- patterns: slices = 5–6 bars w9–10 r999 `--accent-tint` at right:100px; arcs = 3 circles border 10px; dots = `radial-gradient` 1.6px/14px; steps = 12px bars r6 gray; split = `linear-gradient(90deg, soft 0 62%, tint 62%)`.
- compact row: h62 r18, thumb 42² r11, eq bars 3.5px w.

**Channel card:** h62, r18, bg `--subtle`, padding 0 13, gap 11, margin-top 9 (below song card). PFP 40² **circle** (initials 13.5/600 `#5A636E` on `#DCE0E6`). Name 13.5/600. Last message 12 `--text-2`, `white-space:nowrap; overflow:hidden; text-overflow:ellipsis` (truncated with `…`). **No chevron.** Faint slice motif (3 bars w7, `#E2E5EA`, right:16px).

**Playlist sheet:** r28, grabber 38×4 `#D8DCE2`; header art 64² r18; Play all h42 pill; rows r14 padding 10×12, idx column 22px mono 11; playing row bg `--accent-soft`.

**Player (MD3):** card 580px, r28, padding 26, bg `--surface-ch`.
- cover 104² r20; title 24/400; artist 14; assist chip h32 r999 1px `--outline`.
- slider: track h4 r999, handle 20px circle `--primary`; times mono 11.
- FAB 72 circle `--primary`; tonal ibtn 48 r999 bg `--secondary-container`; standard ibtn 48 transparent.
- segmented: 330×48, 1px border r999, segments split by 1px; selected bg `--secondary-container` + check icon 16.
- up-next: container `--surface-cl` r20 padding 8; items r12 padding 9×12, thumb 44² r12.
- mini player: 580px, `--surface-c`, r20, padding 10×14; top progress 3px 46% `--primary`; thumb 48² r12.

**Shape sheet:** buttons h44/h34 r999 (small 12.5px); chips h34; bubbles r20/8, max-width 78%, padding 11×15; rows r18 container; input h52 r16 1.5px border (focus `--accent`); dialog r24 padding 22.

## 3 · Interaction spec

- Song card click (or `›`) → **playlist sheet** (modal/side panel), same data as profile music.
- Play button → starts track, row gets eq-bars indicator, mini player appears.
- "listening now" pill: shown when playback active — accent-soft pill + animated eq dot.
- Item hover states: rows → `--subtle`; list items → `--surface-c` (MD3 state layer ≈8%).
- Channel card click (anywhere on the card) → opens the attached channel.

**Presentation canvas (mockup pages only, NOT product UI):**
```css
background:
  radial-gradient(at 12% 12%, #FFC2D8 0%, transparent 55%),
  radial-gradient(at 88% 6%, #FFE08F 0%, transparent 50%),
  radial-gradient(at 74% 94%, #AEC8FF 0%, transparent 55%),
  radial-gradient(at 22% 86%, #C9B4FF 0%, transparent 52%),
  radial-gradient(at 55% 42%, #A5E1C2 0%, transparent 48%),
  linear-gradient(135deg,#FBEAF1 0%,#E3EAFC 100%);
```

## 4 · Map to tdesktop source (implementation)

| Feature | Where |
|---|---|
| Design tokens | `Telegram/SourceFiles/katsuki/katsuki_design.h` — **✅ v2 token system landed** (flat palette, MD3 player roles, shape/radius/type scales, `Active` theme alias; see §8) |
| Waveform widget | `katsuki_waveform.{h,cpp}` (exists — keep for player scrubber later; MD3 slider is the primary control) |
| Profile UI | `Telegram/SourceFiles/boxes/profile/` → new `katsuki/profile/` module (KatsukiProfileBox) |
| Song card / playlist sheet / channel card | new `katsuki/music/` widgets (custom `RpWidget` painters) |
| Player UI | `Telegram/SourceFiles/media/` → `KatsukiPlayerBar` (MD3). Playback core `Media::Player::Instance` unchanged |
| Theme/style | `style/` + `lib_ui` — map tokens to `.style` variables; light theme first |
| Icons | stroke icons 2px (mockups use hand-drawn SVGs). Solaricons (480.design) if license clears for GPL redistribution; else Phosphor/Lucide (MIT) |

Build registration: every new `.cpp` must be added to `Telegram/CMakeLists.txt` (alphabetical, `katsuki/` entries exist).

## 5 · Repo / CI state

- Repo: `haerin-tg/katsuki-desktop` — **standalone** (NOT a fork), full history (26,611 commits), default branch `dev`.
- Branches: `dev`, `katsuki-ui` (old probe commits: `4bfa6d8` tokens+waveform, noise commits `ca51afe/90be22a/a3ca3b9/8a86d49` — squash later).
- CI: 20 upstream workflows; push to `dev` triggers Linux/Windows/MacOS/MacOS-Packaged/Snap. Dev builds use `TDESKTOP_API_TEST=ON` (no secrets). Release builds later: secrets `TDESKTOP_API_ID` / `TDESKTOP_API_HASH` from my.telegram.org.
- **CI triage 2026-10-07 (first katsuki-ui CI round): Linux ✅ · Windows ❌ Qt6-only `QMouseEvent::position()` · MacOS ❌ ftp.gnu.org timeout building libiconv (flake) · MacOS-Packaged ❌ `find_library(tlottie)` not found · Snap ❌ `git describe --tags` = "No names found" (no tags were pushed with the history).** Fixes in the same round: Qt5 version guard in `katsuki_waveform.cpp`; tlottie build step added to `mac_packaged.yml`; upstream tag `v7.2.10` fetched + pushed (tag pushes trigger no workflows — all filters are `branches(-ignore)` only).
- **MacOS-Packaged tlottie failure is upstream-inherited:** upstream `tdesktop`'s own `mac_packaged.yml` runs fail the same way (tlottie is not in Homebrew and the packaged build never runs `prepare.py`, which is what builds tlottie for `mac.yml`). Our workflow now builds it exactly like `prepare.py` does (dkaraush/tlottie @ `31f1b542f8…`, `desktop-app/patches` @ `ebcd707f17…`, rust 1.96.1, universal lipo → `local/tlottie`).
- **Platform scope (user decision 2026-10-07): Windows-only product.** `MacOS.` / `MacOS Packaged.` / `Snap.` workflows are disabled (still fixed & kept in-tree for a possible future re-enable). Active: `Windows.` + `Linux.` (kept as the fastest second-compiler error check).
- **Architecture scope (user decision 2026-10-07): Windows x64 only.** `win.yml` matrix trimmed from `arch: [x64_x86, x64, arm64]` × `qt: ["", qt6]` × `generator: ["", "Ninja Multi-Config"]` (8 jobs) to `arch: [x64]` × `qt: ["", qt6]` × `generator: [""]` (2 jobs). Qt5 (`qt: ""`) kept as a compat canary, Qt6 is the shipping build. Linux job was already x64-only (Rocky Linux 8 in the `centos_env` Docker image). All targets build from the same code — no extra effort per target.
- `paths-ignore` in workflows: `docs/**`, `**.md` don't trigger builds → put design assets under `docs/` to keep CI quiet.
- Old fork `haerin-tg/tdesktopmac` — archived; user wants deletion eventually.

## 6 · Tooling quirks (this workspace)

- `tools/gh` binary (v2.63.2) + `gh auth setup-git` required for pushes. Scopes: `repo, read:org, gist, workflow`.
- No local cmake/Qt — compile feedback comes from CI only. For Qt-version-sensitive code, the **dual-path stub harness** applies: minimal Qt stubs in workspace `.openclaw/tmp/qtstub/` (`qt5/` defines `QT_VERSION` 5.x with Qt5-only surface, `qt6/` with Qt6-only surface, `common/` shared), then `g++ -fsyntax-only` both paths. Mutually exclusive stubs prove the version guard picks the right API (negative control must fail).
- Screenshots: agent-browser needs a chmod +x'd Chromium (`AGENT_BROWSER_EXECUTABLE_PATH=...`); system `/opt/ms-playwright` binaries are root-owned without +x. The workspace-local Chrome copy was removed (workspace slim-down) — re-copy from Playwright if screenshots are needed again.
- Shallow clones can't be pushed (`index-pack failed`); already unshallowed.
- Empty commits never trigger these workflows (paths-ignore over zero changed files).

## 7 · Open questions

- [ ] Dark flat theme (same geometry, token swap) — user asked "want dark?" → pending answer
- [ ] Accent stays `#3390EC`? (Telegram blue default; user hasn't objected)
- [ ] Song-card background winner: A vs C/D/E/F — A is the favorite so far
- [ ] Profile variant: A (centered) vs B (compact) — pending
- [ ] Name/branding details (client name confirmed: **Katsuki Desktop**)
- [ ] Solaricons license verification before shipping icons
- [ ] Real api_id/api_hash as CI secrets (release builds only)

## 8 · Implementation log

### 2026-10-07 — v2 design token system (first real code step)

**Files changed** (branch `katsuki-ui`, `Telegram/SourceFiles/katsuki/`):

1. `katsuki_design.h` — rewritten as the v2 token system. All "Sound Glass" values are gone.
   - **Token core is pure `constexpr` C++ (no Qt):** `struct Rgba` + `Rgb(hex)` helper, verifiable outside the tdesktop build.
   - `Shape` — MD3 shape scale 8/12/16/20/28/999 (player).
   - `Radius` — product scale 14/18/24/28/999 (design.md “everything round”).
   - `Type` — MD3-named type scale (24/22/16/14/12.5/11 px, weights 400/500/600), `kFontFamily="Geist"`, `kFontMono="Geist Mono"`, letter-spacing constants.
   - `Space` — 4px base grid (4/8/12/16/20/24).
   - `Light` — flat v2 palette exactly per §1 (`kBg/kCanvas/kSubtle/kBorder/kText/kText2/kAccent/kAccentSoft/kAccentTint`), plus `kOnAccentSoft #2374CC`, `kInitials #2A72C4`, `kPfpBg #DCE0E6`, `kPfpText #5A636E`; nested `Light::M3` = all MD3 color roles per §1.
   - `Active` = namespace alias → `Light`. Components must use `Active`; when the dark scheme is designed (§7), only the alias target changes.
   - **Qt bridge** (`KATSUKI_DESIGN_HAS_QT_COLOR`): `ToQColor(Rgba)` + `Design::Qt::` accessors, compiled only when `<QColor>` is available.
   - Waveform colors (provisional): played = `M3::kPrimary`, unplayed = `M3::kSecondaryContainer`, tick = `M3::kOnPrimaryContainer`. MD3 slider remains the primary control.
2. `katsuki_waveform.cpp` — constructor now pulls defaults from `Design::Qt::Wave*()` (was `Design::Active::Wave*()` glass-era accessors, removed). `setBarColors()` override API unchanged.
3. `katsuki_waveform.h` — comment updated (drops "Sound Glass").
4. `Telegram/CMakeLists.txt` — unchanged; `katsuki/` entries already registered.

**Verification (local, before push):**
- `g++ -std=c++17 -Wall -Wextra -Werror` syntax + codegen on the header with **30+ `static_assert`s pinning every token to the §1 values** — passes.
- Qt-bridge smoke test with a stub `QColor` (accents resolve to `rgba(51,144,236,255)`, wave played `rgba(46,124,214,255)`) — passes.
- Test files live in the workspace (`.openclaw/tmp/token_test.cpp`), intentionally **not** committed — CI compiles the real thing with real Qt.

### 2026-10-07 (evening) — CI repairs: Qt5 compat, tlottie, snap tags

1. `katsuki_waveform.cpp` — `QMouseEvent::position()` is Qt6-only and the Windows CI compiles against Qt5. Replaced both call sites with a `MouseLocalPosition()` helper behind `#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)` (Qt6 → `position()`, Qt5 → `localPos()`). Audited the module for other Qt6-only APIs — none left.
   - **Verified with the dual-path stub harness** (see §6): both `qt5/` and `qt6/` paths pass `g++ -std=c++17 -fsyntax-only -Wall`; negative control (unguarded `position()`) fails on qt5 and passes on qt6 — guard proven, not assumed.
2. `.github/workflows/mac_packaged.yml` — added Tlottie cache + build steps (pinned `TLOTTIE`/`PATCHES` shas in `env`, rustup 1.96.1, universal `lipo` → `$LibrariesPath/local/tlottie`). Root-cause details in §5.
3. Snap — pushed upstream tag `v7.2.10` (fetched from `telegramdesktop/tdesktop`; it is an ancestor of `dev`). `snapcraft.yaml`'s `git describe --tags` version suffix now resolves (`v7.2.10-145-g…` at the time of writing). One tag is enough; no need to mirror all upstream tags.

**Branch note:** `katsuki-ui` was reconciled with `dev` first (merged) — the katsuki module now exists on top of the latest design docs; this removes the drift where `dev` had newer docs but no code and `katsuki-ui` had code on stale docs.

**Next up (roadmap order):** theme → `.style`/`lib_ui` mapping, then `katsuki/profile/` (profile box: hero + compact variants, song card, channel card, info rows, profile tab pill), then `katsuki/music/` + `KatsukiPlayerBar` (MD3).
