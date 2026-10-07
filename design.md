# Katsuki Desktop — Design

> **Status:** direction locked (2026-10-07). Mockups live in `docs/design-mockups/` (HTML + PNG).
> Engineering notes → [`raw.md`](raw.md).

## Philosophy

**Flat. Round. Honest.** A Telegram client should look like Telegram — solid fills, one accent color, familiar bones — but *better shaped*. The UI language is:

1. **Flat surfaces** — solid fills and 1px borders only. **No glassmorphism, no blur, no glow, no decorative gradients.** The only "effect" is contrast.
2. **RabbitGram-style roundness** — every container is generously rounded; pills are fully round; even chat bubbles are heavily rounded.
3. **Material 3 for the player** — the music player follows the MD3 component system (shape scale, type scale, slider, segmented buttons, tonal surfaces).
4. **Geist typography** (Vercel) — precise, geometric, tight tracking (`-0.011em` base). Geist Mono for timecodes and micro-labels.
5. **Single-color theme** — one accent (`#3390EC`, Telegram blue), neutrals, and accent tints. Nothing else.

> v1 ("Sound Glass", glassmorphism) is dead and buried. Never revive it.

---

## Typography

| Role | Font | Size / weight |
|---|---|---|
| Headlines | Geist | 24 / 400 (MD3 headline-small rhythm) |
| Titles | Geist | 15–22 / 600, tracking `-0.02em` |
| Body | Geist | 13–14 / 400–500 |
| Micro-labels, timecodes | Geist Mono | 9.5–12 / 400–500, uppercase tracking `0.12–0.14em` |

Fallback stack: `-apple-system, 'Segoe UI', sans-serif`.

---

## Color system (light)

| Token | Value | Use |
|---|---|---|
| `--bg` | `#FFFFFF` | panels |
| `--canvas` | `#E9EBEF` | window background |
| `--subtle` | `#F3F4F6` | tonal buttons, icon tiles |
| `--border` | `#E6E8EB` | 1px separators/outlines |
| `--text` | `#111418` | primary text |
| `--text-2` | `#656D76` | secondary text |
| `--accent` | `#3390EC` | THE color — actions, play, live |
| `--accent-soft` | `#E7F1FD` | accent tint surfaces (song card, chips) |
| `--accent-tint` | `#D3E5F9` | patterns inside accent surfaces |

MD3 player role mapping: `primary #2E7CD6`, `primary-container #D6E8FB`, `secondary-container #E3E7ED`, surface ladder `#FCFCFD / #F7F8FA / #F1F3F7 / #EAEDF2 / #E3E7ED`, `on-surface #191C21`, `on-surface-variant #565D67`, `outline #BFC5CD`.

**Dark variant:** pending (same geometry, tokens swap).

---

## Shape language

| Element | Radius |
|---|---|
| Profile panel / playlist sheet | 24–28 px |
| Song card ("holding pill") | 22–24 px |
| Info container, bio tile | 14–18 px |
| Cover thumbnails | 14–15 px (small 11–12) |
| Avatar | 36 px (124²) / 26 px (82²) — rounded square |
| Buttons, chips, pills | `999px` (fully round) |
| Chat bubbles | `20px` body / `8px` tail corner |
| Inputs | 16 px |
| Dialog | 24 px |
| MD3 player shapes | 12 / 16 / 20 / 28 / full (MD3 scale) |

---

## Components

### 1 · Profile (`profile-v2.html`)

Telegram bones, new composition. **No playlist tiles, no "Music" section header** — a profile shows a person, not a library.

- **Variant A — centered hero:** **round** avatar (124², circle) → name (22/600) → **"last seen recently"** pill (neutral gray, same spot) → action pill row → **single song card** → **channel card** → info container (phone / username / bio rows) → **profile tab pill** below the bio (long pill: `Posts · Gifts · Media · Saved · Files · Links`, centered, `Posts` selected in an inner pill; the row may clip at the panel edge — fit what fits). **No `@handle` near the name** — the username lives in the info rows.
- **Variant B — compact desktop panel:** **round** avatar (82²) + name + "last seen recently" sub on one row, action pills, song card, channel card, info rows **(phone / username / bio — same order as A; bio is a row, not a separate tile)**, then the same profile tab pill. **Panel height matches A** — empty white space at the bottom is fine. Denser, tdesktop-panel feel.

