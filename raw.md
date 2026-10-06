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
- A: avatar 124² r36 (initials 42/600 `#2A72C4` on `--accent-soft`), name 22/600, handle 13.5, status pill h≈26 (5×11 padding, 12px text, 6px live dot).
- B: avatar 82² r26 (initials 28/600), name 19/600, sub 13, bio tile: `--subtle` r14, 13.5/1.5.
- Action row: h44 pills, gap 9, padding 0 20; Message flex 2.2, Call flex 1.1, ··· fixed 50.
- Info rows: container r18 + 1px border; row padding 12×15; icon tile 33² r10; k 12.5 / v 14/500.

**Song card (A/C/D/E/F):** h84–88, r22–24, padding 0 14–16, gap 13–15.
- cover 54–58² r14–15; vinyl mark = circle + inset-10 center dot.
- title 15–15.5/600; artist 12.5; timecode mono 11 with 3px dot separator.
- play circle 42–44 `--accent`, white triangle 15–16px.
- patterns: slices = 5–6 bars w9–10 r999 `--accent-tint` at right:100px; arcs = 3 circles border 10px; dots = `radial-gradient` 1.6px/14px; steps = 12px bars r6 gray; split = `linear-gradient(90deg, soft 0 62%, tint 62%)`.
- compact row: h62 r18, thumb 42² r11, eq bars 3.5px w.

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

## 4 · Map to tdesktop source (implementation)

| Feature | Where |
|---|---|
| Design tokens | `Telegram/SourceFiles/astra/astra_design.h` (exists — **needs rework to v2 tokens**, drop glass values) |
| Waveform widget | `astra_waveform.{h,cpp}` (exists — keep for player scrubber later; MD3 slider is the primary control) |
| Profile UI | `Telegram/SourceFiles/boxes/profile/` → new `astra/profile/` module (AstraProfileBox) |
| Song card / playlist sheet | new `astra/music/` widgets (custom `RpWidget` painters) |
| Player UI | `Telegram/SourceFiles/media/` → `AstraPlayerBar` (MD3). Playback core `Media::Player::Instance` unchanged |
| Theme/style | `style/` + `lib_ui` — map tokens to `.style` variables; light theme first |
| Icons | stroke icons 2px (mockups use hand-drawn SVGs). Solaricons (480.design) if license clears for GPL redistribution; else Phosphor/Lucide (MIT) |

Build registration: every new `.cpp` must be added to `Telegram/CMakeLists.txt` (alphabetical, `astra/` entries exist).

## 5 · Repo / CI state

- Repo: `haerin-tg/katsuki-desktop` — **standalone** (NOT a fork), full history (26,611 commits), default branch `dev`.
- Branches: `dev`, `astra-ui` (old probe commits: `4bfa6d8` tokens+waveform, noise commits `ca51afe/90be22a/a3ca3b9/8a86d49` — squash later).
- CI: 20 upstream workflows; push to `dev` triggers Linux/Windows/MacOS/MacOS-Packaged/Snap. Dev builds use `TDESKTOP_API_TEST=ON` (no secrets). Release builds later: secrets `TDESKTOP_API_ID` / `TDESKTOP_API_HASH` from my.telegram.org.
- `paths-ignore` in workflows: `docs/**`, `**.md` don't trigger builds → put design assets under `docs/` to keep CI quiet.
- Old fork `haerin-tg/tdesktopmac` — archived; user wants deletion eventually.

## 6 · Tooling quirks (this workspace)

- `tools/gh` binary (v2.63.2) + `gh auth setup-git` required for pushes. Scopes: `repo, read:org, gist, workflow`.
- No local cmake/Qt — compile feedback comes from CI only.
- Screenshots: Chrome copy at `.openclaw/tmp/chrome/chrome` (chmod +x'd), via `AGENT_BROWSER_EXECUTABLE_PATH=/home/work/.openclaw/workspace/.openclaw/tmp/chrome/chrome` + `agent-browser open/set viewport/screenshot`.
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
