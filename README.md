<div align="center">

<img src="docs/assets/banner.svg" alt="Katsuki Desktop — a music-first Telegram client with the Sound Glass design language" width="100%"/>

<br/>

[![CI](https://github.com/haerin-tg/katsuki-desktop/actions/workflows/linux.yml/badge.svg)](https://github.com/haerin-tg/katsuki-desktop/actions/workflows/linux.yml)
[![License: GPL-3.0](https://img.shields.io/badge/License-GPL%203.0-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20macOS%20%7C%20Linux-lightgrey.svg)](#-building)
[![Status](https://img.shields.io/badge/status-early%20alpha-orange.svg)](#-roadmap)
[![Based on tdesktop](https://img.shields.io/badge/based%20on-telegramdesktop%2Ftdesktop-26A5E4.svg)](https://github.com/telegramdesktop/tdesktop)

**A Telegram Desktop client where your profile is a listening room.** 💿

*勝 — "victory". Built for people who think a chat client should feel like an album.*

</div>

---

<img src="docs/assets/hero.png" alt="Katsuki Desktop mockup — profile, record shelf, floating glass player, macOS dock" width="100%"/>

---

## 💢 Why another Telegram client?

Every existing fork reskins the same settings-list profile. Katsuki rebuilds the parts that actually say something about you — around **music**.

### 1 · 🪩 The Sound Identity Card *(profile, reinvented)*

No fork has ever done this: the profile isn't a list of info rows — it's a **personal music space**.

- **Full-bleed ambient cover** — gradient mesh derived from the avatar, textured with a live audio waveform
- **"Listening now" badge** — animated equalizer + current track, right on the profile
- **Stat chips** instead of plain rows — mutual groups, shared media, files, tracks, premium
- Squircle avatar, glass action buttons, macOS typography rhythm

### 2 · 💿 The Record Shelf *(playlists, reinvented)*

Playlists rendered as **vinyl records peeking out of album sleeves** — nobody has ever done this in a Telegram client.

- Hover a sleeve → the disc **slides out** with spring physics; the playing one **spins forever**
- Live playback position printed under each sleeve (`▶ 3:24 / 4:01`)
- **Now Spinning** rail — giant rotating vinyl with label art + live tracklist

### 3 · 🎛️ The floating glass player *(player, reinvented)*

Not a corner toolbar — a **floating glass capsule** with a mini spinning vinyl, full transport controls, volume, and a **SoundCloud-style waveform scrubber** (played bars light up as the track moves).

### 4 · 🖥️ The macOS dock *(on Windows, Linux, everywhere)*

A floating glass dock at the bottom — Chats · Contacts · Calls · Music · Settings — with authentic **macOS hover magnification**, gradient active tile, and traffic-light window chrome.

---

## 🎨 Design language — "Sound Glass"

Everything floats. Nothing is flat. Dark-native by default.

| Token | Value | Role |
|---|---|---|
| Base | `#0A0B0F` | deep neutral canvas |
| Glass | `rgba(255,255,255,.055)` + blur | every panel |
| Accent | `#7C5CFF → #4EA8FF` | gradient primary |
| Live | `#3ED598` | "listening now" |
| Radius | 26 / 18 / 12 px | rounded-everything |
| Type | Manrope + Inter | SF-substitute rhythm |

![base](https://img.shields.io/badge/-base%200A0B0F-0A0B0F)
![glass](https://img.shields.io/badge/-glass%20ffffff0e-3A3D4A)
![accent](https://img.shields.io/badge/-accent%207C5CFF-7C5CFF)
![accent](https://img.shields.io/badge/-accent%204EA8FF-4EA8FF)
![live](https://img.shields.io/badge/-live%203ED598-3ED598)

A light variant — **"Sound Paper"** — ships later via the token system (nothing hardcoded).

---

## 🗺️ Roadmap

- [x] Phase 0 — repo, CI, provenance
- [x] Phase 1 — design tokens + waveform scrubber widget
- [ ] Phase 2 — theme system in `style/` + `lib_ui` (glass materials)
- [ ] Phase 3 — dock + traffic-light titlebar
- [ ] Phase 4 — floating glass player
- [ ] Phase 5 — Sound Identity Card (profile)
- [ ] Phase 6 — Record Shelf + Now Spinning
- [ ] Phase 7 — polish: springs, spin, reduced-motion, RTL, light theme

---

## 🛠️ Building

Built on the [Telegram Desktop](https://github.com/telegramdesktop/tdesktop) codebase (~10M lines of C++/Qt). CI does the heavy lifting; local builds need the usual tdesktop toolchain (Qt 6, CMake, Ninja, C++20).

```bash
git clone https://github.com/haerin-tg/katsuki-desktop.git
cd katsuki-desktop
# set your API credentials (see below), then the standard tdesktop CMake flow
```

**API credentials:** Telegram requires every client to have its own `api_id` / `api_hash` — get them free at [my.telegram.org](https://my.telegram.org) (*API development tools*). Development builds use tdesktop's public **test credentials** (`TDESKTOP_API_TEST=ON`); release builds need your real keys as CI secrets.

> ⚠️ **Heads-up:** third-party clients carry account risk. Use a test account while tinkering, and don't do anything against the Telegram ToS.

---

## 🙏 Credits

- **[Telegram Desktop](https://github.com/telegramdesktop/tdesktop)** — the codebase this all stands on. Thank you.
- **[AyuGram](https://github.com/AyuGram/AyuGramDesktop)** & friends — proof that a fork can have a soul
- **[Solaricons](https://www.480.design/solaricons)** by 480.design — the icon set *(license under review for redistribution)*
- Everyone who ever said *"I wish the profile looked like a record store"*

---

## 📜 License

[GPL-3.0](LICENSE) — same as Telegram Desktop. Keep the license and copyright notices, and you're golden.

*Not affiliated with Telegram Messenger Inc. "Telegram" is a trademark of Telegram Messenger Inc.*