Action row: `Message` (accent, labeled) · `Call` (tonal, labeled) · **bell/mute (icon-only tonal, no label)**. No `···` button (the top bar already has one), no gift.

**Presence rule:** Telegram has no "listening" presence — **"listening now" is killed everywhere**. Profiles show last-seen only, like real TG. (Music-activity display would be a fork-local invention; explicitly not wanted.)

**PFPs are round:** profile avatar and channel pfp are **circles**. Music covers stay rounded-squares (album art, not people).

**Interaction:** clicking the song card reveals the whole playlist (sheet below).

### 2 · Song card — the "designed pill"

Spec: **cover thumbnail + title + artist (+ mono timecode)** inside a wide rounded pill with a **flat background design**. Never a tiny text-only pill.

Anatomy: container h≈84–88, r22–24 · cover 54–58² r14–15 (vinyl mark inside) · title 15–15.5/600 · artist 12.5 + `· 3:24` (Geist Mono) · play circle 42–44 accent · optional `›` chevron (playlist affordance).

Background designs (flat, single-hue):
- **A · vinyl slices** — rounded accent-tint bars behind the text (approved favorite)
- **C · dot grid + cropped disc** — 1.5px dot pattern + big cropped circle
- **D · quarter arcs** — concentric circle outlines cropped right
- **E · step blocks** — neutral rounded blocks, grayscale variant
- **F · two-tone split** — hard-edge color split + big cropped vinyl

**Compact row** (inside playlists): h62, r18, cover 42², playing indicator = eq bars instead of duration.

### 3 · Channel card (attached channel)

A profile can **attach one channel**. Same card family as the song card, but **tinier and quieter** (neutral surface, so music stays the star). Sits **directly below the music card**.

- Container h≈62, r18, `--subtle` background with a faint slice motif (same motif language as the song card, gray)
- **PFP** 40² r11 (flat avatar/initials) · **channel name** 13.5/600 · **last message** 12 secondary — single line, **truncated with `…` when too long**
- Trailing chevron **removed** — tapping anywhere on the card opens the channel
- Anatomy mirrors the song card (thumb + meta) at ≈0.75 scale

### 4 · Playlist sheet (`music-sheet.html`)

Click song → reveal. Flat sheet, r28, grabber bar, header (album art 64² r18 + title + `24 tracks · 1h 32m` mono + `Play all` pill), numbered track list (r14 rows), playing row = accent-soft background + eq bars.

### 5 · Player — Material 3 (`player-v2.html`)

Genuine MD3 composition, not just round corners:

- **Surface:** `surface-container-high` card, r28 (MD3 extra-large)
- **Type:** headline-small 24/400 title · body-medium 14 artist · label-small mono timecodes
- **Slider:** 4px track (inactive = secondary-container, active = primary), 20px round handle
- **Controls:** circular FAB 72 (primary) · tonal icon buttons 48 for prev/next · standard icon buttons 48 for shuffle/repeat · like = tonal icon button
- **Segmented button** (repeat modes): `Off · Repeat all · One` — 1px outline, full-round, selected = secondary-container + check icon
- **Assist chip:** "This device" output selector (outline, full-round)
- **Up next:** MD3 list in `surface-container-low`, r20 container; items = 44² thumb r12 + title/body + mono duration; playing item gets eq bars
- **Mini player:** `surface-container` bar r20 — 3px top progress line, 48² thumb, standard icon buttons, tonal play

### 6 · Shape sheet (`shapes.html`)

System reference: buttons (primary/tonal/neutral/outline + small), chips (selected/outline), super-round chat bubbles, list rows, inputs (default/focused), dialog. Approved as-is.

---

## Full-app UI expansion (mandate 2026-10-07)

The redesign covers **the whole client**, not just the player/profile islands. Every surface
gets the same language shown in `shapes.html` (**rabbitgram-round · flat surfaces · one accent**):
heavy rounding on the MD3/product scales, flat solid surfaces (no glass, no gradients in product
UI), Geist type scale, single accent `#3390EC`, MD3 state layers for hover/press.

Surfaces to design & implement:

| Surface | Look & feel | Notes |
|---|---|---|
| **Chat view** | bubbles, message groups, replies, composer, chat header, date dividers, service messages | the `shapes.html` language applied to conversation — the face of the app |
| **Chat list** | dialog rows (avatar, name, preview, time, unread badge, pinned, muted, verified), search field | rows = rounded cards or clean flat rows with rounded hover state |
| **Folder bar** | folder tabs / pills, unread counters, All-chats state | pill/tab shape per the shape scale, accent for selected |
| **Settings** | section cards, setting rows, toggles, profile header | same info-row component as profile |
| **Shared chrome** | main window layout, titlebar, side panel proportions | follows the dock/titlebar roadmap item |

Process per surface (the established loop): **HTML mockup in the design language → PNG render →
user reviews → lock into `design.md`/`raw.md` → implement in `Telegram/SourceFiles/…` → CI.**
Design passes come first; nothing ships to code without a locked mockup.

Status: **chat view v1 ✅ approved 2026-10-07** (`docs/design-mockups/chat-view-v1.{html,png}` —
bubbles r22/tail r8, reply quotes, voice messages with the Katsuki waveform, composer pill).
Next: chat list + folder bar mockup (T2).

## Presentation (mockups only — not product UI)

The mockup pages sit on a **pastel gradient canvas** (pink → yellow → blue → lilac → mint radials over a soft warm base — clearly visible colors, not washed out) so the white panels pop in screenshots and the README. **The product UI itself stays flat and solid** — the gradient is presentation chrome only.

README banner = **typography lockup**: heavy sans `KATSU` + outlined `KI` + Georgia-italic `*` accent + italic serif `desktop` + Geist Mono captions (`/ KATSUKI UI`, `FLAT · ROUNDED · MATERIAL`), on the same pastel gradient. Fonts must be system-safe (Georgia/Menlo fallbacks) since GitHub renders SVGs without webfonts.

## Rules of the system

1. Flat fills only — **no** `backdrop-filter`, blur, glow, or decorative gradient *in the product UI* (pastel gradients are allowed in presentation canvases; functional patterns like dots/stripes/arcs are fine).
2. One accent color. Tints of it only.
3. Everything rounded; pills are fully round.
4. Geist everywhere; Mono only for data-like micro-text.
5. MD3 vocabulary (slider/segmented/chips/FAB/list/type scale) in anything music-related.
6. Profiles stay TG-honest: person info first, one song card, no library sections.

## Implementation status

Code lives in `Telegram/SourceFiles/katsuki/` (branch `katsuki-ui`, merged to `dev`). Token numbers & verification details in [`raw.md`](raw.md) §8. Rolling status & CI matrix in [`progress.md`](progress.md).

**Build targets:** Windows **x64** is the product (Qt6 shipping build, Qt5 kept as a compat canary). Linux x64 is compiled as a fast error check only. macOS/Snap/32-bit/ARM builds are out of scope for now (`raw.md` §5).

| Piece | Status |
|---|---|
| **Design token system v2** (`katsuki_design.h`) | ✅ landed — flat palette, MD3 player color roles, shape/radius/type/space scales, `Active` theme alias (Light), Qt bridge |
| **Waveform scrubber** (`katsuki_waveform.{h,cpp}`) | ✅ reworked onto v2 tokens (optional scrubber skin; MD3 slider is the primary control) — Qt5/Qt6 version-guarded mouse handling |
| **Mini player bar** (`katsuki_player_bar.{h,cpp}`, `Katsuki::PlayerBar`) | ✅ landed — surface-container r20, 3px top progress, 48² thumb, prev/next + tonal play, elided title/subtitle, top-band scrubbing |
| Profile UI (hero + compact, song card, channel card, tab pill) | ⬜ next — `katsuki/profile/` |
| Playlist sheet + song-card variants | ⬜ — `katsuki/music/` |
| MD3 player + mini bar | ⬜ — `KatsukiPlayerBar` |
| Theme → `.style` / `lib_ui` mapping | ⬜ — light first |

## Assets

| File | What |
|---|---|
| `docs/design-mockups/profile.html` / `.png` | profile v1 (superseded, kept for history) |
| `docs/design-mockups/profile-v2.html` / `.png` | **profile — current (A/B, music card + channel card)** |
| `docs/design-mockups/song-card.html` / `.png` | card backgrounds A/B/C |
| `docs/design-mockups/music-sheet.html` / `.png` | cards D/E/F + compact row + playlist sheet |
| `docs/design-mockups/player.html` / `.png` | player v1 (superseded) |
| `docs/design-mockups/player-v2.html` / `.png` | **player — current (MD3)** |
| `docs/design-mockups/shapes.html` / `.png` | shape language sheet (approved) |
